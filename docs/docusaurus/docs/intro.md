---
sidebar_position: 1
---

# Zappy - Introduction

Welcome to the documentation of **Zappy**, a multi-agent virtual ecosystem with game server, graphical interface and artificial intelligence.

## What is Zappy?

Zappy is a multi-component project developed in C/C++ and Python, reproducing a virtual ecosystem where AI-controlled agents evolve by collecting resources, communicating, and performing elevation rituals.

## Main Components

The project consists of three main elements:

- **Server**: Written in C, it manages game logic, rules, and world state
- **Graphical Interface**: Developed in C++, it visualizes the game state in real-time
- **AI Clients**: Implemented in Python, they automatically control players to achieve strategic objectives

## Prerequisites

To compile and run Zappy, you will need:

- C/C++ Compiler (GCC or Clang)
- Python 3.x
- Make
- Bibliothèques de développement graphique (pour le client GUI)

## Architecture simplifiée

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

## Comment utiliser cette documentation

Cette documentation est organisée par sections :

- **Premiers pas** : Installation et configuration du projet
- **Architecture** : Vue d'ensemble de la conception
- **Serveur** : Documentation du composant serveur
- **Interface graphique** : Documentation de la visualisation
- **IA** : Documentation des clients intelligents

Explorez la barre latérale pour accéder aux différentes sections.
