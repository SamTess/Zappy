---
sidebar_position: 1
---

# Communication Protocols

## Overview

The Zappy project relies on two distinct TCP/IP-based network protocols that facilitate communication between the server and client components. These protocols are critical to the system's functionality and are designed to support the specific needs of each client type.

## Protocol Architecture

Both protocols share the following characteristics:
- **Transport**: TCP/IP for reliable, ordered communication
- **Format**: Text-based messages with newline terminators (`\n`)
- **Pattern**: Command/response pattern with asynchronous notifications
- **Character Encoding**: ASCII (7-bit) for maximum compatibility

## Server-AI Protocol

This protocol manages communication between the server and artificial intelligence clients (players).

### Key Features
- **Bidirectional**: AI sends commands, server responds with results
- **Action-Based**: Each command represents a player action in the game world
- **Time-Costed**: Commands have associated execution times in server ticks
- **Stateful**: Maintains player state across command sequences
- **Authentication**: Requires team name for connection establishment

### Command Categories
1. **Movement Commands**: Control player position and orientation
2. **Perception Commands**: Gather information about the environment
3. **Interaction Commands**: Manipulate objects in the game world
4. **Communication Commands**: Send messages to other players
5. **Evolution Commands**: Perform incantations to level up

## Server-GUI Protocol

This protocol facilitates communication between the server and the graphical interface.

### Key Features
- **Observer Pattern**: GUI primarily acts as an observer of the game state
- **Comprehensive State**: Provides complete visibility of the game world
- **Notification System**: Server proactively informs of game events
- **Query Capability**: GUI can request specific information about the game state
- **Minimal Impact**: GUI has limited ability to modify the game state (speed control only)
- **Authentication**: Uses the `GRAPHIC` command for connection identification

### Message Categories
1. **Map Information**: Dimensions and content of the game map
2. **Player Data**: State, position, and inventory of players
3. **Game Events**: Notifications of significant game events
4. **Resource Information**: Distribution and quantities of resources
5. **Team Data**: Information about teams and their members

## Protocol Comparison

| Feature | Server-AI Protocol | Server-GUI Protocol |
|---------|--------------------|--------------------|
| **Purpose** | Player control | Game visualization |
| **Visibility** | Limited to player perception | Full game state visibility |
| **Authentication** | Team name | `GRAPHIC` command |
| **Game Impact** | Modifies game state | Observer with speed control only |
| **Command Timing** | Time-costed actions | Immediate responses |
| **Information Scope** | Player-centric | Global game state |
| **Communication Direction** | Primarily client-initiated | Balanced with server notifications |

## Technical Implementation

### Message Format
- **Command Messages**: `<COMMAND> [<PARAMETERS>]\n`
- **Response Messages**: `<RESPONSE_TEXT>\n` or multi-line responses
- **Notification Messages**: `<EVENT_TYPE> <PARAMETERS>\n`

### Connection Flow
1. **TCP Connection**: Client establishes TCP connection with server
2. **Authentication**: Client identifies itself (team name or `GRAPHIC`)
3. **Initialization**: Server sends initial state information
4. **Command Exchange**: Regular exchange of commands and responses
5. **Notifications**: Server pushes event notifications as they occur

### Error Handling
- **Syntax Errors**: Server responds with `ko\n` or error-specific messages
- **Timeout Handling**: Both protocols implement appropriate timeouts
- **Reconnection Logic**: Clients can reconnect after disconnection

## Protocol Extensions

The protocols support extension through:
- **Version Negotiation**: Potential for protocol versioning
- **Custom Commands**: Space for additional commands in future versions
- **Metadata Exchange**: Support for sending metadata about the game state

---

For detailed protocol specifications, see the [Server-AI Protocol](./server-ai) and [Server-GUI Protocol](./server-gui) documentation.
