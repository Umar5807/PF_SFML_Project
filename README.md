Gravity Flip Runner

A fast-paced 2D endless arcade runner built in C++ using SFML 2.6.2. Navigate a scrolling space-station track on the lunar surface, flip gravity between the floor and ceiling to dodge incoming hazards, collect shields and bonus gems, and beat your saved high score.

Features

Gravity-Flip Mechanics: Instantly reverse gravity mid-run to switch between the floor and ceiling lanes.

Dynamic Scrolling Track: Smooth pixel-based scrolling backed by a 2D grid (track[2][21]) that progressively accelerates as you survive longer.

Procedural Obstacles & Power-Ups:

Orange Blocks: Hazards that end the run unless protected by a shield.

Blue Shield Orbs: Grants a protective energy ring that absorbs one obstacle impact.

Green Booster Gems: Awards +50 bonus points on pickup.

Game States & Pause Support: Complete state flow with a keyboard-navigable Main Menu, How-to-Play Instructions screen, Pause overlay (P), and Game Over summary.

Persistent High Score: Automatically saves and loads the all-time best score using file handling (highscore.txt).

Custom Sprites, Parallax Background & Audio: Includes a scrolling background (background.png), vertically flipping player sprite (player.png), looping background music (music.ogg), and gravity-flip sound effects (flip.wav), with built-in shape fallbacks if any optional asset is missing.

Controls

Key:                        Action


Up / Down Arrow:          Navigate Main Menu options
   
Enter:                     Select Main Menu option

Space:                     Flip gravity (Floor $\leftrightarrow$ Ceiling)

R:                         Restart immediately from the Game Over screen

Esc:                        Return to the Main Menu



Project Structure

├── Main_build.cpp              # Complete C++ source code
├── PF_SFML_Project.exe         # Pre-built Windows executable (x64)
├── SFML.slnx                   # Visual Studio solution file
├── SFML.vcxproj                # Visual Studio project configuration
├── SFML.vcxproj.filters        # Visual Studio project filters
├── background.png              # Scrolling game background
├── player.png                  # Player character sprite
├── flip.wav                    # Gravity-flip sound effect
├── music.ogg                   # Looping background music
├── highscore.txt               # Saved high score data
├── openal32.dll                # OpenAL audio runtime library
└── sfml-*-2.dll                # SFML 2.6.2 runtime libraries


How to Run (Pre-Built Executable)

Extract the submitted .zip archive to a folder on Windows (x64).

Ensure PF_SFML_Project.exe, all .dll files (sfml-*-2.dll and openal32.dll), and the asset files (background.png, player.png, flip.wav, music.ogg) remain in the same folder.

Double-click PF_SFML_Project.exe to launch the game.

How to Compile from Source (Visual Studio + SFML 2.6.2)

Download SFML 2.6.2 matching your Visual Studio compiler version and architecture (x64).

Open SFML.slnx (or SFML.vcxproj) in Visual Studio, or create an Empty C++ Project and add Main_build.cpp.

Open Project Properties (Alt + F7) and configure:

C/C++ $\rightarrow$ General $\rightarrow$ Additional Include Directories: <path-to-SFML-2.6.2>\include

Linker $\rightarrow$ General $\rightarrow$ Additional Library Directories: <path-to-SFML-2.6.2>\lib

Linker $\rightarrow$ Input $\rightarrow$ Additional Dependencies:

For Debug: sfml-graphics-d.lib;sfml-window-d.lib;sfml-system-d.lib;sfml-audio-d.lib;

For Release: sfml-graphics.lib;sfml-window.lib;sfml-system.lib;sfml-audio.lib;

Place the matching SFML .dll files (openal32.dll and sfml-*.dll) and asset files (background.png, player.png, flip.wav, music.ogg) into your working/output directory.

Build and run the project (Ctrl + F5).
