# Zappy Project Documentation

## Introduction

Welcome to the Zappy project documentation. This project is a multi-agent virtual ecosystem composed of several components interacting together to create a complex and efficient simulation environment.

Zappy simulates a virtual world where agents controlled by artificial intelligence evolve, collect resources, communicate with each other, and try to reach the maximum level through collaborative incantations.

## System Overview

### Main Components

- **[Server](src/Server/README.md)** - The core of the system managing game logic, written in C for optimal performance
- **[Graphical Interface](src/GUI/README.md)** - Real-time 3D visualization developed in C++ with Raylib
- **[Artificial Intelligence](src/AI/README.md)** - Autonomous agents developed in Python with adaptive behaviors
- **[Shared Libraries](src/Shared/README.md)** - Dynamic loading system and common interfaces

### Technical Architecture

The project uses a client-server architecture with TCP/IP communication:

```
┌─────────────┐    TCP/IP     ┌──────────────┐    TCP/IP     ┌──────────────┐
│   Clients   │◄─────────────►│   Server    │◄─────────────►│  Interface   │
│     IA      │   Commands     │    Zappy     │  Game data   │  Graphical   │
│  (Python)   │   Responses    │     (C)      │  Events      │   (C++)      │
└─────────────┘               └──────────────┘               └──────────────┘
```

## Documentation Structure

### User Guides
- **[Getting Started Guide](docusaurus/docs/getting-started.md)** - Installation, compilation, and first steps
- **[Architecture](docusaurus/docs/architecture.md)** - Technical overview of the system
- **[Development Guide](docusaurus/docs/development-guide.md)** - Code standards, testing, and contribution

### Technical Documentation

#### Main Components
- **[Server Documentation](src/Server/README.md)** - Architecture, API, and server implementation
- **[AI Documentation](src/AI/README.md)** - Intelligent agents, behaviors, and strategies
- **[GUI Documentation](src/GUI/README.md)** - 3D graphical interface and rendering system
- **[Shared Documentation](src/Shared/README.md)** - Dynamic library loading system

#### Libraries and Tools
- **[RaylibCPP](libs/RaylibCPP/README.md)** - Modern C++ wrapper for Raylib
- **[RayGUICPP](libs/RayGUICPP/README.md)** - User interface with RayGUI
- **[raygui History](libs/RayGUICPP/raygui_history.md)** - Evolution of the raygui library

### Communication Protocols
- **[Server-AI Protocol](docs/protocol_server_ai.md)** - Communication between server and AI agents
- **[Server-GUI Protocol](docusaurus/docs/protocols/server-gui.md)** - Communication for graphical display

### Advanced Documentation
- **[Design Patterns](docs/design_patterns_map_renderer.md)** - Patterns used in the rendering engine
- **[Rendering Implementation](docs/map_renderer_implementation.md)** - Technical details of the rendering system

## Interactive Documentation

### Docusaurus (Web Documentation)
- **Location** : `/docs/docusaurus/`
- **Start** : `cd docs/docusaurus && npm start`
- **Local URL** : http://localhost:3000

The Docusaurus documentation offers a modern browsing experience with:
- Responsive interface and integrated search
- Multi-language support (French/English)
- Structured navigation by components
- Code examples with syntax highlighting

### Doxygen (API Documentation)
- **Configuration** : `/docs/Doxygen/Doxyfile`
- **Generation** : `cd docs/Doxygen && doxygen`
- **Output** : `/docs/Doxygen/build/html/`

The Doxygen documentation automatically generates:
- Complete documentation of C/C++ APIs
- Class and dependency diagrams
- Index of functions and structures
- Documentation extracted from code comments

## Quick Start

### Installing Dependencies

```bash
# System (Ubuntu/Debian)
sudo apt update
sudo apt install build-essential cmake python3-dev python3-pip
sudo apt install libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev

# Documentation
sudo apt install doxygen graphviz nodejs npm

# Python (AI)
cd src/AI && pip install -r requirements.txt

# Docusaurus
cd docs/docusaurus && npm install
```

### Compilation and Testing

```bash
# Full compilation
make

# Test components
./zappy_server --help
./zappy_gui --help
python3 src/AI/main.py --help

# Unit tests
make tests_run

# Generate documentation
cd docs/Doxygen && doxygen
cd docs/docusaurus && npm run build
```

## Contributing to the Documentation

### Documentation Standards

1. **Markdown** : Standard format for all documents
2. **Doxygen Comments** : In C/C++ code for automatic generation
3. **Python Docstrings** : Google/Numpy format for Python modules
4. **Examples** : Functional code in all guides

### Contribution Workflow

1. **Branching** : Create a branch `docs/feature-name`
2. **Editing** : Modify/add documentation
3. **Verification** : Test generation (Doxygen, Docusaurus)
4. **Pull Request** : Submit for review

### File Organization

```
docs/
├── index.md                    # This homepage
├── protocol_server_ai.md       # AI communication protocol
├── design_patterns_*.md        # Design patterns
├── docusaurus/                 # Interactive web documentation
│   ├── docs/                   # Documentation pages
│   ├── src/                    # Components and themes
│   └── sidebars.ts            # Navigation
├── Doxygen/                    # API configuration and output
│   ├── Doxyfile               # Doxygen configuration
│   └── build/                 # Generated documentation
└── assets/                     # Images and resources
```

## Additional Resources

### External References
- **[Raylib](https://www.raylib.com/)** - Graphics library used
- **[Docusaurus](https://docusaurus.io/)** - Documentation generator
- **[Doxygen](https://www.doxygen.nl/)** - API documentation generator

### Support and Community
- **Issues** : Bug reporting and feature requests
- **Discussions** : General questions and community help
- **Wiki** : Collaborative documentation and advanced tutorials

### License and Copyright
This project is developed by the Epitech team as part of the B-YEP-400 educational project. 
See the `LICENSE` file for license details.
