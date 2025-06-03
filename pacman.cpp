#include <GL/glut.h>
#include <cmath>
#include <cstring>
#include <string>
#include <sstream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

const int rows = 15;
const int cols = 15;
int map[rows][cols];

int directionX = 0, directionY = 0;
int score = 0;
bool gameStarted = false;
bool gameOver = false;
bool gameWon = false;

struct Point {
    int x, y;
    Point(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

Point player(1, 1);
Point ghost(13, 13);

bool isValid(int x, int y);

struct Node {
    int x, y;
    int px, py;
};

vector<Point> bfsPath(Point start, Point target) {
    bool visited[rows][cols] = {false};
    Node parent[rows][cols];
    for (int y = 0; y < rows; ++y)
        for (int x = 0; x < cols; ++x)
            parent[y][x] = {-1, -1, -1, -1};
    queue<Node> q;
    q.push({start.x, start.y, -1, -1});
    visited[start.y][start.x] = true;
    bool found = false;
    while (!q.empty()) {
        Node cur = q.front(); q.pop();
        if (cur.x == target.x && cur.y == target.y) {
            found = true;
            break;
        }
        int dx[4] = {1, -1, 0, 0};
        int dy[4] = {0, 0, 1, -1};
        for (int d = 0; d < 4; d++) {
            int nx = cur.x + dx[d];
            int ny = cur.y + dy[d];
            if (isValid(nx, ny) && !visited[ny][nx]) {
                visited[ny][nx] = true;
                parent[ny][nx] = {cur.x, cur.y, -1, -1};
                q.push({nx, ny, cur.x, cur.y});
            }
        }
    }
    vector<Point> path;
    if (found) {
        int cx = target.x, cy = target.y;
        while (!(cx == start.x && cy == start.y)) {
            path.push_back(Point(cx, cy));
            int px = parent[cy][cx].x;
            int py = parent[cy][cx].y;
            if (px == -1 && py == -1) break; 
            cx = px; cy = py;
        }
        reverse(path.begin(), path.end());
    }
    return path;
}

void resetMap() {
    int newMap[rows][cols] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,2,2,2,1,2,2,2,2,2,1,2,2,2,1},
        {1,2,1,2,1,2,1,1,1,2,1,2,1,2,1},
        {1,2,1,2,2,2,2,1,2,2,2,2,1,2,1},
        {1,2,1,1,1,1,2,1,2,1,1,1,1,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,1,1,1,1,1,2,1,1,1,1,1,2,1},
        {1,2,1,2,2,2,2,0,2,2,2,2,1,2,1},
        {1,2,1,2,1,1,1,1,1,1,1,2,1,2,1},
        {1,2,2,2,1,2,2,2,2,2,1,2,2,2,1},
        {1,2,1,1,1,2,1,1,1,2,1,1,1,2,1},
        {1,2,2,2,2,2,1,2,1,2,2,2,2,2,1},
        {1,2,1,1,1,1,1,2,1,1,1,1,1,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };
    memcpy(map, newMap, sizeof(map));
}

void drawRect(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + width, y);
        glVertex2f(x + width, y + height);
        glVertex2f(x, y + height);
    glEnd();
}

void drawCircle(float cx, float cy, float r, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 100; ++i) {
        float angle = 2.0f * 3.1415926f * i / 100;
        glVertex2f(cx + cos(angle) * r, cy + sin(angle) * r);
    }
    glEnd();
}

void drawString(float x, float y, string text, void* font = GLUT_BITMAP_HELVETICA_18) {
    glColor3f(1, 1, 1);
    glRasterPos2f(x, y);
    for (char c : text)
        glutBitmapCharacter(font, c);
}

void drawMap() {
    for (int y = 0; y < rows; y++) {
        for (int x = 0; x < cols; x++) {
            if (map[y][x] == 1)
                drawRect(x, rows - y - 1, 1, 1, 0.2f, 0.2f, 0.8f);
            else if (map[y][x] == 2)
                drawCircle(x + 0.5f, rows - y - 0.5f, 0.1f, 1.0f, 1.0f, 0.0f);
        }
    }
}

void drawGame() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawMap();

    drawCircle(player.x + 0.5f, rows - player.y - 0.5f, 0.4f, 1.0f, 1.0f, 0.0f);
    drawCircle(ghost.x + 0.5f, rows - ghost.y - 0.5f, 0.4f, 1.0f, 0.2f, 0.2f);

    stringstream ss;
    ss << "Score: " << score;
    drawString(0.5f, rows + 0.3f, ss.str());

    if (!gameStarted) {
        drawRect(3, 5, 9, 5, 0.0f, 0.0f, 0.0f);
        drawString(5.5f, 8.5f, "PACMAN GL");
        drawString(4.3f, 7.5f, "Arrows: Move");
        drawString(4.5f, 6.5f, "R: Restart  Q: Quit");
        drawString(4.5f, 5.5f, "Press ENTER to start");
    }

    if (gameOver) {
        drawRect(3, 5, 9, 5, 0.2f, 0.0f, 0.0f);
        drawString(5.5f, 8.0f, "GAME OVER");
        drawString(4.5f, 7.0f, "Final Score: " + to_string(score));
        drawString(4.5f, 6.0f, "Press R to Restart");
        drawString(4.5f, 5.0f, "Press Q to Quit");
    }

    if (gameWon) {
        drawRect(3, 5, 9, 5, 0.0f, 0.2f, 0.0f);
        drawString(4.5f, 8.0f, "You WON!");
        drawString(4.0f, 7.0f, "Press R to Restart");
        drawString(4.0f, 6.0f, "Press Q to Quit");
    }

    glutSwapBuffers();
}

bool isValid(int x, int y) {
    return x >= 0 && x < cols && y >= 0 && y < rows && map[y][x] != 1;
}

void moveGhost() {
    vector<Point> path = bfsPath(ghost, player);
    if (!path.empty()) {
        ghost = path[0]; 
    }
}

void update(int) {
    if (gameStarted && !gameOver && !gameWon) {
        int newX = player.x + directionX;
        int newY = player.y + directionY;
        if (isValid(newX, newY)) {
            player.x = newX;
            player.y = newY;
            if (map[player.y][player.x] == 2) {
                map[player.y][player.x] = 0;
                score++;
            }
        }

        moveGhost();

        if (player.x == ghost.x && player.y == ghost.y)
            gameOver = true;

        bool win = true;
        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                if (map[y][x] == 2) {
                    win = false;
                    break;
                }
            }
            if (!win) break;
        }
        if (win) {
            gameWon = true;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(200, update, 0);
}

void keyboard(unsigned char key, int, int) {
    if ((gameOver || gameWon) && (key == 'r' || key == 'R')) {
        player = Point(1, 1);
        ghost = Point(13, 13);
        directionX = directionY = 0;
        score = 0;
        gameOver = false;
        gameWon = false;
        resetMap();
    } else if ((gameOver || gameWon) && (key == 'q' || key == 'Q')) {
        exit(0);
    } else if (!gameStarted) {
        gameStarted = true;
    }
}

void special(int key, int, int) {
    if (!gameOver) {
        switch (key) {
            case GLUT_KEY_UP:    directionX = 0; directionY = -1; break;
            case GLUT_KEY_DOWN:  directionX = 0; directionY = 1;  break;
            case GLUT_KEY_LEFT:  directionX = -1; directionY = 0; break;
            case GLUT_KEY_RIGHT: directionX = 1; directionY = 0;  break;
        }
    }
}

void init() {
    glClearColor(0.1f, 0.1f, 0.1f, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, cols, 0, rows + 1);
    resetMap();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(600, 650);
    glutCreateWindow("Pacman GL");
    init();
    glutDisplayFunc(drawGame);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutTimerFunc(200, update, 0);
    glutMainLoop();
    return 0;
}
