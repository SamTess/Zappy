---
sidebar_position: 1
---

# Graphical Interface (GUI)

The Zappy Graphical Interface is a sophisticated 3D visualization system that provides real-time observation of the game state, including map, players, resources, and game events.

## Key Features

- **Advanced 3D Rendering**: High-performance visualization using Raylib/OpenGL
- **Multiple View Modes**: Overhead, isometric, and player-following camera perspectives
- **Resource Visualization**: Detailed representation of game resources with texture and model support
- **Player Tracking**: Real-time visualization of player positions, orientations, and actions
- **Game Event Visualization**: Visual effects for incantations, egg laying, and other game events
- **Server Connectivity**: Robust network communication with the Zappy server
- **Simulation Controls**: Interfaces to adjust simulation speed and parameters
- **Dynamic Library Loading**: Modular architecture for graphics and UI components

## Technical Specifications

### Performance Metrics

- **Rendering Performance**: 60+ FPS on standard hardware
- **Map Size Support**: Optimized for maps up to 100×100 tiles
- **Connection Stability**: Automatic reconnection and state recovery
- **Memory Efficiency**: Dynamic resource management to minimize footprint

### Configuration Options

| Parameter | Description                     | Default | Range/Options |
|-----------|--------------------------------|---------|---------------|
| `-p`      | Server port number             | 4242    | 1024-65535    |
| `-h`      | Server address                 | localhost | Any valid hostname/IP |
| `--fullscreen` | Launch in fullscreen mode | false   | true/false    |
| `--width` | Window width (pixels)          | 1280    | 800-3840      |
| `--height`| Window height (pixels)         | 720     | 600-2160      |
| `--msaa`  | Multisample anti-aliasing level| 4       | 0/2/4/8       |
| `--vsync` | Vertical synchronization       | true    | true/false    |

## Core Architecture

The GUI is built using a modular, component-based architecture:

- **Game Loop**: Core update and render cycle
- **Network Manager**: Server communication and data protocol handling
- **Renderer**: 3D and 2D rendering pipeline with multiple strategies
- **Camera Controller**: Advanced camera systems with transitions
- **Resource Manager**: Texture and model loading/caching
- **UI System**: Interactive interface components
- **Event System**: Game event processing and visualization
- **Dynamic Library System**: Plugin-based graphics and UI modules

## User Interface Components

The GUI provides a comprehensive set of interface elements:

1. **Main View**: 3D visualization of the game world
2. **Information Panel**: Detailed data on selected game elements
3. **Team Display**: Team statistics and player information
4. **Resource Counter**: Global and per-tile resource statistics
5. **Game Controls**: Simulation speed and camera controls
6. **Event Log**: Real-time event notification system
7. **Player Tracker**: Focus and track specific players

## Rendering Technologies

The GUI leverages multiple rendering techniques:

- **Dynamic Level of Detail (LOD)**: Adjusts rendering fidelity based on camera distance
- **Frustum Culling**: Only renders objects within camera view
- **Ambient Occlusion**: Enhanced depth perception for 3D elements
- **Normal Mapping**: Advanced texture techniques for better visual quality
- **Particle Systems**: Visual effects for game events like incantations
- **Shader-based Rendering**: Custom visual effects for game elements

## Communication Protocol

The GUI connects to the server using a specialized protocol:

- Initial connection with the `GRAPHIC` command
- Receives comprehensive map and entity data
- Processes real-time updates for game state changes
- Can request specific information about tiles, players, and resources
- Optional time-step control for server simulation speed

## Dynamic Library System (DLLoader)

The GUI implements a sophisticated dynamic library loading system:

- **IGraphicsLib Interface**: Abstracts rendering functionality
- **IGuiLib Interface**: Provides UI component rendering
- **LibraryManager**: Handles runtime loading and unloading of modules

This system allows for:
- Swapping graphics implementations without recompilation
- Custom UI themes and components
- Enhanced extensibility for future development
- Plugin-based architecture for new features

## 3D Visualization Strategies

The renderer employs multiple strategies for tile visualization:

1. **Simple Strategy**: Flat colored tiles for performance
2. **Detailed Strategy**: Enhanced tiles with resource indicators
3. **Model Strategy**: Full 3D models for immersive visualization

---

For implementation details and technical specifications, refer to the GUI architecture documentation and source code in `src/GUI/`.
