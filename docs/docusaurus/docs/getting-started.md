---
sidebar_position: 1
---

# Getting Started with Zappy

Welcome to the Zappy documentation, a multiplayer game server with GUI client and AI bot.

## What is Zappy?

Zappy is a multi-component project developed in C/C++ and Python, including:

- A **server** written in C
- A **graphical interface (GUI)** to visualize the game
- **AI clients** to play automatically

## Prerequisites

To compile and run Zappy, you will need:

- A C/C++ compiler (GCC or Clang)
- Python 3.x (for utility scripts)
- Make

## Installation

Clone the repository and compile the project:

```bash
git clone https://github.com/Epitech/Zappy.git
cd Zappy
make
```

## Project Structure

The project is organized into several directories:

- `src/Server`: Server source code
- `src/GUI`: Graphical interface source code
- `src/AI`: AI clients source code
- `src/Shared`: Code shared between components
- `docs`: Project documentation
- `tests`: Unit and functional tests

## Basic Usage

### Starting the Server

```bash
./zappy_server -p <port> -x <width> -y <height> -n <team names> -c <max clients> -f <freq> [options]
```

Paramètres obligatoires :
- `-p` : Port d'écoute du serveur
- `-x` et `-y` : Dimensions de la carte (largeur et hauteur)
- `-n` : Noms des équipes (séparés par des espaces)
- `-c` : Nombre maximum de clients par équipe
- `-f` : Fréquence du serveur (unités de temps par seconde)

Paramètres optionnels :
- `--auto-start on|off` : Démarrage automatique du jeu
- `--display-eggs true|false` : Visibilité des œufs
- `--game_duration <time>` : Durée de la partie en secondes
- `-v` ou `--verbose` : Mode verbeux

Exemple :
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 5 -f 100 --auto-start on --display-eggs true
```

### Démarrage de l'interface graphique

```bash
./zappy_gui -p <port> -h <host>
```

Paramètres :
- `-p` : Port du serveur
- `-h` : Adresse du serveur

Exemple :
```bash
./zappy_gui -p 4242 -h localhost
```

### Démarrage de l'IA

```bash
./zappy_ai -p <port> -n <team name> -h <host>
```

Paramètres :
- `-p` : Port du serveur
- `-n` : Nom de l'équipe
- `-h` : Adresse du serveur (facultatif, par défaut localhost)

Exemple :
```bash
./zappy_ai -p 4242 -n team1
```

## Développement

### Compilation spécifique

Vous pouvez compiler chaque composant individuellement :

```bash
# Compiler uniquement le serveur
make zappy_server

# Compiler uniquement l'interface graphique
make zappy_gui

# Compiler uniquement l'IA
make zappy_ai
```

### Tests

```bash
# Exécuter tous les tests
make tests_run

# Tester le réseau GUI
make functionnal_tests

# Compiler en mode debug
make debug
```
