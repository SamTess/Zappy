# Zappy Server Documentation

## Overview

The Zappy server is the central component of the project, developed in C for optimal performance. It manages the game state, multiple client connections (AI), and the graphical interface. It is responsible for:
- Map and resource management with automatic distribution
- Player, team, and inventory management
- AI command processing with queue system
- Communication with graphical interface via dedicated protocol
- Game rule enforcement and time management
- Elevation rituals and spawning mechanisms

## Modular Architecture

The server is structured into several specialized modules:

### Network Manager (`network/`)
- **TCP/IP Connections**: Multi-connection management with non-blocking polling
- **Client Types**: Distinction between AI (`AI`) and graphical (`GRAPHICAL`) clients
- **Circular Buffers**: Optimized communication system to avoid blocking
- **Queue Manager**: Polling with `poll()` for efficient I/O management
- **Disconnection Detection**: Robust handling of disconnecting clients

### Game Engine (`map/`, `player/`)
- **Toric Map**: 2D map system with automatic wraparound
- **Resource Distribution**: Automatic generation and regeneration of resources
- **Egg Management**: Spawning and hatching system for new players
- **Player Inventories**: Complete management of objects and food
- **Player States**: Tracking positions, orientations, levels, and actions

### Command Processor (`command/`)
- **Queue System**: FIFO system for pending commands
- **Time Cost**: Each command has a cost in server time units
- **Validation**: Prerequisite verification before execution
- **AI Commands**: `Forward`, `Right`, `Left`, `Look`, `Inventory`, `Broadcast`, etc.
- **Special Commands**: `Incantation`, `Fork`, `Eject`, `Take`, `Set`

### Time Manager
- **Configurable Frequency**: `-f` parameter to define ticks per second
- **Synchronization**: Tick system to coordinate all actions
- **Starvation Management**: Automatic food consumption (1 unit/126 ticks)
- **Timed Events**: Delayed command execution according to their cost

## Communication Protocols

### AI Clients
The server accepts AI connections that authenticate with their team name and receive game commands.

**Connection Sequence:**
1. Client TCP/IP connection
2. Server responds: `WELCOME\n`
3. Client sends: `<team_name>\n`
4. Server responds: `<client_num>\n<X> <Y>\n`

**Main Commands:**
- **Movement**: `Forward` (7 ticks), `Right` (7 ticks), `Left` (7 ticks)
- **Perception**: `Look` (7 ticks), `Inventory` (1 tick)
- **Interaction**: `Take <object>` (7 ticks), `Set <object>` (7 ticks)
- **Communication**: `Broadcast <text>` (7 ticks)
- **Evolution**: `Incantation` (300 ticks), `Fork` (42 ticks)
- **Combat**: `Eject` (7 ticks)

### Graphical Clients
Graphical clients identify with `GRAPHIC` and receive all game events in real-time.

**Automatic Notifications:**
- **Map**: `msz X Y\n`, `bct X Y q0 q1 q2 q3 q4 q5 q6\n`
- **Players**: `pnw # X Y O L N\n`, `ppo # X Y O\n`, `plv # L\n`
- **Inventories**: `pin # X Y q0 q1 q2 q3 q4 q5 q6\n`
- **Events**: `pic X Y L #1 #2 ...\n`, `pie X Y R\n`, `pex #\n`
- **Teams**: `tna N\n`, `seg N\n`

## Main Data Structures

### Player (`player_t`)
```c
typedef struct player_s {
    int client_id;              // Unique identifier
    int pos_x, pos_y;          // Position on map
    int orientation;           // Direction (0-3: N, E, S, W)
    int level;                 // Current level (1-8)
    int inventory[7];          // [food, linemate, deraumere, sibur, mendiane, phiras, thystame]
    char *team_name;           // Team name
    pending_cmd_t *pending_cmd; // Command being executed
    char **command_queue;      // Command queue
    int life_time;             // Remaining life time
    int is_dead;               // Death state
} player_t;
```

### Map Tile (`tile_t`)
```c
typedef struct tile_s {
    int resources[7];          // Resources present on tile
    int *players;              // List of present players
    int nb_players;            // Number of players on tile
    int max_players;           // Maximum capacity
} tile_t;
```

### Egg (`egg_t`)
```c
typedef struct egg_s {
    int id;                    // Unique egg identifier
    int pos_x, pos_y;         // Spawning position
    char *team_name;          // Associated team
    struct egg_s *next;       // Linked list
} egg_t;
```

## Incantation Ritual Management

Incantation rituals are complex mechanisms requiring:

### Elevation Conditions
For each level, specific prerequisites:
- **Level 1→2**: 1 player, 1 linemate
- **Level 2→3**: 1 player, 1 linemate, 1 deraumere, 1 sibur
- **Level 3→4**: 2 players, 2 linemate, 1 sibur, 1 phiras
- **Level 4→5**: 2 players, 1 linemate, 1 deraumere, 2 sibur, 1 phiras
- **Level 5→6**: 4 players, 1 linemate, 2 deraumere, 1 sibur, 1 mendiane
- **Level 6→7**: 4 players, 1 linemate, 2 deraumere, 3 sibur, 1 phiras
- **Level 7→8**: 6 players, 2 linemate, 2 deraumere, 2 sibur, 2 mendiane, 2 phiras, 1 thystame

### Incantation Process
1. **Verification**: Presence of required resources and players
2. **Startup**: Notification `pic X Y L #1 #2 ...` to graphical clients
3. **Duration**: 300 ticks of execution time
4. **Finalization**: Level up and notification `pie X Y R`

## Performance and Optimizations

### Supported Metrics
The server can handle:
- **100+ simultaneous clients** on modern hardware
- **1000+ ticks/second** for fast-paced games
- **Latency < 1ms** for local commands
- **Memory < 100MB** for standard-sized maps

### Implemented Optimizations
- **Non-blocking polling** with `poll()` for network I/O
- **Command queues** for asynchronous processing
- **Optimized memory management** with object pools
- **Map caching** to avoid frequent recalculations
- **Circular buffers** for efficient communication

## Compilation and Execution

### Build
```bash
cd src/Server
make                    # Standard build
make debug             # Build with debug symbols
make coverage          # Build with coverage instrumentation
make clean             # Clean object files
```

### Launch Parameters
```bash
./zappy_server -p <port> -x <width> -y <height> -n <teams> -c <clients> -f <freq>
```

**Advanced Options:**
- `--auto-start on|off` : Automatic startup
- `--display-eggs true|false` : Egg display
- `-v, --verbose` : Verbose mode
- `--game_duration <time>` : Maximum game duration

## Important Files

### Source Structure
- `main.c` : Server entry point with signal handling
- `zappy.c` : Server initialization and configuration
- `parsing.c` : Command line argument parsing
- `network/server_run.c` : Main loop and client management
- `network/connection.c` : TCP/IP connection management
- `map/map_creation.c` : Map creation and initialization
- `player/player_init.c` : Player initialization and management
- `command/parse_command.c` : Command parsing and execution
- `buffer/circular_buffer.c` : Circular buffer system
