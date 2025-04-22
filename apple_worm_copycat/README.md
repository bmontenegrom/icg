# Apple Worm Copycat

A 2D implementation of the classic Apple Worm game using OpenGL, SDL2, and FreeImage.

## Dependencies

- SDL2
- OpenGL
- GLU
- FreeImage

## Installation

### macOS

```bash
brew install sdl2 freeimage
```

### Windows

1. Download SDL2 development libraries from: https://www.libsdl.org/download-2.0.php
2. Download FreeImage from: http://freeimage.sourceforge.net/download.html
3. Extract both to C:\SDL2 and C:\freeimage respectively

## Building the Project

### macOS

```bash
make
```

### Windows

Using MinGW:

```bash
mingw32-make
```

## Running the Game

### macOS

```bash
./bin/apple_worm
```

### Windows

```bash
.\bin\apple_worm.exe
```

## Project Structure

- `src/` - Source files
- `include/` - Header files
- `assets/` - Game assets (images, sounds, etc.)
- `obj/` - Object files (created during build)
- `bin/` - Executable (created during build)
