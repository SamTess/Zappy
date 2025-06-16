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

## Available Commands

### Basic Commands

| Command   | Description         | Result | Duration (server time unit) |
|-----------|---------------------|--------|----------------------------|
| `Forward` | Move forward one tile | `ok\n` | 7 |
| `Right`   | Turn right          | `ok\n` | 7 |
| `Left`    | Turn left           | `ok\n` | 7 |
| `Look`    | Observe the environment | List of visible objects | 7 |
| `Inventory` | Check inventory    | List of
| `Broadcast <text>` | Communiquer avec les autres joueurs | `ok\n` | 7 |
| `Connect_nbr` | Nombre de connexions disponibles | Nombre entier | 0 |
| `Fork` | Créer un nouvel œuf | `ok\n` | 42 |
| `Eject` | Éjecter les joueurs de la case | `ok\n` ou `ko\n` | 7 |
| `Take <object>` | Ramasser un objet | `ok\n` ou `ko\n` | 7 |
| `Set <object>` | Poser un objet | `ok\n` ou `ko\n` | 7 |
| `Incantation` | Lancer une incantation | Succès ou échec de l'incantation | Variable |

### Format des réponses

#### Look
```
[<object1> <object2> ...],
[<object1> <object2> ...],
...
```

Objets possibles : `player`, `food`, `linemate`, `deraumere`, `sibur`, `mendiane`, `phiras`, `thystame`, `egg`

#### Inventory
```
{food <n>, linemate <n>, deraumere <n>, sibur <n>, mendiane <n>, phiras <n>, thystame <n>}
```

#### Broadcast
- Les joueurs reçoivent : `message <direction>, <message>\n`
- Direction : nombre de 1 à 8 représentant la direction du message

#### Incantation
1. Serveur répond immédiatement : `Elevation underway\n`
2. À la fin de l'incantation : `Current level: <level>\n` ou échec

## Conditions d'élévation

| Niveau | Joueurs requis | Linemate | Deraumere | Sibur | Mendiane | Phiras | Thystame |
|--------|----------------|----------|-----------|-------|----------|--------|----------|
| 1→2 | 1 | 1 | 0 | 0 | 0 | 0 | 0 |
| 2→3 | 2 | 1 | 1 | 1 | 0 | 0 | 0 |
| 3→4 | 2 | 2 | 0 | 1 | 0 | 2 | 0 |
| 4→5 | 4 | 1 | 1 | 2 | 0 | 1 | 0 |
| 5→6 | 4 | 1 | 2 | 1 | 3 | 0 | 0 |
| 6→7 | 6 | 1 | 2 | 3 | 0 | 1 | 0 |
| 7→8 | 6 | 2 | 2 | 2 | 2 | 2 | 1 |

## Mort du joueur

Si un joueur meurt (manque de nourriture), le serveur ferme la connexion.

## Exemple d'échange

```
CLIENT: <connexion>
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
```
