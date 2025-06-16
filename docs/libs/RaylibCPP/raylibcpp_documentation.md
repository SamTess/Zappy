# RaylibCPP Documentation

## Overview

RaylibCPP is a C++ wrapper for the Raylib library, designed for the Zappy project. It allows you to use Raylib features in an object-oriented way with modern C++ syntax.

## Features

RaylibCPP provides classes and methods for:
- **Window management**: Creating, configuring, and closing windows
- **2D/3D rendering**: Drawing shapes, models, and textures
- **Camera management**: 2D and 3D cameras with different modes
- **Input management**: Keyboard, mouse, and other devices
- **Audio**: Playing sounds and music
- **Textures and images**: Loading and manipulating images and textures
- **3D models**: Loading and rendering 3D models

## Structure

The library is divided into several modules:
- **Window**: Window and rendering context management
- **Shape**: 2D and 3D drawing primitives
- **Texture**: Texture and image management
- **Camera**: Camera control
- **Input**: User input management
- **Audio**: Audio playback and control
- **Font**: Text loading and rendering

## Usage

### Initialization

```cpp
#include "RayLib.hpp"

int main() {
    // Create a window
    RaylibCPP::Window window(800, 600, "My Application");
    
    // Initialize 3D camera
    RaylibCPP::Camera camera(RaylibCPP::Camera::Mode::CAMERA_FREE);
    camera.setPosition({10.0f, 10.0f, 10.0f});
    
    while (!window.shouldClose()) {
        // Logic and rendering
        window.beginDrawing();
        window.clearBackground(RaylibCPP::Color::RAYWHITE);
        
        window.begin3DMode(camera);
        // 3D rendering here
        window.end3DMode();
        
        // 2D rendering here
        
        window.endDrawing();
    }
    
    return 0;
}
```

## Advantages of RaylibCPP

- **Object-oriented**: More intuitive API for C++ programmers
- **RAII management**: Automatic resource release
- **Method chaining**: Fluent programming style
- **Encapsulation**: Better code organization

## Compilation

To compile a program using RaylibCPP:

```bash
g++ -std=c++17 -Ilibs/RaylibCPP my_program.cpp libs/RaylibCPP/*.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Integration with Zappy

RaylibCPP is used in the Zappy project to:
- Display the game map in 3D
- Visualize players and their actions
- Manage the camera and views
- Draw the user interface (in collaboration with RayGUICPP)

## Main Classes

- **RaylibCPP::Window**: Window and rendering context management
- **RaylibCPP::Shape**: 2D/3D shape drawing
- **RaylibCPP::Texture**: Texture management
- **RaylibCPP::Camera**: Camera control
- **RaylibCPP::Input**: Input management
- **RaylibCPP::Audio**: Audio playback
- **RaylibCPP::Font**: Text rendering
