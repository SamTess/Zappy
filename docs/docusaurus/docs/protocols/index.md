---
sidebar_position: 1
---

# Communication Protocols

## Overview

The Zappy project relies on network communication protocols that allow the different components to interact. There are two main protocols:

1. **Server-AI Protocol**: Used for communication between the server and artificial intelligence clients
2. **Server-GUI Protocol**: Used for communication between the server and the graphical interface

## Common Features

Both protocols share some characteristics:
- Based on TCP/IP
- Communication via textual messages
- Each message ends with a newline character (`\n`)
- Command/response format

## Main Differences

| Feature         | Server-AI Protocol | Server-GUI Protocol |
|-----------------|-------------------|---------------------|
| **Purpose**     | Player control     | Game visualization  |
| **Bidirectionality** | AI sends commands, server responds | GUI can request information, server also sends notifications |
| **Impact on game** | Commands modify game state | GUI does not modify game state (except for speed command) |
| **Authentication** | AI must join a team | GUI identifies itself with `GRAPHIC` command |

## Usage

For detailed documentation of each protocol, see the following pages:
- [Server-AI Protocol](./server-ai)
- [Server-GUI Protocol](./server-gui)
