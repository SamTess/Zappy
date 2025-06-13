---
sidebar_position: 1
---

# Server Architecture and Implementation

## Overview

The Zappy server is the central component of the system, developed in C for optimal performance. It manages the game state, client connections, and enforces all game rules. This document provides a technical exploration of the server's implementation.

## Core Architecture

### Main Server Structure

The server's core structure maintains the essential state and systems:

```c
typedef struct server_s {
    int nfds;                           // Number of file descriptors
    int server_fd;                      // Main server socket
    struct sockaddr_in address;         // Server address
    
    // Management systems
    poll_manager_t *poll_manager;       // Polling system for network I/O
    time_manager_t *time_manager;       // Game time management
    
    // Game state
    map_t *map;                         // Game world grid
    team_manager_t *team_manager;       // Team information
    client_manager_t *client_manager;   // Client connections
    command_queue_t *command_queue;     // Pending commands
    
    // Configuration
    server_config_t config;             // Server parameters
    bool running;                       // Server status flag
} server_t;
```

## Client Management

### Client Types and States

The server distinguishes between two client types:

```c
enum client_type_e {
    GRAPHICAL,                         // GUI observer client
    AI                                 // Player AI client
};

enum client_state_e {
    CONNECTED,                         // Initial connection state
    AUTHENTICATED,                     // Valid team selected
    ACTIVE,                            // Fully active in game
    DISCONNECTING                      // In process of disconnection
};

typedef struct client_s {
    int socket_fd;                     // Client socket descriptor
    enum client_type_e type;           // Client type
    enum client_state_e state;         // Current state
    
    // Communication buffers
    char read_buffer[BUFFER_SIZE];     // Input buffer
    char write_buffer[BUFFER_SIZE];    // Output buffer
    int read_pos;                      // Current read position
    int write_pos;                     // Current write position
    
    // AI-specific data
    player_t *player;                  // Player entity (NULL for GUI)
    team_t *team;                      // Team affiliation
    command_t *current_command;        // Currently executing command
    bool command_in_progress;          // Command execution flag
    uint64_t command_end_time;         // Command completion time
    
    // Queue management
    list_t *command_queue;             // Pending commands
} client_t;
```

### Connection Lifecycle

The server implements a sophisticated connection state machine:

```c
// Connection handling
void handle_new_connection(server_t *server) {
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    
    // Accept new connection
    int client_fd = accept(server->server_fd, 
                          (struct sockaddr *)&client_addr, 
                          &addrlen);
    
    if (client_fd < 0) {
        log_error("Failed to accept connection");
        return;
    }
    
    // Configure as non-blocking
    set_nonblocking(client_fd);
    
    // Initialize client structure
    client_t *client = create_client(client_fd);
    
    // Add to polling system
    add_client_to_poll(server->poll_manager, client);
    
    // Send welcome message
    if (send_to_client(client, "WELCOME\n") < 0) {
        close_client_connection(server, client);
        return;
    }
    
    log_info("New client connected from %s:%d", 
             inet_ntoa(client_addr.sin_addr), 
             ntohs(client_addr.sin_port));
}
```

## Command Processing System

### Command Structure

Commands are represented as structured objects with execution parameters:

```c
typedef struct command_s {
    command_type_t type;               // Command identifier
    char *raw_text;                    // Original command text
    void *params;                      // Command-specific parameters
    uint64_t execution_time;           // Required execution time in ticks
    uint64_t start_time;               // When command began executing
    bool is_executing;                 // Execution state flag
    command_result_t (*execute)(struct command_s*, player_t*); // Execution function
    void (*free_params)(void*);        // Parameter cleanup function
} command_t;
```

### Command Table

The server uses a dispatch table to map commands to handlers:

| Command | Execution Time (ticks) | Description |
|---------|------------------------|-------------|
| `Forward` | 7 | Move player forward one tile |
| `Right` | 7 | Turn player right (90°) |
| `Left` | 7 | Turn player left (90°) |
| `Look` | 7 | Observe surrounding tiles |
| `Inventory` | 1 | Check player's inventory |
| `Broadcast` | 7 | Send message to all players |
| `Connect_nbr` | 0 | Get available team slots |
| `Fork` | 42 | Create new player egg |
| `Eject` | 7 | Eject players from current tile |
| `Take` | 7 | Take object from tile |
| `Set` | 7 | Place object on tile |
| `Incantation` | Variable | Perform level-up ritual |

### Command Execution Pipeline

```c
// Command execution flow
void process_commands(server_t *server) {
    uint64_t current_tick = server->time_manager->current_tick;
    
    // Process all clients
    for (int i = 0; i < server->client_manager->count; i++) {
        client_t *client = server->client_manager->clients[i];
        
        // Skip non-AI clients or inactive ones
        if (client->type != AI || client->state != ACTIVE || !client->player) {
            continue;
        }
        
        // Check for command in progress
        if (client->command_in_progress) {
            if (current_tick >= client->command_end_time) {
                // Command completed, execute and send result
                command_result_t result = client->current_command->execute(
                    client->current_command, client->player);
                
                send_command_result(client, result);
                
                // Clean up command
                free_command(client->current_command);
                client->current_command = NULL;
                client->command_in_progress = false;
            }
        } 
        // Start next command if available
        else if (!list_is_empty(client->command_queue)) {
            client->current_command = list_pop_front(client->command_queue);
            client->command_in_progress = true;
            client->command_end_time = current_tick + 
                                     client->current_command->execution_time;
            
            // Special handling for immediate commands
            if (client->current_command->execution_time == 0) {
                command_result_t result = client->current_command->execute(
                    client->current_command, client->player);
                
                send_command_result(client, result);
                
                // Clean up command
                free_command(client->current_command);
                client->current_command = NULL;
                client->command_in_progress = false;
            }
        }
    }
}
```

## Map System

### Tile Structure

Each map tile contains resource quantities and entity references:

```c
typedef struct tile_s {
    // Resource quantities
    int food;                          // Food units
    int linemate;                      // Level 1 stone
    int deraumere;                     // Level 2 crystal
    int sibur;                         // Level 3 mineral
    int mendiane;                      // Level 4 gem
    int phiras;                        // Level 5 stone
    int thystame;                      // Level 6 crystal
    
    // Entities on tile
    list_t *players;                   // Players on this tile
    list_t *eggs;                      // Eggs on this tile
    
    // Tile state
    bool incantation_active;           // Active ritual flag
    uint64_t last_resource_update;     // Resource generation timestamp
} tile_t;
```

### World Generation

The map is generated with configurable parameters:

```c
map_t *create_map(int width, int height, resource_config_t *config) {
    map_t *map = zappy_malloc(sizeof(map_t));
    map->width = width;
    map->height = height;
    
    // Allocate tile grid
    map->tiles = zappy_malloc(sizeof(tile_t*) * height);
    for (int y = 0; y < height; y++) {
        map->tiles[y] = zappy_malloc(sizeof(tile_t) * width);
        
        // Initialize each tile
        for (int x = 0; x < width; x++) {
            tile_t *tile = &map->tiles[y][x];
            memset(tile, 0, sizeof(tile_t));
            
            tile->players = list_create();
            tile->eggs = list_create();
            
            // Initial resources based on probability
            if (rand() % 100 < config->food_probability) {
                tile->food = rand() % config->max_food_per_tile + 1;
            }
            
            // Initialize other resources with decreasing probability
            // ...
        }
    }
    
    return map;
}
```

## Player Management

### Player Structure

Players have extensive state information:

```c
typedef struct player_s {
    int id;                            // Unique ID
    int x;                             // X position
    int y;                             // Y position
    direction_t direction;             // Facing direction (N,E,S,W)
    int level;                         // Current level (1-8)
    
    // Inventory
    inventory_t inventory;             // Resource storage
    
    // Game state
    team_t *team;                      // Team affiliation
    bool alive;                        // Survival status
    uint64_t last_food_time;           // Last food consumption time
    uint64_t next_food_time;           // Next food consumption time
    
    // References
    client_t *client;                  // Associated client
} player_t;

typedef struct inventory_s {
    int food;                          // Food units
    int linemate;                      // Level 1 stone
    int deraumere;                     // Level 2 crystal
    int sibur;                         // Level 3 mineral
    int mendiane;                      // Level 4 gem
    int phiras;                        // Level 5 stone
    int thystame;                      // Level 6 crystal
} inventory_t;
```

### Player Lifecycle

```c
// Player creation
player_t *create_player(server_t *server, team_t *team) {
    player_t *player = zappy_malloc(sizeof(player_t));
    
    // Initialize with defaults
    player->id = server->next_player_id++;
    player->level = 1;
    player->alive = true;
    player->team = team;
    
    // Random starting position
    player->x = rand() % server->map->width;
    player->y = rand() % server->map->height;
    
    // Random starting direction
    player->direction = rand() % 4;
    
    // Initial inventory
    memset(&player->inventory, 0, sizeof(inventory_t));
    player->inventory.food = 10;  // Starting food
    
    // Set food timers
    player->last_food_time = server->time_manager->current_tick;
    player->next_food_time = player->last_food_time + FOOD_CONSUMPTION_RATE;
    
    // Add to map tile
    add_player_to_tile(server->map, player);
    
    log_info("Created player %d in team %s at position (%d,%d)",
             player->id, team->name, player->x, player->y);
             
    return player;
}
```

## Incantation System

### Level Requirements

The incantation system enforces specific requirements for each level upgrade:

```c
// Level requirements verification
bool check_incantation_requirements(map_t *map, player_t *player) {
    tile_t *tile = &map->tiles[player->y][player->x];
    int player_count = list_size(tile->players);
    
    // Requirements array indexed by target level (level-1)
    static const struct {
        int players;       // Required player count
        int linemate;      // Required linemate
        int deraumere;     // Required deraumere
        int sibur;         // Required sibur
        int mendiane;      // Required mendiane
        int phiras;        // Required phiras
        int thystame;      // Required thystame
    } level_reqs[] = {
        {1, 1, 0, 0, 0, 0, 0},  // Level 1->2
        {2, 1, 1, 1, 0, 0, 0},  // Level 2->3
        {2, 2, 0, 1, 0, 2, 0},  // Level 3->4
        {4, 1, 1, 2, 0, 1, 0},  // Level 4->5
        {4, 1, 2, 1, 3, 0, 0},  // Level 5->6
        {6, 1, 2, 3, 0, 1, 0},  // Level 6->7
        {6, 2, 2, 2, 2, 2, 1}   // Level 7->8
    };
    
    // Check current level is valid for advancement
    if (player->level <= 0 || player->level >= 8) {
        return false;
    }
    
    // Get requirements for next level
    const int req_idx = player->level - 1;
    
    // Check player count
    if (player_count < level_reqs[req_idx].players) {
        return false;
    }
    
    // Check minimum player level
    for (node_t *node = tile->players->head; node != NULL; node = node->next) {
        player_t *p = (player_t *)node->data;
        if (p->level < player->level) {
            return false;  // All players must be at least this level
        }
    }
    
    // Check resources on tile
    if (tile->linemate < level_reqs[req_idx].linemate ||
        tile->deraumere < level_reqs[req_idx].deraumere ||
        tile->sibur < level_reqs[req_idx].sibur ||
        tile->mendiane < level_reqs[req_idx].mendiane ||
        tile->phiras < level_reqs[req_idx].phiras ||
        tile->thystame < level_reqs[req_idx].thystame) {
        return false;
    }
    
    return true;
}
```

## Time Management

The server implements a sophisticated time management system:

```c
typedef struct time_manager_s {
    int frequency;                      // Ticks per second
    double tick_duration_ms;            // Milliseconds per tick
    uint64_t current_tick;              // Current game time
    struct timeval last_update_time;    // Last update time
} time_manager_t;

void update_time(server_t *server) {
    time_manager_t *tm = server->time_manager;
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    
    // Calculate elapsed time in milliseconds
    long elapsed_ms = 
        (current_time.tv_sec - tm->last_update_time.tv_sec) * 1000 +
        (current_time.tv_usec - tm->last_update_time.tv_usec) / 1000;
    
    // Calculate elapsed ticks
    uint64_t elapsed_ticks = elapsed_ms / tm->tick_duration_ms;
    
    if (elapsed_ticks > 0) {
        tm->current_tick += elapsed_ticks;
        tm->last_update_time = current_time;
        
        // Process time-based events like food consumption
        process_time_events(server, elapsed_ticks);
    }
}
```

## Network I/O System

The server uses non-blocking I/O with a polling mechanism:

```c
typedef struct poll_manager_s {
    struct pollfd *pollfds;            // Array of poll structures
    int nfds;                           // Number of file descriptors
    int capacity;                       // Allocated capacity
} poll_manager_t;

void process_network_events(server_t *server) {
    poll_manager_t *pm = server->poll_manager;
    
    // Wait for network events (timeout = 10ms for responsiveness)
    int activity = poll(pm->pollfds, pm->nfds, 10);
    
    if (activity < 0) {
        if (errno != EINTR) {
            log_error("Poll failed: %s", strerror(errno));
        }
        return;
    }
    
    // Check server socket for new connections
    if (pm->pollfds[0].revents & POLLIN) {
        handle_new_connection(server);
    }
    
    // Process client sockets
    for (int i = 1; i < pm->nfds; i++) {
        // Handle readable sockets (data available)
        if (pm->pollfds[i].revents & POLLIN) {
            process_client_input(server, i);
        }
        
        // Handle writable sockets (can send data)
        if (pm->pollfds[i].revents & POLLOUT) {
            process_client_output(server, i);
        }
        
        // Handle disconnect/error events
        if (pm->pollfds[i].revents & (POLLHUP | POLLERR | POLLNVAL)) {
            handle_client_disconnect(server, i);
        }
    }
}
```

## Resource Optimization

The server includes several optimizations for resource efficiency:

### Memory Management

```c
// Custom memory allocator with tracking
void *zappy_tracked_malloc(size_t size, const char *file, int line) {
    void *ptr = malloc(size);
    if (!ptr) {
        log_critical("Memory allocation failed at %s:%d", file, line);
        exit(EXIT_FAILURE);
    }
    
    // Track allocation for leak detection
    #ifdef MEMORY_DEBUG
    track_allocation(ptr, size, file, line);
    #endif
    
    return ptr;
}

// Memory leak detection
void check_memory_leaks(void) {
    #ifdef MEMORY_DEBUG
    allocation_stats_t stats = get_allocation_stats();
    
    if (stats.active_allocations > 0) {
        log_warning("Memory leak detected: %d allocations, %zu bytes not freed",
                   stats.active_allocations, stats.active_bytes);
        
        // Print detailed leak information
        print_memory_leaks();
    }
    #endif
}
```

### Performance Monitoring

```c
typedef struct server_stats_s {
    // Network stats
    uint64_t bytes_received;
    uint64_t bytes_sent;
    uint64_t connections_accepted;
    uint64_t connections_closed;
    
    // Command stats
    uint64_t commands_processed;
    uint64_t commands_successful;
    uint64_t commands_failed;
    
    // Performance stats
    double avg_tick_time_ms;
    double peak_tick_time_ms;
    uint64_t tick_samples;
    
    // World stats
    uint64_t resources_generated;
    uint64_t resources_consumed;
    uint64_t players_died;
    uint64_t eggs_laid;
    uint64_t eggs_hatched;
    uint64_t incantations_started;
    uint64_t incantations_completed;
} server_stats_t;
```

---

This document provides a technical overview of the Zappy server's architecture and implementation. For protocol details, refer to the protocol documentation.
