---
sidebar_position: 2
---

# Server-GUI Protocol

## Overview

This document details the communication protocol between the Zappy server and the graphical interface (GUI). The protocol uses text-based exchanges over TCP/IP that allow the GUI to visualize the game state in real-time with minimal impact on gameplay.

## Connection Process

1. **TCP Connection**: The GUI establishes a TCP connection to the server
2. **Client Identification**: GUI sends the command: `GRAPHIC\n` to identify itself as a graphical client
3. **Initial State**: The server responds with comprehensive information about the current game state

## Message Format

- All commands and responses are terminated with a newline character (`\n`)
- The GUI primarily receives updates but can make specific information requests
- Unlike AI clients, GUI commands are processed immediately with no time cost

## Initial Information Sequence

After connecting with the `GRAPHIC` command, the server sends the following information in sequence:

1. **Map Dimensions**: `msz X Y\n` (width X, height Y)
2. **Time Unit**: `sgt T\n` (time unit in milliseconds)
3. **Tile Contents**: `bct X Y q0 q1 q2 q3 q4 q5 q6\n` (for each tile)
   - q0: food quantity
   - q1-q6: quantities of each resource type
4. **Team Names**: `tna N\n` (for each team name N)
5. **Player Information**: `pnw # X Y O L N\n` (for each player)
   - #: Player ID
   - X, Y: Coordinates
   - O: Orientation (1-4)
   - L: Level (1-8)
   - N: Team name

## GUI Command Reference

The GUI can send the following commands to request specific information:

| Command | Description | Response Format |
|---------|-------------|-----------------|
| `msz` | Request map dimensions | `msz X Y\n` |
| `bct X Y` | Request tile content | `bct X Y q0 q1 q2 q3 q4 q5 q6\n` |
| `mct` | Request all tile contents | Series of `bct` responses |
| `tna` | Request team names | Series of `tna N\n` responses |
| `ppo #` | Request player position | `ppo # X Y O\n` |
| `plv #` | Request player level | `plv # L\n` |
| `pin #` | Request player inventory | `pin # X Y q0 q1 q2 q3 q4 q5 q6\n` |
| `sgt` | Request time unit | `sgt T\n` |
| `sst T` | Modify time unit | `sst T\n` |

## Server Notifications

The server proactively sends notifications about game events to the GUI:

| Notification | Format | Description | Parameters |
|--------------|--------|-------------|------------|
| **Player Events** | | | |
| New player | `pnw # X Y O L N\n` | Player connects | #: ID, X/Y: pos, O: orientation, L: level, N: team |
| Player position | `ppo # X Y O\n` | Player moves/turns | #: ID, X/Y: pos, O: orientation |
| Player level | `plv # L\n` | Level changes | #: ID, L: new level |
| Player inventory | `pin # X Y q0 q1 q2 q3 q4 q5 q6\n` | Inventory changes | #: ID, X/Y: pos, q0-q6: resource quantities |
| Player expulsion | `pex #\n` | Player ejected | #: ID of expelled player |
| Player broadcast | `pbc # M\n` | Player broadcasts | #: ID, M: message text |
| Player death | `pdi #\n` | Player dies | #: ID of dead player |
| **Egg Events** | | | |
| Egg laying | `pfk #\n` | Player creates egg | #: ID of player laying egg |
| Egg laid | `enw # X Y N\n` | Egg is created | #: egg ID, X/Y: pos, N: team name |
| Egg collection | `ebo #\n` | Egg hatches into player | #: ID of hatched egg |
| Egg death | `edi #\n` | Egg expires | #: ID of dead egg |
| **Incantation Events** | | | |
| Start incantation | `pic X Y L #n\n` | Ritual begins | X/Y: pos, L: level, #n: list of player IDs |
| End incantation | `pie X Y R\n` | Ritual completes | X/Y: pos, R: result (0:fail, 1:success) |
| **Resource Events** | | | |
| Tile content | `bct X Y q0 q1 q2 q3 q4 q5 q6\n` | Resource change | X/Y: pos, q0-q6: resource quantities |
| **Game Events** | | | |
| End of game | `seg N\n` | Game ends | N: winning team name |
| Server message | `smg M\n` | Server notification | M: message text |
| Time unit change | `sgt T\n` | Time unit updated | T: new time unit value |

## Resource Identification

Resource quantities in tile content and inventory messages follow this order:
1. q0: Food
2. q1: Linemate
3. q2: Deraumere
4. q3: Sibur
5. q4: Mendiane
6. q5: Phiras
7. q6: Thystame

## Player Orientation

Orientation values in player position messages:
1. North (top of map)
2. East (right of map)
3. South (bottom of map)
4. West (left of map)

## Technical Details

### Time Unit Control

The GUI has limited ability to affect the game by modifying the server's time unit:
- `sst T\n`: Set the time unit to T milliseconds
- Valid range: 2 to 10000 milliseconds
- Lower values = faster game pace
- Default is typically 100 milliseconds

### Map Coordinates

- Coordinates are zero-based, starting from the top-left
- X increases from left to right (0 to width-1)
- Y increases from top to bottom (0 to height-1)
- The map is toroidal (wraps around edges)

### Connection Behavior

- Multiple GUI clients can connect simultaneously
- If the server is full, GUI clients can still connect (unlike AI clients)
- The GUI connection does not count against team player limits

### Protocol Extension Points

The protocol allows for future extensions:
- Additional notification types can be added with new codes
- The server ignores unknown commands from the GUI
- The GUI should ignore unknown notifications from the server

## Implementation Guidelines

### Parsing Strategy
1. Parse notifications by their prefix (first 3 characters)
2. Extract parameters based on the notification type
3. Update local game state accordingly
4. Trigger appropriate visualization updates

### Connection Recovery
1. If connection is lost, attempt to reconnect
2. Upon reconnection, identify as GUI again with `GRAPHIC\n`
3. The server will resend the complete game state

---

This protocol documentation provides a comprehensive reference for developers implementing the GUI client for the Zappy server. For implementation guidance, refer to the GUI architecture documentation.
