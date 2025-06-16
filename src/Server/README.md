# Zappy Server

## Overview

The Zappy server is the central component of the project, developed in C for optimal performance. It manages the game state, multiple client connections, and applies all the rules of the virtual world simulation.

## Architecture

### Main Components

- **Network Manager** (`network/`) - TCP/IP communication with AI and GUI clients
- **Game Engine** (`map/`, `player/`) - World state and simulation rules
- **Command Processor** (`command/`) - Interpretation and execution of actions
- **Time Manager** - Synchronization and game tick management

### Key Data Structures

```c
typedef struct server_s {
    int s_fd;                           // Server socket
    parsing_info_t *config;            // Configuration
} server_t;
```

## Compilation

```bash
cd src/Server
make                    # Standard build
make debug             # Build with debug symbols
make clean             # Clean up object files
```

## Usage

### Standard Startup

```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 5 -f 100
```

### Parameters

- `-p <port>` : Listening port (required)
- `-x <width>` : Map width (required, min: 10)
- `-y <height>` : Map height (required, min: 10)
- `-n <teams>` : Team names separated by spaces (required)
- `-c <clients>` : Max number of clients per team (required)
- `-f <freq>` : Server frequency in ticks/second (required)
- `--auto-start on|off` : Auto start (optional)
- `--display-eggs true|false` : Egg visibility (optional)
- `--game_duration <sec>` : Game duration in seconds (optional)
- `-v, --verbose` : Verbose mode (optional)

### Advanced Examples

```bash
# Quick test game
./zappy_server -p 4242 -x 10 -y 10 -n test -c 2 -f 1000 --auto-start on

# Balanced multi-team game
./zappy_server -p 4242 -x 30 -y 30 -n alpha beta gamma -c 6 -f 100 \
               --game_duration 3600 --display-eggs true -v

# Tournament setup
./zappy_server -p 4242 -x 50 -y 50 -n team1 team2 team3 team4 -c 10 -f 100 \
               --auto-start off --game_duration 7200
```

## Communication Protocols

### AI Clients

The server accepts AI connections that authenticate with their team name and receive game commands. See [Server-AI Protocol](../../docs/protocol_server_ai.md) for details.

### GUI Clients

Graphical clients identify themselves with `GRAPHIC` and receive all game events in real-time. See [Server-GUI Protocol](../../docs/docusaurus/docs/protocols/server-gui.md).

## Performance and Optimization

### Metrics

The server can handle:
- **100+ simultaneous clients** on modern hardware
- **1000+ ticks/second** for fast games
- **Latency < 1ms** for local commands
- **Memory < 100MB** for standard size maps

### Implemented Optimizations

- **Non-blocking polling** with `poll()` for network I/O
- **Command queues** for asynchronous processing
- **Optimized memory management** with object pools
- **Map caching** to avoid recalculations

## Testing

### Unit Tests

```bash
make tests_run          # All tests
make test_player        # Player module tests
make test_map          # Map module tests
make test_commands     # Command tests
```

### Performance Tests

```bash
# Load test with simulated clients
./tests/stress_test.sh 100 clients

# Memory test
valgrind --leak-check=full ./zappy_server -p 4242 -x 10 -y 10 -n test -c 1 -f 100

# CPU profiling
perf record ./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 5 -f 100
perf report
```

## Debugging

### Debug Mode

```bash
# Build with symbols
make debug

# Debugging with GDB
gdb ./zappy_server
(gdb) run -p 4242 -x 10 -y 10 -n team1 -c 1 -f 100
(gdb) break main
(gdb) continue
```

### Verbose Logs

```bash
# Enable verbose mode
./zappy_server -v -p 4242 -x 10 -y 10 -n team1 -c 1 -f 100

# Redirect to file
./zappy_server -v -p 4242 -x 10 -y 10 -n team1 -c 1 -f 100 > server.log 2>&1
```

## Internal Architecture

### Main Loop

```c
while (server->should_run) {
    // 1. Polling client sockets
    poll(server->poll_manager->fds, server->nfds, timeout);
    
    // 2. Processing new connections
    check_client(server);
    
    // 3. Reading client commands
    process_client_messages(server);
    
    // 4. Updating game tick
    update_game_tick(server);
    
    // 5. Processing pending commands
    process_command_queues(server);
    
    // 6. Sending updates to GUIs
    send_gui_updates(server);
}
```

### Command Handling

```c
typedef struct command_s {
    char *name;                // Command name
    int (*execute)(server_t *, client_t *, char **); // Execution function
    int time_cost;            // Cost in ticks
    bool gui_only;            // Command reserved for GUIs
} command_t;
```

### Player States

Players progress through different states:
1. **Connected** - Authenticated but not yet spawned
2. **Active** - In game, can execute commands
3. **Incantation** - In the process of elevation (immobile)
4. **Dead** - No more food, connection closed

## Advanced Configuration

### Environment Variables

```bash
export ZAPPY_MAX_CLIENTS=200      # Global client limit
export ZAPPY_TICK_PRECISION=1000  # Tick precision (µs)
export ZAPPY_LOG_LEVEL=INFO       # Log level (DEBUG/INFO/WARN/ERROR)
export ZAPPY_BUFFER_SIZE=8192     # Network buffer size
```

### Configuration File

The server can read an optional configuration file:

```ini
# zappy.conf
[server]
max_clients = 200
tick_precision = 1000
log_level = INFO

[game]
default_food_generation = 50
default_resource_density = 0.15
incantation_duration = 300

[network]
buffer_size = 8192
timeout = 5000
keep_alive = true
```

## Monitoring and Maintenance

### System Signals

- **SIGINT/SIGTERM** : Graceful server shutdown
- **SIGUSR1** : Reload configuration
- **SIGUSR2** : Dump internal state

### Runtime Metrics

```bash
# Send signal for statistics
kill -USR2 <pid_serveur>

# Output in logs :
# [INFO] Runtime Statistics:
# - Connected clients: 25
# - Commands/sec: 150
# - Memory usage: 45MB
# - Uptime: 02:34:17
```

## Contributing

### Code Standards

- **Format** : K&R style, 4 spaces indentation
- **Naming** : `snake_case` for functions and variables
- **Documentation** : Mandatory Doxygen comments
- **Tests** : > 80% coverage for new features

### Workflow

1. Fork the repository
2. Feature branch : `git checkout -b feature/new-feature`
3. Development with tests
4. Pull Request to `develop`

## Full Documentation

- **[Server Architecture](../../docs/docusaurus/docs/server/server-overview.md)** - Detailed architecture
- **[AI Protocol](../../docs/protocol_server_ai.md)** - Communication with AIs
- **[Doxygen API](../../docs/Doxygen/build/html/)** - Generated code documentation
