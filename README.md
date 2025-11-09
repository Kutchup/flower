# Flower - A Peaceful Adventure

A first-person camera adventure game built with C++ and SDL3.

## About

"There's no grand villain, no impending doom, only the quiet task of reminding the world of its own splendor."

Flower is a peaceful, happy game where you explore a beautiful world and interact with flowers. The gameplay features first-person shooter mechanics but without violence - instead, you plant flowers, water them, and photograph the beauty around you.

## Features

- **First-Person Camera**: Explore the world with smooth WASD movement and mouse look
- **Grid-Based World**: Navigate a 50x50 grid world filled with possibilities
- **Peaceful Interactions**:
  - Plant flowers of different varieties
  - Water flowers to help them grow
  - Take photographs of beautiful scenes
  - Collect seeds and tools
- **Gameplay Objectives**:
  - Plant flowers across the world
  - Water and care for your garden
  - Discover different flower types
  - Create beautiful landscapes

## Controls

- **WASD** - Move around
- **Mouse** - Look around
- **Space** - Move up
- **Left Shift** - Move down
- **Left Click** - Use current tool (plant/water/photograph)
- **E** - Pick up items
- **ESC** - Exit game

## Building

### Requirements
- CMake 3.20 or higher
- C++17 compatible compiler
- SDL3 (automatically downloaded if not found)
- OpenGL
- On Linux: X11 or Wayland development libraries
  - Ubuntu/Debian: `sudo apt-get install libx11-dev libxext-dev libxrandr-dev libxcursor-dev libxinerama-dev libxi-dev`
  - Or Wayland: `sudo apt-get install libwayland-dev libxkbcommon-dev libegl1-mesa-dev`

### Build Instructions

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install build-essential cmake libx11-dev libxext-dev libgl1-mesa-dev

# Build the project
mkdir build
cd build
cmake ..
cmake --build .

# Run the game
./bin/flower
```

### Windows Build

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
.\bin\Release\flower.exe
```

### macOS Build

```bash
mkdir build
cd build
cmake ..
cmake --build .
./bin/flower
```

## Development Notes

- The game uses OpenGL for 3D rendering
- SDL3 is used for window management, input handling, and cross-platform support
- The code is designed to be portable across Windows, macOS, and Linux
- In headless/CI environments, SDL3 requires video driver support (X11/Wayland on Linux)

## Game Elements

### Tools (Weapons)
- **Watering Can**: Water your flowers to help them grow
- **Seed Planter**: Plant new flowers in the world
- **Camera**: Take photographs of beautiful scenes

### Collectibles (Pickups)
- **Sunflower Seeds**: Bright yellow flowers
- **Rose Seeds**: Red romantic flowers
- **Tulip Seeds**: Pink spring flowers
- **Daisy Seeds**: White cheerful flowers
- **Lavender Seeds**: Purple calming flowers
- **Water Bucket**: Refill your watering can

### World Features
- **Grass**: Natural ground cover
- **Flowers**: Your planted creations with animated petals and stems
- **Grid-Based Layout**: Organized world for strategic planting

## Philosophy

This game is about finding beauty in simplicity and spreading joy. There are no enemies, no combat, no stress - just you, nature, and the task of making the world a little more beautiful, one flower at a time.