# 🎮 Zappy

<div align="center">
  <strong>Multiplayer AI-based network game</strong><br>
  Control autonomous agents, collect resources, evolve, and conquer a world governed by TCP and time units.
</div>

<div align="center">
  <img src="https://img.shields.io/badge/language-C%2FC%2B%2B%2FPython-blue" alt="Languages">
  <img src="https://img.shields.io/badge/protocol-TCP%2FIP-orange" alt="Protocol">
  <img src="https://img.shields.io/badge/platform-Linux-lightgrey" alt="Platform">
  <img src="https://img.shields.io/badge/version-4.0.0-green" alt="Version">
</div>

<div align="center">
  <img src="https://img.shields.io/github/license/SamTess/Zappy" alt="License">
  <img src="https://img.shields.io/github/actions/workflow/status/SamTess/Zappy/zappyCICD.yaml" alt="CI Status">
  <img src="https://img.shields.io/github/issues/SamTess/Zappy" alt="Issues">
  <img src="https://img.shields.io/github/forks/SamTess/Zappy" alt="Forks">
  <img src="https://img.shields.io/github/stars/SamTess/Zappy" alt="Stars">
</div>

<div align="center">
  <img src="https://img.shields.io/github/last-commit/SamTess/Zappy" alt="Last Commit">
  <img src="https://img.shields.io/github/contributors/SamTess/Zappy" alt="Contributors">
  <img src="https://img.shields.io/github/release/SamTess/Zappy" alt="Release">
  <img src="https://img.shields.io/github/downloads/SamTess/Zappy/total" alt="Downloads">
</div>

---

## 📑 Table of Contents

- [Overview](#-overview)
- [Architecture](#-architecture)
- [User Interface](#-user-interface)
- [Prerequisites](#-prerequisites)
- [Installation](#-installation)
- [Usage](#-usage)
- [Components](#-components)
  - [Server](#server)
  - [Artificial Intelligence](#artificial-intelligence)
- [Releases](#-releases)
- [Documentation](#-documentation)
- [Development](#-development)

---

## 🌐 Overview

Zappy is a multi-component project developed in C/C++ and Python, reproducing a virtual ecosystem where AI-controlled agents evolve by collecting resources, communicating, and performing elevation rituals. The game consists of three main elements :

- **Server** : Written in C, it manages the game logic, rules, and world state
- **Graphical Interface** : Developed in C++, it visualizes the game state in real-time
- **AI Clients** : Implemented in Python, they automatically control players to achieve strategic objectives

## 🏗 Architecture

The components communicate via TCP/IP network protocols:

```
┌────────┐    Commands/Responses    ┌──────────┐
│        │◄────────────────────────►│          │
│ Client │                          │          │
│  (AI)  │                          │          │
│        │                          │          │
└────────┘                          │          │
                                    │  Server  │
┌────────┐    Game data             │          │
│        │◄────────────────────────►│          │
│  GUI   │                          │          │
│        │                          │          │
└────────┘                          └──────────┘
```

## 💎 User Interface

The GUI user interface has been designed following advanced object-oriented design principles:

- **Factory Pattern**: Use of `UIWindowFactory` for window creation and management
- **Class Hierarchy**: Organization around the `IUIWindow` interface and `AUIWindow` abstract class
- **Modularity**: Each window type is encapsulated in its own specialized class

Available window types:
- Logs (`LogsWindow`) - displays system messages and events
- Tile Information (`TileInfoWindow`) - details on selected tiles
- Player Information (`PlayerInfoWindow`) - player statistics
- Broadcasts (`BroadcastsWindow`) - messages between players
- Controls (`ControlsWindow`) - camera and game control options
- Time Information (`TimeInfoWindow`) - time and frequency statistics
- Menu (`MenuWindow`) - global application settings

Detailed documentation available in [docs/ui_architecture.md](docs/ui_architecture.md).

## 🔧 Prerequisites

To compile and run Zappy, you will need:

### System Requirements
- **C/C++ Compiler** (GCC 9.0+ or Clang 10.0+)
- **Python 3.8+** with pip3
- **Make 4.0+**
- **Git** (for submodules and Raylib)
- **CMake** (for compiling Raylib from sources if necessary)

### Automatic dependency installation
```bash
# Automatic installation of all dependencies
make install_requirements
```

This command automatically installs:
- Raylib (via APT or compilation from GitHub)
- Python dependencies for AI (cryptography, dotenv)
- Git submodule initialization

### System dependencies for Raylib
The project requires the following libraries for the graphical interface:
- **OpenGL**: `libgl1-mesa-dev`
- **X11**: `libx11-dev`, `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev`, `libxi-dev`, `libxext-dev`
- **Build tools**: `build-essential`, `cmake`, `pkg-config`

```bash
# Manual installation of system dependencies (Ubuntu/Debian)
sudo apt update
sudo apt install -y build-essential cmake git pkg-config
sudo apt install -y libgl1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev
sudo apt install -y libxcursor-dev libxi-dev libxext-dev
```

### Python dependencies for AI
The `src/AI/requirements.txt` file contains:
- **cryptography**: AES encryption for secure communications between agents
- **dotenv**: Environment variable management

```bash
# Manual installation (if make install_requirements fails)
cd src/AI && pip3 install -r requirements.txt
```

## 💻 Installation

```bash
# Clone the repository
git clone https://github.com/Epitech/Zappy.git
cd Zappy

# Compile all components
make

# Or compile specific components
make zappy_server
make zappy_gui
make zappy_ai
```

## 🚀 Usage

### Start the server

```bash
./zappy_server -p <port> -x <width> -y <height> -n <team names> -c <max clients> -f <freq>
```

**Required parameters:**
- `-p <port>`: Server listening port
- `-x <width>`: Map width (minimum 10)
- `-y <height>`: Map height (minimum 10)
- `-n <team1> [team2] ...`: Team names (separated by spaces)
- `-c <max_clients>`: Maximum number of clients per team
- `-f <freq>`: Server frequency (time units per second)

**Optional parameters:**
- `--auto-start on|off`: Automatic start (default: off)
- `--display-eggs true|false`: Display eggs in logs (default: false)
- `-v` or `--verbose`: Verbose mode for more logs
- `--game_duration <time>`: Maximum game duration in seconds

**Example:**
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 team3 -c 5 -f 100 --auto-start on
```

### Start the graphical interface

```bash
./zappy_gui -p <port> -h <host>
```

**Parameters:**
- `-p <port>`: Server port
- `-h <host>`: Server address (default: localhost)

**Example:**
```bash
./zappy_gui -p 4242 -h localhost
```

**Interface controls:**
- **Camera**: Left click + drag to rotate, mouse wheel to zoom
- **View**: WASD keys to move the camera
- **Interface**: Real-time information panels on teams and resources
- **Selection**: Click on a tile or player to display details
- **Menu**: Access to audio, visual, and game settings

### Start the AI

#### Prerequisites
Before starting the AI, make sure to activate the Python virtual environment:
```bash
source venv/bin/activate
```

#### Start command

```bash
./zappy_ai -p <port> -n <team name> [-h <host>] [--performance]
```

**Parameters:**
- `-p <port>`: Server port
- `-n <team>`: Team name
- `-h <host>`: Server address (optional, default: localhost)
- `--performance`: Performance mode for simulations (optional)

**Example:**
```bash
./zappy_ai -p 4242 -n team1 --performance
```

**AI behaviors:**
- **Automatic collection**: Optimized exploration and resource collection
- **Inter-agent communication**: Coordination via encrypted broadcast
- **Evolution**: Automatic execution of incantations to level up
- **Adaptive strategy**: Behavior change according to context
- **Intelligent management**: Route optimization and priority management

#### Multi-agents
The system automatically launches multiple agents for the specified team. The number of agents is configurable in the code (default: 10 agents per team).

## 📦 Components

### Server

The Zappy server is the core of the system, developed in C for optimal performance.

**Main features:**
- Game map and resource management
- Real-time processing of client commands
- Physical and temporal simulation of the world
- TCP/IP communication protocols
- Multi-threaded management for performance

**Modular architecture:**
- `network/` : Connection and protocol management
- `map/` : Map and tiles system
- `player/` : Player management and their states
- `command/` : Command processing
- `buffer/` : Buffer system for communication

### Graphical Interface

3D interface developed in C++ using Raylib for real-time visualization.

**Features:**
- 3D rendering of the map and entities
- Intuitive user interface with RayGUI
- Free camera system with smooth controls
- Real-time display of statistics
- Texture and 3D model management

**Components:**
- `renderer/` : 3D rendering engine
- `cameraController/` : Camera management
- `textureManager/` : Loading and management of textures
- `network/` : Communication with the server
- `graphicalContext/` : Graphic context and state

### Artificial Intelligence

Autonomous bots developed in Python with a modular and scalable architecture.

**Characteristics:**
- Adaptive behavioral architecture
- Secure communication system between agents
- Exploration and optimization algorithms
- Evolution and cooperation strategies
- Advanced logging for debugging and analysis
./zappy_ai -p 4242 -n team1

## 📦 Components

### Server

The Zappy server is developed in C for optimal performance and manages:

**Main features:**
- Game map and resource management with automatic distribution
- Multiple connections (AI clients and graphical interface) via TCP/IP
- Game logic and application of virtual universe rules
- Time manager with configurable tick system
- Distinct textual communication protocols for AI and GUI
- Elevation rituals with condition verification and player coordination
- Egg laying system and new agent generation

**Modular architecture:**
- `network/`: TCP/IP connection management and communication
- `map/`: Map system with tiles and resources
- `player/`: Player management, inventories and actions
- `command/`: Command processor with queue
- `buffer/`: Circular buffer system for communication
- `graphical_command/`: Specialized commands for graphical interface

### Graphical Interface

The graphical interface is developed in C++ with Raylib and offers:

**Advanced 3D visualization:**
- Real-time rendering of the game map with level of detail (LOD) system
- Player display with animated 3D models and orientations
- Resource visualization on each tile with visual indicators
- Free camera system with smooth controls and transitions
- Visual effects for special events (incantations, ejections, deaths)
- Skybox and 3D lighting for increased immersion

**Modular user interface:**
- Window system based on Factory pattern (`UIWindowFactory`)
- Specialized information panels (players, tiles, teams, time)
- Interactive controls for navigation and observation
- Real-time event logging system
- Configuration menu with audio and visual settings
- Extensible architecture allowing easy addition of new windows

**Dynamic loading system:**
- DLLoader for graphics and interface libraries
- Plugin support with `IGraphicsLib` and `IGuiLib` interfaces
- Texture and 3D model management with optimized cache

### Artificial Intelligence

The AI for clients developed in Python includes:

**Adaptive behavioral architecture:**
- Hierarchical state machine with intelligent transitions
- Perception modules for environment analysis (`Look`, `Inventory`)
- Decision-making algorithms based on objective priority
- Resource management system with route optimization
- Multi-agent coordination via AES encrypted communication

**Strategies and behaviors:**
- Intelligent exploration with internal universe mapping
- Optimized resource collection with conflict avoidance
- Grouping strategies for elevation rituals
- Emergency behaviors in case of famine or danger
- Dynamic role system (miner, fighter, breeder)

**Communication and collaboration:**
- Secure broadcast protocol between agents of the same team
- Information exchange on resource positions
- Coordination for multi-player incantation rituals
- Message encryption system to avoid espionage

**Advanced features:**
- A* pathfinding algorithm optimized for toric maps
- Internal world representation with real-time updates
- Intelligent time and action priority management
- Complete logging system for analysis and debugging
- Performance mode for large-scale simulations

## 🚀 Releases

The project uses an automated release system via GitHub Actions with semantic versioning :

### 📦 Download a release
Go to the [releases page](../../releases) to download the latest version.

### 🔄 Create a new release
Releases are automatically created via GitHub Actions :

1. **Access Actions** : GitHub → Actions tab → "🚀 Release Zappy" workflow
2. **Choose the version type** :
   - `nouvelle-version` : Major version (v1.0.0 → v2.0.0)
   - `nouvelle-feature` : New feature (v1.0.0 → v1.1.0)  
   - `resolution-bug` : Bug fix (v1.0.0 → v1.0.1)
3. **Run the workflow** : The system automatically generates the version, compiles the project, and creates the release

Each release contains :
- All binaries (server, GUI, AI)
- Static and dynamic libraries
- Automatic installation script
- Documentation and changelog

📖 **Detailed guide** : [docs/release-workflow.md](docs/release-workflow.md)

## 📚 Documentation

Detailed documentation is available in the `docs/` folder :
- Architecture and design : `docs/architecture.md`
- Getting started guide : `docs/getting-started.md`
- Server documentation : `docs/server/`
- Graphical interface documentation : `docs/gui/`
- AI documentation : `docs/ai/`

The Docusaurus documentation can be viewed locally :
```bash
cd docs/docusaurus
yarn start
```

## 🛠 Development

### Project structure

```
├── src/
│   ├── Server/    # Server implementation in C
│   ├── GUI/       # Graphical interface in C++
│   ├── AI/        # AI client in Python
│   └── Shared/    # Code shared between components
├── libs/          # External libraries (RaylibCPP, RayGUICPP)
├── docs/          # Project documentation
├── tests/         # Unit and functional tests
├── assets/        # Graphic resources
└── tools/         # Development tools
```

### Tests

```bash
# Run all tests
make tests_run

# Test the GUI network
make functionnal_tests

# Compile in debug mode
make debug
```

---

<div align="center">
  <p>Developed with ❤️ by the Zappy team</p>
  <p>© 2025 Epitech</p>
</div>

