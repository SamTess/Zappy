# 🎮 Zappy

<div align="center">
  <strong>Multiplayer AI-based network game</strong><br>
  Control autonomous agents, collect resources, evolve, and conquer a world governed by TCP and time units.
</div>

<div align="center">
  <img src="https://img.shields.io/badge/langage-C%2FC%2B%2FPython-blue" alt="Languages">
  <img src="https://img.shields.io/badge/protocole-TCP%2FIP-orange" alt="Protocol">
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
- [Documentation](#-documentation)
- [Development](#-development)

---

## 🌐 Overview

Zappy is a multi-component project developed in C/C++ and Python, reproducing a virtual ecosystem where agents controlled by AI evolve by collecting resources, communicating, and performing elevation rituals. The game consists of three main elements :

- **Server** : Written in C, it manages the game logic, rules, and world state
- **Graphical Interface** : Developed in C++, it visualizes the game state in real-time
- **AI Clients** : Implemented in Python, they automatically control players to achieve strategic objectives

## 🏗 Architecture

The components communicate via TCP/IP network protocols :

```
┌────────┐    Commands/Responses    ┌──────────┐
│        │◄────────────────────────►│          │
│ Client │                          │          │
│  (AI)  │                          │          │
│        │                          │          │
└────────┘                          │          │
                                    │ Server  │
┌────────┐    Game data            │          │
│        │◄────────────────────────►│          │
│  GUI   │                          │          │
│        │                          │          │
└────────┘                          └──────────┘
```

## 🔧 Prerequisites

To compile and run Zappy, you will need :

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
./zappy_server -p <port> -x <width> -y <height> -n <team names> -c <max clients> -f <freq>
```

Example :
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 5 -f 100
```

### Start the graphical interface

```bash
./zappy_gui -p <port> -h <host>
```

Example :
```bash
./zappy_gui -p 4242 -h localhost
```

### Start the AI

```bash
./zappy_ai -p <port> -n <team name>
```

Example :
```bash
./zappy_ai -p 4242 -n team1
```

## 📦 Components

### Server

The server is the central component that manages :
- The game map and resources
- Client connections (AI and GUI)
- The game logic and rules
- Time and event management
- Communication protocol
- Elevation rituals and player evolution

### Graphical Interface

The graphical interface provides :
- Real-time visualization of the game map
- Display of players and their inventories
- Visualization of resources on each tile
- Event tracking (spells, reproductions, etc.)
- Interactive controls for navigation and observation

### Artificial Intelligence

The AI of the clients includes :
- Perception modules for environment analysis
- Strategic decision-making algorithms
- Resource management and planning
- Communication and coordination between agents
- Strategies for elevation rituals

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

