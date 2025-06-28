# Zappy AI Documentation

## Overview

The Zappy artificial intelligence is implemented in Python and represents autonomous virtual players that evolve in the game environment. Each AI is independent and makes decisions based on:
- Its current state (level, inventory, position)
- Its perception of the environment via `Look` and `Inventory` commands
- Objectives to achieve for level advancement
- Communications with other team agents
- Analysis of the tactical situation (resources, enemies, allies)

## Detailed Architecture

The AI code is structured into several specialized modules:

### Main Modules
- **Agent** (`agent/agent.py`): Main class managing the AI's decision logic
- **DecisionManager** (`agent/decisionManager.py`): Strategic decision manager
- **BroadcastManager** (`agent/broadcastManager.py`): Inter-agent communication manager
- **SocketManager** (`agent/socketManager.py`): Server communication manager
- **Logger** (`logger/logger.py`): Advanced logging system with agent prefixes
- **Utils** (`utils/`): Shared utility functions (encryption, communication, pathfinding)

### Constants and Definitions
- **Upgrades** (`constants/upgrades.py`): Definitions of resources required for each level
- **Parser** (`parser/`): Command-line argument parser
- **Multiprocessing** (`utils/multiprocessing.py`): Management of multiple agent launches

## Advanced Behavioral Logic

The AI follows a hierarchical state machine that allows it to:

### Main States
1. **BigDyson**: Main exploration and collection behavior
2. **Collecting**: Targeted collection of specific resources
3. **Rallying**: Gathering for incantation rituals
4. **Setting**: Placement and preparation of incantations
5. **Upgrading**: Execution of elevation rituals
6. **Reproducing**: Reproduction and creation of new eggs
7. **Emergency**: Emergency state in case of critical famine

### Dynamic Roles
- **Miner**: Specialized in resource collection
- **Fighter**: Combat-oriented and enemy ejection
- **Coordinator**: Communication management and team coordination

### Tactical Phases
- **Start**: Initialization and reconnaissance phase
- **Collecting**: Active resource collection
- **Rallying**: Gathering with allies
- **Setting**: Positioning for collective actions
- **Upgrading**: Ritual execution
- **Reproducing**: Team expansion

## Communication and Coordination

### Secure Broadcast System
Agents communicate via the server's broadcast mechanism using:
- **AES Encryption**: Secure protocol with team name-based key
- **Message Types**: Actions, requests, information, alerts
- **Structured Format**: Team prefixes and typed content
- **Direction**: Automatic calculation of sender direction

### Information Sharing
- Positions of discovered resources
- Alerts about enemy presence
- Coordination for incantation rituals
- Sharing of inventories and needs
- Synchronization of roles and phases

## Algorithms and Optimizations

### A* Pathfinding
- Optimized algorithm for toric maps
- Obstacle and dangerous zone avoidance
- Calculation of optimal paths to resources

### Intelligent Resource Management
- Resource prioritization according to current level
- Conflict avoidance with other agents
- Collection route optimization

### Tactical Analysis
- Threat detection and evaluation
- Evolution opportunity calculation
- Strategy adaptation according to context

## Usage and Configuration

### Standard Launch
```bash
./zappy_ai -p <port> -n <team_name> -h <host>
```

### Performance Mode
```bash
./zappy_ai -p <port> -n <team_name> --performance
```

Performance mode disables certain logs and optimizes performance for large-scale simulations.

### Multi-agents
The system automatically launches 10 agents by default for each team, each with:
- A unique ID within the team
- A slightly differentiated strategy
- Coordination via encrypted broadcast

## Important Files

### File Structure
- `main.py`: Program entry point with argument handling
- `agent/agent.py`: Main AI agent class with all behaviors
- `agent/decisionManager.py`: Advanced decision-making logic
- `agent/broadcastManager.py`: Inter-agent communication management
- `agent/socketManager.py`: Interface with Zappy server
- `utils/encryption.py`: AES encryption system for communications
- `utils/zappy.py`: Interface with server commands
- `utils/multiprocessing.py`: Multi-agent launch management
- `constants/upgrades.py`: Evolution prerequisite definitions
- `logger/logger.py`: Advanced logging system

### Configuration
- `requirements.txt`: Python dependencies (cryptography, dotenv)
- `Makefile`: Automated compilation and installation
