---
sidebar_position: 1
---

# Zappy Server

The server is the core component of the Zappy project, responsible for managing game state, enforcing rules, simulating the world, and coordinating communication between AI clients and the graphical interface.

## Architecture Overview

The Zappy server is built with a modular, high-performance architecture:

- **Network Manager**: Handles client connections, message parsing, and async I/O
- **Game Engine**: Implements core game logic, rules, and state transitions
- **Map System**: Manages the game world, resource distribution, and spatial queries
- **Player Manager**: Tracks player states, teams, and action processing
- **Command Processor**: Parses and executes client commands with proper timing
- **Resource System**: Controls resource generation, distribution, and consumption

## Server Configuration

The server accepts the following command-line parameters for flexible deployment:

| Parameter | Description                               | Default | Range/Format |
|-----------|-------------------------------------------|---------|--------------|
| `-p`      | Port number                               | 4242    | 1024-65535   |
| `-x`      | Map width                                 | 20      | 10-100       |
| `-y`      | Map height                                | 20      | 10-100       |
| `-n`      | Team names (space-separated)              | *Required* | String identifiers |
| `-c`      | Maximum clients per team                  | 10      | 1-100        |
| `-f`      | Execution frequency (ticks per second)    | 100     | 2-10000      |
| `-t`      | Initial food regeneration interval (ticks)| 20      | 1-1000       |
| `-r`      | Initial resource regeneration rate        | 20      | 1-100        |
| `-d`      | Debug mode (verbose logging)              | Off     | Flag         |

## Internal Architecture

The server employs several key architectural patterns:

### Event-Driven Architecture

The server uses an event-driven model with asynchronous I/O to handle multiple clients efficiently:

```c
// Core event loop
while (server->running) {
    // Monitor file descriptors for activity
    int activity = poll(server->poll_manager->pollfds, server->poll_manager->nfds, -1);
    
    if (activity < 0) {
        // Handle polling error
        continue;
    }
    
    // Check for new client connections
    if (FD_ISSET(server_fd, &readfds)) {
        handle_new_connection(server);
    }
    
    // Process client events
    for (int i = 0; i < server->nfds; i++) {
        if (server->poll_manager->pollfds[i].revents & POLLIN) {
            process_client_input(server, &server->clients[i]);
        }
        if (server->poll_manager->pollfds[i].revents & POLLOUT) {
            send_client_responses(server, &server->clients[i]);
        }
    }
    
    // Update game state
    update_game_state(server);
}
```

### Time Management

The server uses a sophisticated time management system to ensure consistent gameplay:

```c
typedef struct time_manager_s {
    int frequency;              // Ticks per second
    double tick_duration_ms;    // Milliseconds per tick
    uint64_t current_tick;      // Current game time
    uint64_t last_update;       // Timestamp of last update
    struct timeval last_time;   // System time of last update
} time_manager_t;
```

### Command Processing Pipeline

Commands from clients are processed through a multi-stage pipeline:

1. **Reception**: Network layer receives raw text commands
2. **Parsing**: Commands are parsed into structured data
3. **Validation**: Command syntax and parameters are validated
4. **Scheduling**: Valid commands are scheduled for execution
5. **Execution**: Commands are executed in the game world
6. **Response**: Results are sent back to clients

## Resource Management

The server implements sophisticated algorithms for resource distribution:

### Distribution Algorithm

Resources are distributed using a parameterized algorithm that ensures:
- Even distribution across the map
- Controlled rarity for higher-tier resources
- Regeneration based on server time and consumption rates

```c
void regenerate_resources(map_t *map, resource_config_t *config) {
    // Calculate number of resources to add based on map size and config
    int food_to_add = (map->width * map->height * config->food_density) / 100;
    
    // Distribute resources with probability weighting
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            tile_t *tile = &map->tiles[y][x];
            
            // Probability decreases with resource tier
            if (rand() % 100 < config->food_probability) {
                tile->food += 1;
            }
            if (rand() % 100 < config->linemate_probability) {
                tile->linemate += 1;
            }
            // ... other resources with decreasing probabilities
        }
    }
}
```

## Player Lifecycle Management

The server tracks player lifecycle from connection to death:

1. **Connection**: TCP connection established
2. **Authentication**: Team selection and validation
3. **Initialization**: Player state creation and map placement
4. **Active State**: Command processing and state updates
5. **Death**: Food depletion or disconnection handling
6. **Resource Recovery**: Inventory dropped on tile

## Team Management

Teams have specific properties and limitations:

```c
typedef struct team_s {
    char *name;                  // Team identifier
    int max_clients;             // Maximum players allowed
    int connected_clients;       // Current player count
    client_t **clients;          // Array of client pointers
    int eggs_count;              // Available egg count
    egg_t **eggs;                // Array of egg pointers
} team_t;
```

## Implementation Details

### Memory Management

The server implements careful memory management to prevent leaks:

```c
// Resource allocation
void* zappy_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        server_error("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

// Resource cleanup
void cleanup_server(server_t *server) {
    // Free all dynamically allocated resources
    cleanup_clients(server->clients, server->nfds);
    cleanup_map(server->map);
    cleanup_teams(server->teams, server->team_count);
    
    // Close socket connections
    for (int i = 0; i < server->nfds; i++) {
        close(server->poll_manager->pollfds[i].fd);
    }
    
    free(server->poll_manager);
    free(server);
}
```

### Concurrency Control

The server uses a single-threaded event loop with non-blocking I/O to avoid race conditions while maintaining high performance.

### Error Handling

Comprehensive error handling ensures server stability:

```c
void handle_server_error(server_t *server, const char *message, int error_code) {
    // Log the error
    log_error("[SERVER] %s (Error %d: %s)", message, error_code, strerror(error_code));
    
    // Attempt recovery based on error type
    switch (error_code) {
        case ECONNRESET:
            // Client disconnected unexpectedly
            handle_client_disconnect(server, find_client_by_error(server));
            break;
        case EAGAIN:
            // Resource temporarily unavailable, retry later
            break;
        // ... other error cases
        default:
            // Unrecoverable error
            if (error_code > ERROR_THRESHOLD) {
                log_critical("Fatal error, shutting down server");
                server->running = false;
            }
    }
}
```

## Performance Optimization

The server includes optimizations for handling large maps and many clients:

1. **Spatial Partitioning**: Grid-based spatial indexing for efficient queries
2. **Command Batching**: Processing commands in batches for efficiency
3. **Response Buffering**: Buffering outgoing messages to reduce system calls
4. **Memory Pooling**: Reusing memory allocations for common operations
5. **Efficient Data Structures**: Custom data structures for game state

## File Structure

The server code is organized into logical modules:

```
src/Server/
  ├── main.c               # Entry point
  ├── include/             # Header files
  │   ├── server.h         # Main server definitions
  │   ├── map.h            # Map system definitions
  │   └── ...
  ├── network/             # Network handling
  │   ├── socket.c         # Socket operations
  │   └── client.c         # Client handling
  ├── map/                 # Map management
  │   ├── map.c            # Map operations
  │   └── resource.c       # Resource management
  ├── player/              # Player management
  │   ├── player.c         # Player operations
  │   └── team.c           # Team operations
  ├── command/             # Command processing
  │   ├── command.c        # Command handler
  │   └── ...              # Individual command implementations
  └── util/                # Utilities
      ├── logger.c         # Logging system
      └── memory.c         # Memory management
```

---

For more detailed information about specific server components and protocols, refer to the server components documentation and protocol specifications.
