# 🎮 Zappy

<div align="center">
  <strong>Multiplayer AI-based network game</strong><br>
  Control autonomous agents, collect resources, evolve, and conquer a world governed by TCP and time units.
</div>

<div align="center">
  <img src="https://img.shields.io/badge/language-C%2FC%2B%2B%2FPython-blue" alt="Languages">
  <img src="https://img.shields.io/badge/protocol-TCP%2FIP-orange" alt="Protocol">
  <img src="https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey" alt="Platform">
  <img src="https://img.shields.io/badge/version-1.0.0-green" alt="Version">
</div>

<div align="center">
  <img src="https://img.shields.io/github/license/SamTess/Zappy" alt="License">
  <img src="https://img.shields.io/github/actions/workflow/status/SamTess/Zappy/zappyCICD.yml" alt="CI Status">
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

## 🔧 Prerequisites

To compile and run Zappy, you will need:

- C/C++ compiler (GCC or Clang)
- Python 3.x
- Make
- Graphic development libraries (for the GUI client)

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
./zappy_server -p <port> -x <width> -y <height> -n <team names> -c <max clients> -f <freq> [options]
```

**Required parameters:**
- `-p <port>` : Server listening port
- `-x <width>` : Map width (minimum 10)
- `-y <height>` : Map height (minimum 10) 
- `-n <team1> [team2] ...` : Team names (separated by spaces)
- `-c <max_clients>` : Maximum number of clients per team
- `-f <freq>` : Server frequency (time units per second)

**Optional parameters:**
- `--auto-start on|off` : Automatic game start (default: off)
- `--display-eggs true|false` : Egg visibility (default: true)
- `--game_duration <time>` : Game duration in seconds
- `-v` or `--verbose` : Verbose mode for debugging

**Example:**
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 team3 -c 5 -f 100 --auto-start on --display-eggs true
```

### Start the graphical interface

```bash
./zappy_gui -p <port> -h <host>
```

**Parameters:**
- `-p <port>` : Server port
- `-h <host>` : Server address (default: localhost)

**Example:**
```bash
./zappy_gui -p 4242 -h localhost
```

**Interface controls:**
- **Camera** : Left click + drag to rotate, mouse wheel to zoom
- **View** : WASD keys to move the camera
- **Interface** : Real-time information panels on teams and resources

### Start the AI

```bash
./zappy_ai -p <port> -n <team name> [-h <host>]
```

**Parameters:**
- `-p <port>` : Server port
- `-n <team>` : Team name
- `-h <host>` : Server address (optional, default: localhost)

**Example:**
```bash
./zappy_ai -p 4242 -n team1
```

**AI behaviors:**
- **Resource collection** : Automatic exploration and optimized collection
- **Inter-agent communication** : Coordination via encrypted broadcast
- **Evolution** : Automatic performance of incantations to level up
- **Adaptive strategy** : Behavior change according to context

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
```

## 📦 Components

### Server

The server is the central component that manages :
- The game map and resources
- Client connections (AI and GUI)
- Game logic and rules
- Time and event management
- Communication protocol
- Elevation rituals and player evolution

### Graphical Interface

The graphical interface offers :
- Real-time visualization of the game map
- Display of players and their inventories
- Visualization of resources on each tile
- Event tracking (incantations, reproductions, etc.)
- Interactive controls for navigation and observation

### Artificial Intelligence

The AI of the clients includes :
- Perception modules for environment analysis
- Strategic decision-making algorithms
- Resource management and planning
- Communication and coordination between agents
- Strategies for elevation rituals

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

