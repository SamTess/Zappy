---
sidebar_position: 2
---

# Protocole Serveur-GUI

## Vue d'ensemble

Ce document détaille le protocole de communication entre le serveur Zappy et l'interface graphique (GUI). Le protocole est basé sur des échanges textuels permettant à l'interface de visualiser l'état du jeu en temps réel.

## Connexion

1. Le GUI se connecte au serveur via TCP/IP
2. Pour se différencier d'un client IA, le GUI envoie la commande : `GRAPHIC\n`
3. Le serveur répond avec les informations initiales sur la carte et les équipes

## Format des commandes

- Chaque commande est envoyée comme une chaîne de caractères terminée par `\n`
- Les réponses du serveur sont également terminées par `\n`
- Le GUI ne peut pas envoyer de commandes qui modifient le jeu, uniquement des requêtes d'information

## Informations initiales

Après la connexion avec la commande `GRAPHIC`, le serveur envoie :

1. Dimensions de la carte : `msz X Y\n` (largeur X, hauteur Y)
2. Unité de temps : `sgt T\n` (temps par action en ms)
3. Contenu de chaque case : `bct X Y q0 q1 q2 q3 q4 q5 q6\n` (pour chaque case)
4. Noms des équipes : `tna N\n` (pour chaque équipe)
5. Positions et états des joueurs : `pnw # X Y O L N\n` (pour chaque joueur)

## Commandes du GUI

Le GUI peut envoyer les commandes suivantes :

| Commande | Description | Réponse |
|----------|-------------|---------|
| `msz` | Dimensions de la carte | `msz X Y\n` |
| `bct X Y` | Contenu d'une case | `bct X Y q0 q1 q2 q3 q4 q5 q6\n` |
| `mct` | Contenu de toutes les cases | série de `bct` |
| `tna` | Noms des équipes | série de `tna N\n` |
| `ppo #` | Position d'un joueur | `ppo # X Y O\n` |
| `plv #` | Niveau d'un joueur | `plv # L\n` |
| `pin #` | Inventaire d'un joueur | `pin # X Y q0 q1 q2 q3 q4 q5 q6\n` |
| `sgt` | Unité de temps | `sgt T\n` |
| `sst T` | Modifier l'unité de temps | `sst T\n` |

## Notifications du serveur

Le serveur envoie des notifications des événements du jeu au GUI :

| Notification | Description |
|--------------|-------------|
| `pnw # X Y O L N` | Nouveau joueur |
| `pex #` | Expulsion d'un joueur |
| `pbc # M` | Diffusion d'un message |
| `pic X Y L #n` | Début d'incantation |
| `pie X Y R` | Fin d'incantation |
| `pfk #` | Ponte d'un œuf |
| `pdr # i` | Joueur lâche ressource |
| `pgt # i` | Joueur prend ressource |
| `pdi #` | Mort d'un joueur |
| `enw # X Y` | Création d'un œuf |
| `eht #e` | Éclosion d'un œuf |
| `edi #e` | Mort d'un œuf |
| `sgr` | Fin de partie |
| `smg M` | Message du serveur |

## Format des ressources

Le contenu des cases et inventaires (q0-q6) correspond aux ressources :
- q0 : nourriture
- q1 : linemate
- q2 : deraumere
- q3 : sibur
- q4 : mendiane
- q5 : phiras
- q6 : thystame

## Exemple d'échange

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

## Remarques

- Les coordonnées X et Y commencent à 0
- L'orientation O est un nombre de 1 à 4 (1: Nord, 2: Est, 3: Sud, 4: Ouest)
- Le niveau L des joueurs va de 1 à 8
- # représente l'ID d'un joueur
- #n représente une liste d'IDs de joueurs
- #e représente l'ID d'un œuf
- R est 0 (échec) ou 1 (succès) pour les incantations
