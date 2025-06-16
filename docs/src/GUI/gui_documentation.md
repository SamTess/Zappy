# Zappy Graphical User Interface (GUI) Documentation

## Overview

The Zappy graphical interface is responsible for 3D visualization of the game environment, allowing real-time observation of:
- The terrain and its resources
- Players and their actions
- Game events (incantations, egg hatching, etc.)

## Architecture

The GUI is developed in C++ and uses a modular architecture based on dynamic library loading (DLLoader):

- **Core**: Main loop, event management, coordinator
- **NetworkManager**: Communication with the server
- **Renderer**: 3D and 2D graphical display
- **CameraController**: Camera management and views
- **GUI Components**: User interface elements

## Dynamic Loading System (DLLoader)

The GUI uses a dynamic library loading system to allow the use of different graphical implementations:
- Interface `IGraphicsLib`: Defines the expected functions for graphic libraries
- Interface `IGuiLib`: Defines the expected functions for user interface libraries
- Manager `LibraryManager`: Allows dynamic loading of libraries

This system enables easy switching of graphic implementations without modifying the main code.

## Server Communication

The GUI communicates with the server via a specific protocol described in the "GUI Protocol" documentation. The main commands include:
- Map and cell information
- Player status (position, orientation, level)
- Available resources
- Events (incantations, hatchings, etc.)

## 3D Visualization

The interface uses RaylibCPP for 3D rendering with the following features:
- Rendering of the map as a 3D grid
- 3D models for players and resources
- Free camera or tracking modes
- Visual effects for special events

## User Interface

The user interface uses RayGUICPP and offers:
- Information on selected players
- Real-time game statistics
- Visualization configuration
- Camera options

## Compilation and Execution

```bash
# Compilation
make -C src/GUI

# Execution
./zappy_gui -p <port> -h <host>
```

## Important Files
- `main.cpp` : Program entry point
- `GameLoop.cpp` : Main game loop
- `network/networkManager/NetworkManager.cpp` : Network communication management
- `renderer/Renderer.cpp` : Graphic rendering
- `cameraController/CameraController.cpp` : Camera control
