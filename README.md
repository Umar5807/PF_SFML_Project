# Gravity Flip Runner

A 2D endless runner game made in **C++** using the **SFML** graphics library. The player runs along a track between the floor and the ceiling and can flip gravity to switch sides, while obstacles appear at random.

This is the **mid evaluation build** of our Programming Fundamentals project.

## Features Completed

- Game window (800 x 600) with a 60 FPS game loop
- Infinite scrolling track: red floor and ceiling lines with moving yellow stripes
- Player (cyan block) with gravity and smooth movement
- Gravity flip between the floor and the ceiling using the spacebar
- Random obstacle generator: obstacles appear on the floor or the ceiling after a random delay
- Obstacles are removed from memory once they leave the screen

## Controls

| Key | Action |
|-----|--------|
| Space | Flip gravity (floor <-> ceiling) |
| Close window (X) | Exit the game |

## How to Run

1. Install SFML (version 2.x) on your computer.
2. Compile the code:

```
g++ main.cpp -o game -lsfml-graphics -lsfml-window -lsfml-system
```

3. Run the game:

```
./game
```

On Windows, make sure the SFML libraries are linked in your IDE (for example Visual Studio or Code::Blocks) and the SFML DLL files are next to the executable.

## How the Code Works

The game runs in a loop that repeats 60 times per second. In every round it:

1. Checks keyboard input (`handleEvents`)
2. Updates the player, the stripes and the obstacles
3. Clears the screen and draws everything again

## Code Structure

| Function | What it does |
|----------|--------------|
| `main()` | Creates the window and runs the game loop |
| `setupGame()` | Sets the player's color and position and creates the stripes |
| `handleEvents()` | Closes the window and flips gravity on spacebar |
| `updateStripes()` | Moves the stripes left and loops them back to the right |
| `drawTrack()` | Draws the red lines and yellow stripes |
| `updatePlayer()` | Applies gravity and stops the player at the floor or ceiling |
| `drawPlayer()` | Draws the player |
| `spawnObstacle()` | Creates an obstacle at the right edge (floor or ceiling, chosen randomly) |
| `updateObstacles()` | Spawns obstacles after a random delay, moves them left and removes the ones that left the screen |
| `drawObstacles()` | Draws the obstacles as orange blocks |

## Team Members and Work Division

| Member | Work |
|--------|------|
| **Person A: M. Umar Farooq | Main skeleton, window and game loop, shared variables, events, scrolling track (`main`, `setupGame`, `handleEvents`, `updateStripes`, `drawTrack`) |
| **Person B: M. Hamza Kashif| Player gravity and movement (`updatePlayer`, `drawPlayer`) |
| **Person C: Abdullah Shafique | Random obstacle generator (`spawnObstacle`, `updateObstacles`, `drawObstacles`) |

## Planned Features

- Collision detection with obstacles and a Game Over screen
- Shield and score booster items
- Score system and high score saved in a file
- Start menu and instructions screen
- Speed increasing over time

## Concepts Used

Functions, structs, vectors, loops, random numbers (`rand()`), if/else conditions and the SFML library for graphics and input.
