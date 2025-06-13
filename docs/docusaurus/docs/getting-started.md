---
sidebar_position: 1
---

# Getting Started with Zappy

Welcome to Zappy, a sophisticated multi-agent virtual ecosystem. This guide will help you build, configure, and run all components of the system.

## What is Zappy?

Zappy is a high-performance multiplayer simulation featuring:

- **Game Server**: High-performance C engine managing world state and client connections
- **3D Graphical Interface**: Advanced C++ visualization with real-time rendering
- **AI Clients**: Intelligent Python agents with behavioral algorithms

## System Requirements

### Development Environment
- **OS**: Linux (Ubuntu 20.04+ recommended), macOS, or WSL2
- **Compiler**: GCC 9.0+ or Clang 10.0+
- **Python**: 3.8 or higher
- **Build Tools**: Make 4.0+, CMake 3.16+
- **Version Control**: Git 2.25+

### Runtime Dependencies
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake git python3-dev python3-pip
sudo apt install libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxinerama-dev
sudo apt install libxcursor-dev libxi-dev libxext-dev libwayland-dev libxkbcommon-dev

# macOS (with Homebrew)
brew install cmake python3 gcc

# Python Dependencies
pip3 install asyncio cryptography pytest
```

## Quick Installation

### 1. Clone and Build
```bash
# Clone the repository
git clone https://github.com/Epitech/Zappy.git
cd Zappy

# Initialize submodules
git submodule update --init --recursive

# Build all components
make

# Verify installation
./zappy_server --help
./zappy_gui --help
python3 src/AI/main.py --help
```

### 2. Run a Complete Game Session

**Terminal 1 - Start the Server:**
```bash
./zappy_server -p 4242 -x 20 -y 20 -n TeamAlpha TeamBeta -c 5 -f 100
```

**Terminal 2 - Launch 3D Interface:**
```bash
./zappy_gui -p 4242 -h localhost
```

**Terminal 3 - Deploy AI Agents:**
```bash
# Launch first AI agent
python3 src/AI/main.py -p 4242 -n TeamAlpha -h localhost &

# Launch second AI agent  
python3 src/AI/main.py -p 4242 -n TeamAlpha -h localhost &

# Launch competing team
python3 src/AI/main.py -p 4242 -n TeamBeta -h localhost &
```

## Server Configuration

### Command Line Parameters

| Parameter | Description | Default | Example |
|-----------|-------------|---------|---------|
| `-p <port>` | Server listening port | 4242 | `-p 8080` |
| `-x <width>` | Map width (tiles) | 20 | `-x 30` |
| `-y <height>` | Map height (tiles) | 20 | `-y 25` |
| `-n <teams>` | Team names (space-separated) | - | `-n Alpha Beta Gamma` |
| `-c <clients>` | Max clients per team | 10 | `-c 3` |
| `-f <freq>` | Server frequency (ticks/second) | 100 | `-f 200` |

### Advanced Options

| Option | Description | Default |
|--------|-------------|---------|
| `--auto-start on\|off` | Automatic game start | off |
| `--display-eggs true\|false` | Show eggs in GUI | true |
| `--game-duration <seconds>` | Game time limit | unlimited |
| `-v, --verbose` | Enable detailed logging | disabled |

### Example Configurations

**Development Setup:**
```bash
./zappy_server -p 4242 -x 10 -y 10 -n Dev Test -c 2 -f 50 --verbose
```

**Competition Setup:**
```bash
./zappy_server -p 4242 -x 50 -y 50 -n TeamA TeamB TeamC TeamD -c 6 -f 100 --game-duration 1800
```

## GUI Interface

### Launch Parameters
```bash
./zappy_gui -p <port> -h <hostname>
```

### Controls
- **Mouse**: Camera rotation and zoom
- **WASD**: Camera movement
- **Space**: Follow selected player
- **Tab**: Cycle through players
- **F1**: Toggle help overlay
- **Esc**: Exit application

### Interface Elements
- **Map View**: 3D isometric world visualization
- **Team Panel**: Real-time team statistics
- **Player Info**: Selected player details
- **Resource Monitor**: Map resource distribution
- **Event Log**: Game action history

## AI Client Configuration

### Basic Usage
```bash
python3 src/AI/main.py -p <port> -n <team> -h <host>
```

### Advanced Options
```bash
python3 src/AI/main.py \
  -p 4242 \
  -n TeamAlpha \
  -h localhost \
  --behavior dyson \
  --log-level debug \
  --encryption-key mySecretKey
```

### Available Behaviors
- **dyson**: Optimized resource collection
- **incanter**: Evolution-focused strategy  
- **explorer**: Map exploration specialist
- **hybrid**: Adaptive multi-strategy

## Development Workflow

### Component-Specific Building
```bash
# Server only
make -C src/Server

# GUI only  
make -C src/GUI

# AI dependencies
cd src/AI && pip3 install -r requirements.txt

# Documentation
make -C docs/docusaurus && npm run build
```

### Testing and Validation
```bash
# Run unit tests
make tests_run

# Performance testing
make benchmark

# Network stress testing
make stress_test

# Code quality analysis
make lint
```

## Troubleshooting

### Common Issues

**Port Already in Use:**
```bash
# Find process using port
lsof -i :4242
# Kill process if needed
kill -9 <PID>
```

**OpenGL/Graphics Issues:**
```bash
# Check OpenGL support
glxinfo | grep "OpenGL version"
# Install missing drivers
sudo apt install mesa-utils
```

**Python Dependencies:**
```bash
# Create virtual environment
python3 -m venv zappy_env
source zappy_env/bin/activate
pip install -r src/AI/requirements.txt
```

**Build Failures:**
```bash
# Clean and rebuild
make fclean && make
# Check compiler version
gcc --version
```

### Performance Optimization

**Server Performance:**
- Increase frequency for faster simulation: `-f 200`
- Reduce map size for lower latency: `-x 15 -y 15`
- Limit client connections: `-c 3`

**GUI Performance:**
- Enable VSync in graphics settings
- Reduce rendering distance for older hardware
- Use windowed mode instead of fullscreen

## Next Steps

Now that you have Zappy running:

1. **[Explore the Architecture](./architecture)** - Understand system design
2. **[Server Development](./server/)** - Extend game mechanics  
3. **[GUI Customization](./gui/)** - Enhance visualization
4. **[AI Programming](./ai/)** - Create intelligent agents
5. **[Protocol Reference](./protocols/)** - Network communication details

Ready to build your first AI strategy? Check out the [AI Documentation](./ai/) to get started!
