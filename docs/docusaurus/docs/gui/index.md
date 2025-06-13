---
sidebar_position: 1
---

# Graphical Interface (GUI)

The graphical interface allows real-time visualization of the Zappy game state, players, resources, and actions.

## Features

- Display of the game map in 2D or 3D
- Visualization of players and their orientation
- Display of resources present on each tile
- Visualization of player actions (movement, item pickup, etc.)
- Interface to monitor team status
- Visualization of incantations
- Controls to adjust simulation speed

## Configuration

The GUI client accepts the following parameters:

| Parameter | Description                | Default value |
|-----------|----------------------------|---------------|
| `-p`      | Server port number         | 4242          |
| `-h`      | Server address             | localhost     |

## Communication Protocol

The GUI client connects to the server and receives information about the game state. It uses a text protocol over TCP. The GUI client cannot send commands that affect the game; it is only an observer.

## User Interface

The user interface includes several sections:

1. **Map View** : Displays the game grid with players and resources
2. **Information Panel** : Shows details about the selected item
3. **Viewing Controls** : Allow zooming, rotating, and moving the view
4. **Team List** : Displays teams and their statistics
5. **Timeline** : Allows tracking of event history

## Architecture

The GUI is developed in C++ and uses a modular architecture based on dynamic library loading (DLLoader):

- **Core** : Main loop, event management, coordinator
- **NetworkManager** : Communication with the server
- **Renderer** : 3D and 2D graphic display
- **CameraController** : Camera management and views
- **GUI Components** : User interface elements

## Dynamic Loading System (DLLoader)

The GUI uses a dynamic library loading system to allow the use of different graphic implementations:
- `IGraphicsLib` Interface : Defines the expected functions for graphic libraries
- `IGuiLib` Interface : Defines the expected functions for user interface libraries
- `LibraryManager` Manager : Allows dynamic loading of libraries

## 3D Visualization

The interface uses RaylibCPP for 3D rendering with the following features:
- Rendering the map as a 3D grid
- 3D models for players and resources
- Free camera or tracking modes
- Visual effects for special events

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
