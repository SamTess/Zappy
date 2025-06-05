---
sidebar_position: 1
---

# Premiers pas avec Zappy

Bienvenue dans la documentation de Zappy, un serveur de jeu multijoueur avec client GUI et bot IA.

## Qu'est-ce que Zappy ?

Zappy est un projet multi-composants développé en C/C++ et Python, comprenant :

- Un **serveur** écrit en C
- Une **interface graphique (GUI)** pour visualiser le jeu
- Des **clients IA** pour jouer automatiquement

## Prérequis

Pour compiler et exécuter Zappy, vous aurez besoin de :

- Un compilateur C/C++ (GCC ou Clang)
- Python 3.x (pour les scripts utilitaires)
- Make

## Installation

Clonez le dépôt et compilez le projet :

```bash
git clone https://github.com/Epitech/Zappy.git
cd Zappy
make
```

## Structure du projet

Le projet est organisé en plusieurs répertoires :

- `src/Server` : Code source du serveur
- `src/GUI` : Code source de l'interface graphique
- `src/AI` : Code source des clients IA
- `src/Shared` : Code partagé entre les composants
- `docs` : Documentation du projet
- `tests` : Tests unitaires et fonctionnels

## Utilisation de base

### Démarrage du serveur

```bash
./zappy_server -p <port> -x <width> -y <height> -n <team names> -c <max clients> -f <freq>
```

Paramètres :
- `-p` : Port d'écoute du serveur
- `-x` et `-y` : Dimensions de la carte (largeur et hauteur)
- `-n` : Noms des équipes (séparés par des espaces)
- `-c` : Nombre maximum de clients par équipe
- `-f` : Fréquence du serveur (unités de temps par seconde)

Exemple :
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 5 -f 100
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
