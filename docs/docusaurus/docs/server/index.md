---
sidebar_position: 1
---

# Zappy Server

The server is the central component of the Zappy project. It manages the game state, logic, rules, and coordinates communication between AI clients and the graphical interface.

## Server Architecture

The Zappy server is designed with a modular architecture:

- **Network Manager**: Manages client connections and communication
- **Game Engine**: Implements game rules and logic
- **Resource Manager**: Manages the game map and resources
- **Team Manager**: Manages teams and players

## Server Configuration

The server accepts the following command-line parameters:

| Parameter | Description                                  | Default value |
|-----------|----------------------------------------------|---------------|
| `-p`      | Port number                                  | 4242          |
| `-x`      | Map width                                    | 20            |
| `-y`      | Map height                                   | 20            |
| `-n`      | Team names (separated by spaces)             | -             |
| `-c`      | Maximum number of clients per team           | 10            |
| `-f`      | Execution frequency (ticks per second)       | 100           |

## Communication Protocol

The server communicates with clients using a text-based protocol over TCP. Commands and responses are sent as newline-terminated text strings.

### AI Client Commands

The server accepts the following commands from AI clients:

- `Forward`: Move forward one space
- `Right`: Turn right 90°
- `Left`: Turn left 90°
- `Look`: Observe the environment
- `Inventory`: Show inventory
- `Broadcast text`: Broadcast a message to all players
- `Connect_nbr`: Get the number of available spots in the team
- `Fork`: Create a new egg (new player)
- `Eject`: Eject players on the same tile
- `Take object`: Take an object
- `Set object`: Place an object
- `Incantation`: Start an incantation to level up

### GUI Protocol

The server sends updates to the GUI client to allow real-time visualization of the game state.

## Implementation

The server is implemented in C and uses POSIX sockets for network communication and an event-driven programming model to handle multiple clients simultaneously.

## File Structure

The server code is organized as follows:
- `main.c`: Program entry point
- `parsing.c`: Command-line argument parsing
- `network/*.c`: Network connection management
- `map/*.c`: Map and resource management
- `player/*.c`: Player and command management

## Compilation and Execution

```bash
# Compilation
make -C src/Server

# Execution
./zappy_server -p <port> -x <width> -y <height> -n <team_names> -c <clients_per_team> -f <freq>
```

## Important Mechanisms

### Resource Management
The server randomly generates resources on the map and renews them at regular intervals based on the configured frequency.

### Time Management
All actions in the game are measured in time units. The server uses a queue system to manage actions that must be executed after a certain delay.

### Leveling Up
The game includes a leveling system based on incantation rituals that require:
- A specific number of players
- Precise combinations of resources
- All players to be at the same level
