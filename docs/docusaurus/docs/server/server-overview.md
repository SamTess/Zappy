---
sidebar_position: 1
---

# Zappy Server Documentation

## Overview

The Zappy server is the central component of the project, developed in C for optimal performance. It manages the game state, client connections, and enforces all game rules.

## Server Architecture

### Main Structure

```c
typedef struct server_s {
    int nfds;                           // Number of file descriptors
    poll_manager_t *poll_manager;       // Polling manager
} server_t;
```

## Client Management

### Client Types

The server manages two types of clients :

1. **AI Clients** : Control the game players
2. **Graphical Clients** : Observe the game state for display

```c
enum client_type_e {
    GRAPHICAL,
    AI
};

typedef struct client_s {
    int client_id;                      // Unique identifier
    enum client_type_e type;            // Client type
} client_t;
```

### Connection Cycle

1. **Acceptance** : The server accepts the TCP connection
2. **Authentication** : WELCOME exchange and team name
3. **Configuration** : Sending game parameters
4. **Activation** : The client is ready to receive commands

## Command System

### AI Command Table

| Command | Cost (ticks) | Description |
|----------|--------------|-------------|
| `Forward` | 7 | Move forward one square |
| `Right` | 7 | Turn right |
| `Left` | 7 | Turn left |
| `Look` | 7 | Observe the environment |
| `Inventory` | 1 | Check inventory |
| `Broadcast <text>` | 7 | Broadcast a message |
| `Connect_nbr` | 0 | Number of available connections |
| `Fork` | 42 | Spawn an egg |
| `Eject` | 7 | Eject players from the square |
| `Take <object>` | 7 | Pick up an object |
| `Set <object>` | 7 | Drop an object |
| `Incantation` | Variable | Cast an incantation |

### Graphical Commands

| Command | Description |
|----------|-------------|
| `msz` | Size of the map |
| `bct X Y` | Content of a square |
| `mct` | Content of the entire map |
| `tna` | Team names |
| `ppo #n` | Position of a player |
| `plv #n` | Level of a player |
| `pin #n` | Inventory of a player |

## Map Management

### Square Structure

```c
typedef struct tile_s {
    int food;           // Amount of food
    int linemate;       // Amount of linemate
    int deraumere;      // Amount of deraumere
    int sibur;          // Amount of sibur
    int mendiane;       // Amount of mendiane
    int phiras;         // Amount of phiras
    int thystame;       // Amount of thystame
    player_t *players;  // List of players on the square
} tile_t;
```

### Resource Distribution Algorithm

```c
void distribute_resources(server_t *server) {
    int total_tiles = server->parsed_info->width * server->parsed_info->height;
    
    // Proportional distribution of resources
    for (int resource = 0; resource < 7; resource++) {
        int to_place = server->total_resources[resource];
        while (to_place > 0) {
            int x = rand() % server->parsed_info->width;
            int y = rand() % server->parsed_info->height;
            server->map[y][x].resources[resource]++;
            to_place--;
        }
    }
}
```

## Temporal System

### Tick Management

The server operates with a tick system based on the configured frequency :

```c
void update_game_tick(server_t *server) {
    server->current_tick++;
    
    // Processing pending commands
    client_t *current = server->client;
    while (current) {
        if (current->player && current->player->action_time <= server->current_tick) {
            process_next_queued_command(server, current);
        }
        current = current->next;
    }
    
    // Reducing player food
    reduce_player_food(server);
    
    // Generating new resources
    if (server->current_tick % RESOURCE_GENERATION_RATE == 0) {
        generate_new_resources(server);
    }
}
```

### Command Queue

Each client has a command queue to manage asynchrony :

```c
typedef struct command_queue_s {
    char *command;
    struct command_queue_s *next;
} command_queue_t;

void add_to_command_queue(server_t *server, client_t *client, char *command) {
    command_queue_t *new_cmd = malloc(sizeof(command_queue_t));
    new_cmd->command = strdup(command);
    new_cmd->next = NULL;
    
    // Adding to the end of the queue
    if (client->player->command_queue == NULL) {
        client->player->command_queue = new_cmd;
    } else {
        command_queue_t *current = client->player->command_queue;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_cmd;
    }
}
```

## Incantation Management

### Promotion Conditions

```c
typedef struct elevation_requirements_s {
    int level;
    int players_required;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
} elevation_requirements_t;

static elevation_requirements_t requirements[] = {
    {1, 1, 1, 0, 0, 0, 0, 0},
    {2, 2, 1, 1, 1, 0, 0, 0},
    {3, 2, 2, 0, 1, 0, 2, 0},
    {4, 4, 1, 1, 2, 0, 1, 0},
    {5, 4, 1, 2, 1, 3, 0, 0},
    {6, 6, 1, 2, 3, 0, 1, 0},
    {7, 6, 2, 2, 2, 2, 2, 1}
};
```

### Incantation Process

1. **Verification** : Check resources and present players
2. **Start** : Send `Elevation underway` to all participants
3. **Waiting** : Waiting period (300 ticks)
4. **Finalization** : Promotion or failure according to conditions

## Optimizations and Performance

### Non-blocking Polling

```c
typedef struct poll_manager_s {
    struct pollfd *fds;     // Array of descriptors
    int capacity;           // Array capacity
    bool needs_rebuild;     // Rebuilding flag
} poll_manager_t;

void rebuild_poll_array(server_t *server) {
    if (!server->poll_manager->needs_rebuild) return;
    
    // Rebuilding the polling array
    int count = 1; // Server socket
    client_t *current = server->client;
    
    while (current) {
        count++;
        current = current->next;
    }
    
    server->poll_manager->fds = realloc(server->poll_manager->fds, 
                                       count * sizeof(struct pollfd));
    server->poll_manager->capacity = count;
    server->poll_manager->needs_rebuild = false;
}
```

### Memory Management

- **Object pools** : Reuse of client and player structures
- **Circular buffers** : For incoming/outgoing messages
- **Progressive release** : Avoids latency spikes

## Compilation and Configuration

### Compilation Options

```makefile
CFLAGS = -Wall -Wextra -Werror -std=c99
DEBUG_FLAGS = -g3 -DDEBUG
RELEASE_FLAGS = -O3 -DNDEBUG
```

### Server Configuration

```bash
# Starting with full configuration
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 team3 -c 5 -f 100 \
               --auto-start on --display-eggs true --game_duration 3600 -v
```

## Debugging and Monitoring

### Debug Logs

```c
#ifdef DEBUG
    #define DEBUG_LOG(fmt, ...) printf("[DEBUG] " fmt "\n", ##__VA_ARGS__)
#else
    #define DEBUG_LOG(fmt, ...)
#endif

void log_client_action(client_t *client, const char *action) {
    DEBUG_LOG("Client %d (team: %s) executed: %s", 
              client->client_id, 
              client->player->team_name, 
              action);
}
```

### Performance Metrics

- Number of connected clients
- Real ticks per second
- Memory used
- Average command latency

## Security and Robustness

### Input Validation

```c
bool validate_command(const char *command) {
    if (!command || strlen(command) == 0) return false;
    if (strlen(command) > MAX_COMMAND_LENGTH) return false;
    
    // Check for allowed characters
    for (int i = 0; command[i]; i++) {
        if (!isprint(command[i]) && command[i] != '\n') {
            return false;
        }
    }
    return true;
}
```

### Disconnection Management

- Automatic resource cleanup
- Release of positions on the map
- Notification to graphical clients

## Testing and Validation

### Unit Tests

```c
void test_player_movement() {
    server_t server;
    init_test_server(&server, 10, 10);
    
    player_t player = {.x = 5, .y = 5, .orientation = NORTH};
    move_player_forward(&server, &player);
    
    assert(player.x == 5 && player.y == 4); // Move north
}
```

### Integration Tests

- Simulation of multiple clients
- Load testing with 100+ connections
- Validation of state synchronization
