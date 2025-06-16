# Zappy AI Documentation

## Overview

Zappy AI is implemented in Python and represents the virtual players evolving in the game environment. Each AI is independent and makes decisions based on:
- Its current state (level, inventory)
- Its perception of the environment
- The objectives to achieve to level up

## Architecture

The AI code is structured into several modules:
- **Agent**: Manages the AI's decision logic
- **Parser**: Analyzes messages received from the server
- **Logger**: Logs actions and events
- **Utils**: Shared utility functions
- **Defs**: Game definitions and constants

## AI Logic

The AI follows a hierarchical state machine allowing it to:
1. Explore the map to find resources
2. Collect the resources needed for incantation
3. Search for other team players for incantation
4. Perform incantations to level up
5. Coordinate its actions with other players via broadcast

## States and Transitions

The agent has several main states:
- **Exploration**: Searching for food and resources
- **Collection**: Picking up identified resources
- **Regrouping**: Searching for other players for incantation
- **Incantation**: Executing the level-up ritual
- **SOS**: Emergency state when food is low

## Communication

Agents communicate with each other via the server's broadcast mechanism using a simple protocol:
- Prefix: Team identifier
- Message type: Action, Request, Information
- Content: Depends on the message type

## Usage

```bash
# Run
./zappy_ai -p <port> -n <team_name> -h <host>
```

## Important Files
- `main.py`: Entry point of the program
- `agent/agent.py`: Main class of the AI agent
- `defs/zappy.py`: Definitions of game constants
- `utils/*.py`: Utility functions
- `logger/logger.py`: Logging system
