---
sidebar_position: 1
---

# Zappy - Introduction

Bienvenue dans la documentation de **Zappy**, un écosystème virtuel multi-agents avec serveur de jeu, interface graphique et intelligence artificielle.

## Qu'est-ce que Zappy ?

Zappy est un projet multi-composants développé en C/C++ et Python, reproduisant un écosystème virtuel où des agents contrôlés par IA évoluent en collectant des ressources, en communiquant, et en réalisant des rituels d'élévation.

## Composants principaux

Le projet se compose de trois éléments principaux :

- **Serveur** : Écrit en C, il gère la logique du jeu, les règles, et l'état du monde
- **Interface Graphique** : Développée en C++, elle visualise l'état du jeu en temps réel
- **Clients IA** : Implémentés en Python, ils contrôlent automatiquement les joueurs pour accomplir des objectifs stratégiques

## Prérequis

Pour compiler et exécuter Zappy, vous aurez besoin de :

- Compilateur C/C++ (GCC ou Clang)
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
