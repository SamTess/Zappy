---
sidebar_position: 1
---

# Server-AI Protocol

## Overview

This document details the communication protocol between the Zappy server and artificial intelligence (AI) clients. The protocol uses text-based exchanges over TCP/IP with specified commands and responses, enabling AI clients to interact with the game world.

## Connection Sequence

1. **TCP Connection**: AI client connects to the server via TCP/IP
2. **Welcome Message**: Server responds with: `WELCOME\n`
3. **Team Selection**: AI client sends team name: `<team_name>\n`
4. **Connection Confirmation**: Server responds with:
   - `<client_num>\n`: Client's sequence number within the team
   - `<X> <Y>\n`: Map dimensions (width × height)

## Message Format

- All commands and responses are terminated with a newline character (`\n`)
- Commands have specific execution times measured in server time units
- Multi-line responses end with an empty line (`\n`)

## Command Reference

### Basic Commands

| Command | Description | Response Format | Duration (time units) | Failure Response |
|---------|-------------|-----------------|----------------------|------------------|
| `Forward` | Move forward one tile | `ok\n` | 7 | N/A |
| `Right` | Turn 90° clockwise | `ok\n` | 7 | N/A |
| `Left` | Turn 90° counter-clockwise | `ok\n` | 7 | N/A |
| `Look` | Survey the environment | Multi-line tile contents | 7 | N/A |
| `Inventory` | Check inventory contents | Multi-line resource list | 1 | N/A |
| `Broadcast <text>` | Send message to all players | `ok\n` | 7 | N/A |
| `Connect_nbr` | Get available team slots | `<n>\n` (integer) | 0 | N/A |
| `Fork` | Create a new player egg | `ok\n` | 42 | N/A |
| `Eject` | Push players from current tile | `ok\n` or `ko\n` | 7 | `ko\n` |
| `Take <object>` | Pick up a resource | `ok\n` or `ko\n` | 7 | `ko\n` |
| `Set <object>` | Place a resource | `ok\n` or `ko\n` | 7 | `ko\n` |
| `Incantation` | Start level-up ritual | Start: `Elevation underway\n`<br />End: `Current level: <level>\n` or `ko\n` | Variable | `ko\n` |

### Resource Types

Valid object names for `Take` and `Set` commands:
- `food`: Food units (hunger management)
- `linemate`: Level 1 resource (green)
- `deraumere`: Level 2 resource (blue)
- `sibur`: Level 3 resource (red)
- `mendiane`: Level 4 resource (purple)
- `phiras`: Level 5 resource (yellow)
- `thystame`: Level 6 resource (cyan)

## Response Formats

### Look Command

The `Look` command returns a multi-line response representing the visible area:
```
[<obj1> <obj2> ...],
[<obj1> <obj2> ...],
...
```

- The first line shows the contents of the player's tile
- Subsequent lines represent tiles in expanding vision squares
- Vision range depends on player level (levels 1-8: 1-9 tiles per side)
- Objects can be: `player`, `food`, or any resource type

#### Vision Pattern by Level

```
Level 1: 3×3 square (9 tiles)
Level 2: 5×5 square (25 tiles)
Level 3: 7×7 square (49 tiles)
Level 4: 9×9 square (81 tiles)
Level 5: 11×11 square (121 tiles)
...etc
```

### Inventory Command

The `Inventory` command returns the player's current inventory:
```
[food <qty>, linemate <qty>, deraumere <qty>, sibur <qty>, mendiane <qty>, phiras <qty>, thystame <qty>]
```

### Broadcast Command

When a player receives a broadcast, they receive a message:
```
message <K>, <text>
```

Where `K` is a direction index (1-8) indicating where the message came from:
```
     2 1 8
     3 X 7
     4 5 6
```
(`X` represents the player's position)

## Incantation Requirements

Each level upgrade requires specific resources on the tile and player participation:

| Level Transition | Players | linemate | deraumere | sibur | mendiane | phiras | thystame |
|------------------|---------|----------|-----------|-------|----------|--------|----------|
| 1→2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2→3 | 2 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3→4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4→5 | 4 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5→6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6→7 | 6 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7→8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

## Player States

### Orientation
Player orientation is one of four cardinal directions:
1. North (top of map)
2. East (right of map)
3. South (bottom of map)
4. West (left of map)

### Food Management
- Each player consumes 1 food unit every 126 time units
- Death occurs when a player runs out of food
- Initial inventory contains 10 food units

## Technical Details

### Time Units
- Server operates on discrete time units (ticks)
- Default frequency is 100 ticks per second (configurable via server `-f` parameter)
- Command costs are measured in ticks

### Error Handling
- Invalid commands receive `ko\n` 
- Malformed commands may disconnect the client
- Commands during execution of another command are enqueued

### Concurrency
- Multiple commands can be sent without waiting for responses
- Responses arrive in same order as commands were sent
- Command execution is sequential per player

### Map Properties
- The map is toroidal (wraps around edges)
- Coordinates range from 0 to width-1 (X) and 0 to height-1 (Y)
- Map can contain multiple players on one tile

---

This protocol documentation provides a comprehensive reference for developers implementing AI clients for the Zappy server. For implementation guidance, refer to the AI client architecture documentation.
