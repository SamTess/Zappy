---
sidebar_position: 2
---

# Architecture du projet

## Vue d'ensemble

Zappy est composé de trois composants principaux qui interagissent entre eux :

1. **Serveur** - Le cœur du jeu qui gère la logique, les règles et l'état du jeu
2. **GUI (Interface graphique)** - Représentation visuelle de l'état du jeu
3. **IA** - Clients automatisés qui jouent au jeu

## Communication entre composants

Les composants communiquent via des protocoles réseau TCP/IP :

```
┌────────┐    Commandes/Réponses    ┌──────────┐
│        │◄────────────────────────►│          │
│ Client │                          │          │
│  (IA)  │                          │          │
│        │                          │          │
└────────┘                          │          │
                                    │ Serveur  │
┌────────┐    Données de jeu        │          │
│        │◄────────────────────────►│          │
│  GUI   │                          │          │
│        │                          │          │
└────────┘                          └──────────┘
```

## Structure des fichiers

Le code source est organisé comme suit :

- `src/Server/` - Implémentation du serveur en C
  - `main.c` - Point d'entrée
  - `include/` - En-têtes du serveur
  - ...

- `src/GUI/` - Implémentation de l'interface graphique en C++
  - `main.cpp` - Point d'entrée
  - ...

- `src/AI/` - Implémentation du client IA
  - ...

- `src/Shared/` - Code partagé entre les composants
  - ...

## Technologies utilisées

- **Serveur** : C, sockets, gestion des threads
- **GUI** : C++, bibliothèque graphique
- **IA** : Algorithmes de prise de décision, stratégies
