---
sidebar_position: 2
---

# Server-GUI Protocol

## Overview

This document details the communication protocol between the Zappy server and the graphical interface (GUI). The protocol is based on textual exchanges allowing the interface to visualize the game state in real time.

## Connection

1. The GUI connects to the server via TCP/IP
2. To differentiate itself from an AI client, the GUI sends the command: `GRAPHIC\n`
3. The server responds with initial information about the map and teams

## Command Format

- Each command is sent as a string ending with `\n`
- Server responses also end with `\n`
- The GUI cannot send commands that modify the game, only information requests

## Initial Information

After connecting with the `GRAPHIC` command, the server sends:

1. Map dimensions: `msz X Y\n` (width X, height Y)
2. Time unit: `sgt T\n` (time per action in ms)
3. Content of each tile: `bct X Y q0 q1 q2 q3 q4 q5 q6\n` (for each tile)
4. Team names: `tna N\n` (for each team)
5. Player positions and states: `pnw # X Y O L N\n` (for each player)

## GUI Commands

The GUI can send the following commands:

| Command | Description | Response |
|----------|-------------|---------|
| `msz` | Map dimensions | `msz X Y\n` |
| `bct X Y` | Content of a tile | `bct X Y q0 q1 q2 q3 q4 q5 q6\n` |
| `mct` | Content of all tiles | series of `bct` |
| `tna` | Team names | series of `tna N\n` |
| `ppo #` | Position of a player | `ppo # X Y O\n` |
| `plv #` | Level of a player | `plv # L\n` |
| `pin #` | Inventory of a player | `pin # X Y q0 q1 q2 q3 q4 q5 q6\n` |
| `sgt` | Time unit | `sgt T\n` |
| `sst T` | Modify the time unit | `sst T\n` |

## Server Notifications

The server sends notifications of game events to the GUI:

| Notification | Description |
|--------------|-------------|
| `pnw # X Y O L N` | New player |
| `pex #` | Player expelled |
| `pbc # M` | Broadcast a message |
| `pic X Y L #n` | Start of incantation |
| `pie X Y R` | End of incantation |
| `pfk #` | Laying of an egg |
| `pdr # i` | Player drops resource |
| `pgt # i` | Player takes resource |
| `pdi #` | Player death |
| `enw # X Y` | Egg creation |
| `eht #e` | Egg hatching |
| `edi #e` | Egg death |
| `sgr` | End of game |
| `smg M` | Server message |

## Resource Format

The content of tiles and inventories (q0-q6) corresponds to the resources:
- q0 : food
- q1 : linemate
- q2 : deraumere
- q3 : sibur
- q4 : mendiane
- q5 : phiras
- q6 : thystame

## Example Exchange

```
GUI: GRAPHIC\n
SERVER: msz 8 8\n
SERVER: sgt 100\n
SERVER: bct 0 0 1 0 0 0 0 0 0\n
SERVER: bct 0 1 0 1 0 0 0 0 0\n
...
SERVER: tna Team1\n
SERVER: tna Team2\n
...
GUI: mct\n
SERVER: bct 0 0 1 0 0 0 0 0 0\n
...
SERVER: pnw 1 3 2 1 1 Team1\n
SERVER: pic 3 2 1 1\n
```

## Remarks

- Coordinates X and Y start at 0
- Orientation O is a number from 1 to 4 (1: North, 2: East, 3: South, 4: West)
- Player level L ranges from 1 to 8
- # represents a player's ID
- #n represents a list of player IDs
- #e represents an egg's ID
- R is 0 (failure) or 1 (success) for incantations
