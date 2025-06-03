# PacmanGL - Pacman Clone 
PacmanGL is a simple Pacman clone written in C++ using OpenGL and GLUT. The game features a single ghost that chases the player using a shortest-path algorithm. The goal is to collect all the coins (yellow dots) on the map while avoiding the ghost.

## Features
- Classic Pacman gameplay on a 15x15 grid
- One ghost with smart pathfinding
- Win and game over screens
- Keyboard controls

## Requirements
- WSL2 Ubuntu on Windows 10/11 or Linux Ubuntu operating system 
- OpenGL development libraries
- GLUT (OpenGL Utility Toolkit)

On Ubuntu/Debian or WSL2 Ubuntu, you can install the required libraries with:
```
sudo apt-get install build-essential freeglut3-dev
```

## Build Instructions
To compile the game, run:
```sh
g++ pacman.cpp -o pacman -lGL -lGLU -lglut
```

## Running the Game
After building, start the game with:
```
./pacman
```

![Ekran görüntüsü 2025-06-03 102420](https://github.com/user-attachments/assets/613dccb4-a9f6-453c-b9b2-0a05d8a6da28)

![Ekran görüntüsü 2025-06-03 102431](https://github.com/user-attachments/assets/ec7952c9-97a3-4dfe-bf49-e95421cd302f)

![Ekran görüntüsü 2025-06-03 102455](https://github.com/user-attachments/assets/05cbb7d8-5770-4357-836b-c9d4182d1b7d)

![Ekran görüntüsü 2025-06-03 102620](https://github.com/user-attachments/assets/fddce104-1128-4cd5-87fd-98130e066fe4)

## Controls
- **Arrow Keys:** Move Pacman
- **R:** Restart the game (on win or game over screen)
- **Q:** Quit the game (on win or game over screen)
- **ENTER:** Start the game from the title screen

## Game Rules
- Collect all the coins to win.
- Avoid the ghost. If the ghost catches you, it's game over.
- When you win or lose, you can restart or quit using the keys above.

## Notes
- The game logic and rendering are all in `pacman.cpp`.
- The ghost uses a shortest-path algorithm (BFS) to chase the player.
