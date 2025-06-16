---
sidebar_position: 1
---

# AI Client

The AI client is an autonomous player that connects to the Zappy server and makes decisions to play the game independently, optimizing resource collection, team coordination, and progression through levels.

## Features

- Full TCP/IP connectivity with the Zappy server
- Environment analysis and world state tracking
- Decision-making based on game state and objectives
- Advanced resource management system
- Team coordination through encrypted broadcast messages
- Multi-state behavior with adaptive transitions
- Pathfinding with A* algorithm optimized for toroidal maps

## Core Architecture

Each AI client follows an agent-based architecture comprising:

1. **Perception Module**: Processes and interprets server information
   - Look command interpretation
   - Inventory tracking
   - Broadcast message decoding

2. **Decision Module**: Determines optimal actions
   - State machine with hierarchical behaviors
   - Priority-based action selection
   - Resource allocation algorithms

3. **Action Module**: Executes selected commands
   - Command scheduling and queuing
   - Action failure handling
   - Action cost optimization

4. **State Manager**: Maintains internal representation of game world
   - Tile-based world map model
   - Resource availability tracking
   - Team member position tracking

## AI Strategies

The AI implements several sophisticated strategies:

- **Resource Collection**: Optimized algorithms for finding and gathering needed resources
- **Level Progression**: Calculations to determine the most efficient path to level up
- **Exploration**: Map discovery with minimal redundancy
- **Team Coordination**: Synchronized actions with other team members

## States and Transitions

The agent utilizes multiple primary states:

- **Exploration**: Searching for food and resources
- **Collection**: Gathering identified resources
- **Assembly**: Finding other players for incantation
- **Incantation**: Performing level-up ritual
- **Emergency**: High-priority state when food is low

## Team Communication

Agents communicate via the server's broadcast mechanism using a secure protocol:

- **Prefix**: Team identifier with encryption
- **Message Type**: Action, Request, Information, Coordination
- **Content**: Type-specific data payload
- **Encryption**: AES-based team-specific encryption

## Technical Implementation

The AI client is implemented in Python with these key technical features:

- **Asynchronous Operation**: Uses Python's asyncio for non-blocking communication
- **Cryptography**: Implements AES encryption for team communication
- **Memory-Efficient Design**: Optimized data structures for world representation
- **Logging System**: Comprehensive debug and performance tracking
- **Error Recovery**: Robust error handling and reconnection logic

## Key Files

- `main.py`: Program entry point and argument parsing
- `agent/agent.py`: Primary agent implementation
- `agent/state_machine.py`: State management system
- `agent/behaviors.py`: Behavior implementations
- `utils/pathfinder.py`: A* pathfinding algorithm
- `utils/world_map.py`: Internal world representation
- `utils/communication.py`: Team broadcast utilities
- `utils/zappy.py`: Server communication interface
- `logger/logger.py`: Logging system

---

For detailed implementation specifics, refer to the source code in `src/AI/` and the API documentation.
