# Zappy Server Documentation

## Overview

The Zappy server is the central component of the project, managing game logic, communication between clients (AI), and the graphical interface. It is responsible for:
- Managing the map and resources
- Managing players and teams
- Processing AI commands
- Communicating with the graphical interface

## Architecture

The server is structured into several modules:
- **Network**: Connection management, message sending/receiving
- **Map**: 2D map and resource management
- **Player**: Player states, inventories, and actions management
- **Team**: Team management and available slots
- **Commands**: Client request processing

## Communication Protocol

The server uses a simple text protocol to communicate with clients and the graphical interface. Commands are sent as strings ending with `\n`.

### AI Commands
- `Forward`: Move forward one square
- `Right`: Turn right
- `Left`: Turn left
- `Look`: Observe visible squares
- `Inventory`: Check inventory
- `Broadcast text`: Send a message
- `Connect_nbr`: Number of available slots in the team
- `Fork`: Create a new egg
- `Eject`: Eject players from a square
- `Take object`: Pick up an object
- `Set object`: Place an object
- `Incantation`: Start an incantation

### Graphical Protocol
The server communicates with the graphical interface via a specific protocol detailed in the "GUI Protocol" documentation.

## Compilation and Execution

```bash
# Compilation
make -C src/Server

# Execution
./zappy_server -p <port> -x <width> -y <height> -n <team_names> -c <clients_per_team> -f <freq>
```

## Important Files
- `main.c` : Program entry point
- `parsing.c` : Command line argument parsing
- `network/*.c` : Network connection management
- `map/*.c` : Map and resource management
- `player/*.c` : Player and command management
