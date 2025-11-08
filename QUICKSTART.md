# Quick Start Guide

Welcome to Flower! This guide will help you get started quickly.

## What is Flower?

Flower is a peaceful first-person adventure game where you plant and nurture flowers, take photographs, and create beautiful landscapes. There's no combat, no enemies, just you and nature.

## Installation

### Prerequisites

#### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install build-essential cmake libx11-dev libxext-dev libgl1-mesa-dev
```

#### macOS
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake (via Homebrew)
brew install cmake
```

#### Windows
- Install Visual Studio 2019 or later with C++ support
- Install CMake from https://cmake.org/download/

### Building the Game

```bash
# Clone the repository
git clone https://github.com/Kutchup/flower.git
cd flower

# Build
mkdir build
cd build
cmake ..
cmake --build .

# Run (Linux/macOS)
./bin/flower

# Run (Windows)
.\bin\Release\flower.exe
```

## How to Play

### Controls

| Action | Key/Button |
|--------|------------|
| Move Forward | W |
| Move Backward | S |
| Move Left | A |
| Move Right | D |
| Move Up | Space |
| Move Down | Left Shift |
| Look Around | Mouse |
| Use Tool | Left Click |
| Pick Up Items | E |
| Exit Game | ESC |

### Your First Garden

1. **Start the game** - You'll spawn in the middle of a 50x50 grid world
2. **Look around** - Move your mouse to look in different directions
3. **Find seeds** - Press `E` near colored cubes to collect seeds
4. **Find tools** - Press `E` near tools (watering can, camera)
5. **Plant a flower** - Aim at an empty grass tile and Left Click
6. **Water your flower** - Aim at a planted flower and Left Click
7. **Take a photo** - Look at a beautiful scene and Left Click (with camera equipped)

### Tips for Beginners

- **Explore freely** - There's no time limit or danger
- **Create patterns** - Try making shapes or designs with different colored flowers
- **Mix colors** - Different seed types create different colored flowers:
  - Sunflower Seeds → Yellow flowers
  - Rose Seeds → Red flowers
  - Tulip Seeds → Pink flowers
  - Daisy Seeds → White flowers
  - Lavender Seeds → Purple flowers
- **Take your time** - This is a peaceful game meant to be relaxing
- **Statistics** - Check the console when you exit to see your accomplishments

### Game Objectives

While Flower is designed to be played at your own pace, here are some suggested goals:

- **Beginner**: Plant 10 flowers
- **Intermediate**: Plant 50 flowers and create a pattern
- **Advanced**: Plant 100 flowers across the entire world
- **Master**: Create beautiful gardens in all four corners of the map

## Troubleshooting

### The game won't build
- Make sure you have all prerequisites installed
- Check that CMake version is 3.20 or higher: `cmake --version`
- Ensure you have a C++17 compatible compiler

### The game crashes on startup
- Make sure your graphics drivers are up to date
- Ensure OpenGL is supported on your system
- Try running from the command line to see error messages

### The mouse isn't captured
- Click in the game window to capture the mouse
- If it still doesn't work, check that your window manager allows mouse capture

### I can't see what I planted
- Make sure you're looking down at the ground
- Flowers appear as colored shapes on grass tiles
- Try moving closer to see them better

## Next Steps

- Read the [DESIGN.md](DESIGN.md) file to understand the game philosophy
- Check out [CONTRIBUTING.md](CONTRIBUTING.md) if you want to help improve the game
- Share screenshots of your beautiful gardens!

## Getting Help

If you have questions or run into issues:
1. Check this guide first
2. Look at the main [README.md](README.md) for more details
3. Open an issue on GitHub with a description of your problem

## Have Fun!

Remember, Flower is about enjoying the process of creation. There's no wrong way to play. Take your time, experiment, and most importantly - have fun making the world more beautiful! 🌸🌼🌺🌻🌷
