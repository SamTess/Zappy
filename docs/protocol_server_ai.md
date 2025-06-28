# Server-AI Communication Protocol

## Overview

This document details the communication protocol between the Zappy server and artificial intelligence (AI) clients. The protocol is based on textual exchanges with specific commands and their associated responses.

## Connection

1. The AI connects to the server via TCP/IP
2. The server responds with: `WELCOME\n`
3. The AI sends its team name: `<team_name>\n`
4. The server responds with:
   - `<client_num>\n`: Client number in the team
   - `<X> <Y>\n`: Map dimensions

## Command Format

- Each command is a string ending with `\n`
- Server responses also end with `\n`
- Each command has a specific execution time
- Commands are queued if the player is busy

## Available Commands

### Basic Commands

| Command | Description | Result | Duration (server time unit) |
|---------|-------------|--------|------------------------------|
| `Forward` | Move forward one tile | `ok\n` | 7 |
| `Right` | Turn right | `ok\n` | 7 |
| `Left` | Turn left | `ok\n` | 7 |
| `Look` | Observe environment | List of visible objects | 7 |
| `Inventory` | Check inventory | List of owned objects | 1 |
| `Broadcast <text>` | Communicate with other players | `ok\n` | 7 |
| `Connect_nbr` | Number of available connections | Integer number | 0 |
| `Fork` | Create a new egg | `ok\n` | 42 |
| `Eject` | Eject players from the tile | `ok\n` or `ko\n` | 7 |
| `Take <object>` | Pick up an object | `ok\n` or `ko\n` | 7 |
| `Set <object>` | Drop an object | `ok\n` or `ko\n` | 7 |
| `Incantation` | Cast an incantation | Success or failure of incantation | 300 |

### Manipulable Objects

- `food`: Food (consumed automatically: 1 unit/126 ticks)
- `linemate`: Level 1 precious stone
- `deraumere`: Level 2 precious stone
- `sibur`: Level 3 precious stone
- `mendiane`: Level 4 precious stone
- `phiras`: Level 5 precious stone
- `thystame`: Level 6 precious stone

## Detailed Response Formats

### Look
The `Look` command returns a view of the environment as concentric tiles:
```
[<object1> <object2> ...],
[<object1> <object2> ...],
[<object1> <object2> ...],
...
```

**Visible objects:** `player`, `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`, `egg`

**View organization:**
- Tile 0: Player's current position
- Tiles 1-2: Tiles directly in front (according to orientation)
- Tiles 3-8: Level 2 tiles (fan-shaped)
- Tiles 9-15: Level 3 tiles, etc.

### Inventory
```
{food <n>, linemate <n>, deraumere <n>, sibur <n>, mendiane <n>, phiras <n>, thystame <n>}
```

### Broadcast
- **Emission**: The sending player receives `ok\n`
- **Reception**: Other players receive `message <direction>, <message>\n`
- **Direction**: Number from 0 to 8 representing message direction
  - 0: Same tile as sender
  - 1-8: Cardinal and diagonal directions

### Incantation
1. **Start**: Server responds immediately `Elevation underway\n`
2. **Verification**: Server checks conditions (resources + players)
3. **Result**:
   - **Success**: `Current level: <level>\n` (new level)
   - **Failure**: `ko\n` (conditions not met)

## Detailed Elevation Conditions

| Level | Required Players | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-------|------------------|----------|-----------|-------|----------|--------|----------|
| 1→2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2→3 | 1 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3→4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4→5 | 2 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5→6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6→7 | 4 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7→8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

**Important notes:**
- All players on the tile must be at the same level for incantation
- Resources are consumed during successful incantation
- Incantation can fail if conditions are no longer met at execution time

## Error Handling and Special Cases

### Invalid Commands
- **Unknown command**: `ko\n`
- **Invalid parameter**: `ko\n`
- **Impossible action**: `ko\n` (e.g., taking an object that doesn't exist)

### Player Death
- **Cause**: Lack of food (food inventory at 0)
- **Behavior**: Server closes connection without notification

### Ejection
- **Mechanism**: The `Eject` command pushes all other players from the tile
- **Direction**: Ejected players are moved in the ejector's direction
- **Notification**: Ejected players receive `eject: <direction>\n`

## Orientation and Movement

### Orientation System
- **0**: North (top of map)
- **1**: East (right of map)
- **2**: South (bottom of map)
- **3**: West (left of map)

### Toric Map
- The map is toric (edges connect)
- Exiting through one edge appears on the other side
- Coordinates are calculated modulo the dimensions

## Complete Exchange Example

```
CLIENT: <TCP connection>
SERVER: WELCOME\n
CLIENT: team1\n
SERVER: 0\n
SERVER: 10 10\n
CLIENT: Look\n
SERVER: [player food],[food sibur],[linemate],[],[],[],[],[]\n
CLIENT: Forward\n
SERVER: ok\n
CLIENT: Take linemate\n
SERVER: ok\n
CLIENT: Inventory\n
SERVER: {food 9, linemate 1, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0}\n
CLIENT: Broadcast Hello team!\n
SERVER: ok\n
CLIENT: Incantation\n
SERVER: Elevation underway\n
SERVER: Current level: 2\n
```

## Technical Considerations

### Performance
- **Server frequency**: Configurable via `-f` parameter
- **Queues**: Commands are queued if player is busy
- **Timeout**: No server-side timeout, but clients can implement their own timeouts

### Multithreading
- **Thread-safety**: Server handles concurrent access
- **Command order**: Commands are processed in arrival order per player
- **Synchronization**: Events (incantations, ejections) are synchronized between all clients
