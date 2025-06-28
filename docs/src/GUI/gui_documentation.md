# Zappy Graphical User Interface (GUI) Documentation

## Overview

The Zappy graphical interface is responsible for 3D visualization of the game environment, allowing real-time observation of:
- The terrain and its resources with visual indicators
- Players and their actions with animated 3D models
- Game events (incantations, egg hatching, battles, etc.)
- Team statistics and temporal information
- Visual effects and specialized animations

## Advanced Architecture

The graphical interface is developed in C++ and uses a modular architecture based on dynamic library loading (DLLoader):

### Main Components
- **GameLoop**: Main game loop with event handling
- **NetworkManager**: Bidirectional communication with the server
- **Renderer**: 3D rendering pipeline with visual effects support
- **MapRenderer**: Specialized map rendering with levels of detail (LOD)
- **CameraController**: Advanced camera management with smooth transitions
- **UserInterface**: Modular user interface system
- **GameController**: Component coordination and state management

### Dynamic Loading System (DLLoader)

The graphical interface uses a dynamic loading system allowing the use of different graphics implementations:

**Main interfaces:**
- **IGraphicsLib**: Defines expected functions for graphics libraries
- **IGuiLib**: Defines expected functions for user interface libraries
- **LibraryManager**: Enables dynamic library loading

**Advantages:**
- Easy switching between graphics implementations
- Modularity without modifying main code
- Support for custom graphics plugins

### User Interface Architecture

The user interface system is based on several design patterns:

**Factory Pattern (`UIWindowFactory`):**
- Centralized creation of specialized windows
- UI component lifecycle management
- Easy extension for new window types

**Class hierarchy:**
```
IUIWindow (Interface)
    |
    +-- AUIWindow (Abstract class)
            |
            +-- TileInfoWindow      (Tile information)
            +-- PlayerInfoWindow    (Player information)
            +-- TimeInfoWindow      (Temporal information)
            +-- MenuWindow          (Main menu)
            +-- MapInfoWindow       (Map information)
```

## Communication with the server

The graphical interface communicates with the server via a specialized protocol:

### GUI Commands
- **Identification**: `GRAPHIC\n` to identify as a graphical client
- **Initial information**: Automatic reception of complete game state
- **Temporal control**: `sst T\n` to modify server frequency
- **Specific requests**: `pin #\n`, `ppo #\n`, `plv #\n` for player information

### Server notifications
- **Map**: `msz X Y\n`, `bct X Y q0 q1 q2 q3 q4 q5 q6\n`
- **Players**: `pnw # X Y O L N\n`, `ppo # X Y O\n`, `plv # L\n`
- **Events**: `pic X Y L #1 #2 ...\n`, `pie X Y R\n`, `pex #\n`
- **Teams**: `tna N\n`, `seg N\n`

## Advanced 3D Visualization

The interface uses RaylibCPP for sophisticated 3D rendering:

### Rendering features
- **3D Map**: Map rendering as 3D grid with automatic adjustment
- **3D Models**: Players and resources with custom models (.glb)
- **Camera System**: Free camera with intuitive controls and tracking modes
- **Visual Effects**: Particles, animations and special effects for events
- **Lighting**: Dynamic lighting system with skybox
- **Optimizations**: Level of Detail (LOD) and frustum culling for large maps

### Visual Effects System
- **ParticleSystem**: Particle manager for special effects
- **EjectionAnimationManager**: Player ejection animations
- **DeathAnimationManager**: Death animations with visual effects
- **BroadcastEffect**: Visual effects for broadcast messages

## Complete User Interface

The user interface uses RayGUICPP and offers:

### Specialized windows
- **TileInfoWindow**: Detailed information on selected tiles
- **PlayerInfoWindow**: Complete player statistics with commands
- **TimeInfoWindow**: Temporal information and real-time FPS
- **MenuWindow**: Audio, visual, and game configuration
- **MapInfoWindow**: General information on map and teams

### Interactive features
- **Selection**: Click on tiles and players to display information
- **Camera controls**: Intuitive rotation, zoom, and movement
- **Real-time configuration**: Parameter modification without restart
- **Player tracking**: Automatic tracking of specific players

## Resource Management

### TextureManager and ModelManager
- **Optimized cache**: Loading and caching of textures and models
- **Supported formats**: Support for .glb, .jpg, .png formats
- **Memory management**: Automatic release of unused resources

### Game assets
- **3D Models**: Collection of models for players and objects
- **Textures**: Skybox and terrain textures
- **Audio**: Background music and sound effects

## Compilation and Execution

### Build
```bash
# Complete compilation
make -C src/GUI

# Special modes
make -C src/GUI debug      # Debug mode with symbols
make -C src/GUI coverage   # Code coverage
make -C src/GUI clean      # Cleanup
```

### Dependencies
- **Raylib**: 3D graphics library
- **OpenGL**: Accelerated graphics rendering
- **System libraries**: X11, pthread, dl, m

### Execution
```bash
./zappy_gui -p <port> -h <host>
```

**Interface controls:**
- **Camera**: Left click + drag for rotation, mouse wheel for zoom
- **Movement**: WASD keys for camera movement
- **Selection**: Click on elements to display information
- **Interface**: Resizable and repositionable windows

## Important Files

### Main structure
- `main.cpp`: Entry point with argument parsing
- `GameLoop.cpp`: Main game loop and coordination
- `GameLoopUI.cpp`: Game loop user interface

### Network and communication
- `network/networkManager/NetworkManager.cpp`: Server communication management
- `network/protocol/ProtocolParser.cpp`: GUI protocol parser
- `network/buffer/CircularBuffer.cpp`: Communication buffers

### Rendering and graphics
- `renderer/Renderer.cpp`: Main rendering engine
- `renderer/MapRenderer.cpp`: Specialized map rendering
- `renderer/Skybox.cpp`: Skybox management
- `renderer/ParticleSystem.cpp`: Particle system

### User interface
- `ui/UserInterface.cpp`: Main user interface
- `ui/UIWindowFactory.cpp`: Window factory
- `ui/windows/*/`: Specialized windows

### Controls and management
- `cameraController/CameraController.cpp`: Advanced camera control
- `gameController/GameController.cpp`: Component coordination
- `textureManager/ModelManager.cpp`: 3D model management
