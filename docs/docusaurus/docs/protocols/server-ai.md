---
sidebar_position: 1
---

# Server-AI Protocol

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

## Available Commands

### Basic Commands

| Command   | Description         | Result | Duration (server time unit) |
|-----------|---------------------|--------|----------------------------|
| `Forward` | Move forward one tile | `ok\n` | 7 |
| `Right`   | Turn right          | `ok\n` | 7 |
| `Left`    | Turn left           | `ok\n` | 7 |
| `Look`    | Observe the environment | List of visible objects | 7 |
| `Inventory` | Check inventory    | List of owned resources | 1 |
| `Broadcast <text>` | Communicate with other players | `ok\n` | 7 |
| `Connect_nbr` | Number of available connections | Integer | 0 |
| `Fork`    | Create a new egg    | `ok\n` | 42 |
| `Eject`   | Eject players from the tile | `ok\n` or `ko\n` | 7 |
| `Take <object>` | Pick up an object | `ok\n` or `ko\n` | 7 |
| `Set <object>` | Place an object | `ok\n` or `ko\n` | 7 |
| `Incantation` | Cast an incantation | Success or failure of the incantation | Variable |

### Response Format

#### Look
```
[<object1> <object2> ...],
[<object1> <object2> ...],
...
```

Possible objects: `player`, `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`, `egg`

The result of the `Look` command is a list of visible tiles, organized as follows:
- The first tile is the one where the player is located
- The following tiles are arranged in concentric circles around the player
- The number of visible tiles depends on the player's level

#### Inventory
```
{food <n>, linemate <n>, deraumere <n>, sibur <n>, mendiane <n>, phiras <n>, thystame <n>}
```

#### Broadcast
- Players receive: `message <direction>, <message>\n`
- Direction: number from 1 to 8 representing the direction of the message
  - 1 = North, 2 = North-East, 3 = East, 4 = South-East, 5 = South, 6 = South-West, 7 = West, 8 = North-West

#### Incantation
1. Server immediately responds: `Elevation underway\n`
2. At the end of the incantation: `Current level: <level>\n` or failure with `ko\n`

To start an incantation:
- All participating players must be on the same tile
- The required resources must be present on the tile
- All players must be of the same level

## Elevation Conditions

| Level | Required Players | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|-------|------------------|----------|-----------|-------|----------|--------|----------|
| 1→2   | 1                | 1        | 0         | 0     | 0        | 0      | 0        |
| 2→3   | 2                | 1        | 1         | 1     | 0        | 0      | 0        |
| 3→4   | 2                | 2        | 0         | 1     | 0        | 2      | 0        |
| 4→5   | 4                | 1        | 1         | 2     | 0        | 1      | 0        |
| 5→6   | 4                | 1        | 2         | 1     | 3        | 0      | 0        |
| 6→7   | 6                | 1        | 2         | 3     | 0        | 1      | 0        |
| 7→8   | 6                | 2        | 2         | 2     | 2        | 2      | 1        |

## Field of Vision

The field of vision depends on the player's level and extends in concentric circles around them:

| Level | Visible Zones |
|-------|---------------|
| 1     | 1 (current tile) + 8 (circle 1) = 9 tiles |
| 2     | 9 + 16 (circle 2) = 25 tiles |
| 3     | 25 + 24 (circle 3) = 49 tiles |
| And so on... |  |

## Unsolicited Server Messages

The AI may receive certain messages from the server without having requested them:

| Message | Description |
|---------|-------------|
| `message <direction>, <message>\n` | Broadcast message received from another player |
| `dead\n` | The player is dead (lack of food) |
| `eject: <direction>\n` | The player has been ejected by another player |

## Food Logic

- Each player starts with a certain amount of food
- Food is consumed at a constant rate (default: 126 time units)
- If the food reaches zero, the player dies and the connection is closed

## Egg System

When a player uses the `Fork` command:
1. A new egg is created on the same tile as the player
2. This egg allows a new player to join the team
3. The hatching of the egg occurs when a new client connects

## Example Exchange

```
CLIENT: <connection>
SERVER: WELCOME\n
CLIENT: team1\n
SERVER: 0\n
SERVER: 8 8\n
CLIENT: Look\n
SERVER: [player food],[],[food],\n
CLIENT: Forward\n
SERVER: ok\n
CLIENT: Inventory\n
SERVER: {food 9, linemate 0, deraumere 0, sibur 0, mendiane 0, phiras 0, thystame 0}\n
CLIENT: Broadcast Hello team!\n
SERVER: ok\n
CLIENT: Take food\n
SERVER: ok\n
```
