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

Exemple :
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 5 -f 100
```

### Démarrage du client GUI

```bash
./zappy_client -p <port> -h <host>
```

Exemple :
```bash
./zappy_client -p 4242 -h localhost
```
