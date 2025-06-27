# Zappy Server Documentation

## Overview

The Zappy server is the central component of the project, developed in C for optimal performance. It manages the game state, multiple client connections (AI), and the graphical interface. It is responsible for:
- Map and resource management with automatic distribution
- Player, team, and inventory management
- AI command processing with queue system
- Communication with graphical interface via dedicated protocol
- Game rule enforcement and time management
- Elevation rituals and spawning mechanisms

## Modular Architecture

The server is structured into several specialized modules:

### Network Manager (`network/`)
- **TCP/IP Connections**: Multi-connection management with non-blocking polling
- **Client Types**: Distinction between AI (`AI`) and graphical (`GRAPHICAL`) clients
- **Circular Buffers**: Optimized communication system to avoid blocking
- **Queue Manager**: Polling with `poll()` for efficient I/O management
- **Disconnection Detection**: Robust handling of disconnecting clients

### Game Engine (`map/`, `player/`)
- **Toric Map**: 2D map system with automatic wraparound
- **Resource Distribution**: Automatic generation and regeneration of resources
- **Egg Management**: Spawning and hatching system for new players
- **Player Inventories**: Complete management of objects and food
- **Player States**: Tracking positions, orientations, levels, and actions

### Command Processor (`command/`)
- **Queue System**: FIFO system for pending commands
- **Time Cost**: Each command has a cost in server time units
- **Validation**: Prerequisite verification before execution
- **AI Commands**: `Forward`, `Right`, `Left`, `Look`, `Inventory`, `Broadcast`, etc.
- **Special Commands**: `Incantation`, `Fork`, `Eject`, `Take`, `Set`

### Time Manager
- **Configurable Frequency**: `-f` parameter to define ticks per second
- **Synchronization**: Tick system to coordinate all actions
- **Starvation Management**: Automatic food consumption (1 unit/126 ticks)
- **Timed Events**: Delayed command execution according to their cost

## Communication Protocols

### AI Clients
The server accepts AI connections that authenticate with their team name and receive game commands.

**Connection Sequence:**
1. Client TCP/IP connection
2. Server responds: `WELCOME\n`
3. Client sends: `<team_name>\n`
4. Server responds: `<client_num>\n<X> <Y>\n`

**Main Commands:**
- **Movement**: `Forward` (7 ticks), `Right` (7 ticks), `Left` (7 ticks)
- **Perception**: `Look` (7 ticks), `Inventory` (1 tick)
- **Interaction**: `Take <object>` (7 ticks), `Set <object>` (7 ticks)
- **Communication**: `Broadcast <text>` (7 ticks)
- **Evolution**: `Incantation` (300 ticks), `Fork` (42 ticks)
- **Combat**: `Eject` (7 ticks)

### Graphical Clients
Graphical clients identify with `GRAPHIC` and receive all game events in real-time.

**Automatic Notifications:**
- **Map**: `msz X Y\n`, `bct X Y q0 q1 q2 q3 q4 q5 q6\n`
- **Players**: `pnw # X Y O L N\n`, `ppo # X Y O\n`, `plv # L\n`
- **Inventories**: `pin # X Y q0 q1 q2 q3 q4 q5 q6\n`
- **Events**: `pic X Y L #1 #2 ...\n`, `pie X Y R\n`, `pex #\n`
- **Teams**: `tna N\n`, `seg N\n`

## Main Data Structures

### Player (`player_t`)
```c
typedef struct player_s {
    int client_id;              // Unique identifier
    int pos_x, pos_y;          // Position on map
    int orientation;           // Direction (0-3: N, E, S, W)
    int level;                 // Current level (1-8)
    int inventory[7];          // [food, linemate, deraumere, sibur, mendiane, phiras, thystame]
    char *team_name;           // Team name
    pending_cmd_t *pending_cmd; // Command being executed
    char **command_queue;      // Command queue
    int life_time;             // Remaining life time
    int is_dead;               // Death state
} player_t;
```

### Map Tile (`tile_t`)
```c
typedef struct tile_s {
    int resources[7];          // Resources present on tile
    int *players;              // List of present players
    int nb_players;            // Number of players on tile
    int max_players;           // Maximum capacity
} tile_t;
```

### Egg (`egg_t`)
```c
typedef struct egg_s {
    int id;                    // Identifiant unique de l'œuf
    int pos_x, pos_y;         // Position de ponte
    char *team_name;          // Équipe associée
    struct egg_s *next;       // Liste chaînée
} egg_t;
```

## Gestion des rituels d'incantation

Les rituels d'incantation sont des mécanismes complexes nécessitant :

### Conditions d'élévation
Pour chaque niveau, des prérequis spécifiques :
- **Niveau 1→2** : 1 joueur, 1 linemate
- **Niveau 2→3** : 1 joueur, 1 linemate, 1 deraumere, 1 sibur
- **Niveau 3→4** : 2 joueurs, 2 linemate, 1 sibur, 1 phiras
- **Niveau 4→5** : 2 joueurs, 1 linemate, 1 deraumere, 2 sibur, 1 phiras
- **Niveau 5→6** : 4 joueurs, 1 linemate, 2 deraumere, 1 sibur, 1 mendiane
- **Niveau 6→7** : 4 joueurs, 1 linemate, 2 deraumere, 3 sibur, 1 phiras
- **Niveau 7→8** : 6 joueurs, 2 linemate, 2 deraumere, 2 sibur, 2 mendiane, 2 phiras, 1 thystame

### Processus d'incantation
1. **Vérification** : Présence des ressources et joueurs nécessaires
2. **Démarrage** : Notification `pic X Y L #1 #2 ...` aux clients graphiques
3. **Durée** : 300 ticks de temps d'exécution
4. **Finalisation** : Montée de niveau et notification `pie X Y R`

## Performances et optimisations

### Métriques supportées
Le serveur peut gérer :
- **100+ clients simultanés** sur du matériel moderne
- **1000+ ticks/seconde** pour des jeux rapides
- **Latence < 1ms** pour les commandes locales
- **Mémoire < 100MB** pour des cartes de taille standard

### Optimisations implémentées
- **Polling non-bloquant** avec `poll()` pour les I/O réseau
- **Files de commandes** pour le traitement asynchrone
- **Gestion mémoire optimisée** avec pools d'objets
- **Cache de la carte** pour éviter les recalculs fréquents
- **Buffers circulaires** pour la communication efficace

## Compilation et exécution

### Construction
```bash
cd src/Server
make                    # Construction standard
make debug             # Construction avec symboles de debug
make coverage          # Construction avec instrumentation de couverture
make clean             # Nettoyage des fichiers objets
```

### Paramètres de lancement
```bash
./zappy_server -p <port> -x <width> -y <height> -n <teams> -c <clients> -f <freq>
```

**Options avancées :**
- `--auto-start on|off` : Démarrage automatique
- `--display-eggs true|false` : Affichage des œufs
- `-v, --verbose` : Mode verbeux
- `--game_duration <time>` : Durée maximale de partie

## Fichiers importants

### Structure des sources
- `main.c` : Point d'entrée du serveur avec gestion des signaux
- `zappy.c` : Initialisation et configuration du serveur
- `parsing.c` : Analyse des arguments de ligne de commande
- `network/server_run.c` : Boucle principale et gestion des clients
- `network/connection.c` : Gestion des connexions TCP/IP
- `map/map_creation.c` : Création et initialisation de la carte
- `player/player_init.c` : Initialisation et gestion des joueurs
- `command/parse_command.c` : Parsing et exécution des commandes
- `buffer/circular_buffer.c` : Système de buffers circulaires
