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

## Bibliothèques utilisées

Le projet utilise plusieurs bibliothèques spécifiques :

### RaylibCPP
Une encapsulation C++ de la bibliothèque Raylib pour faciliter le rendu graphique 3D dans l'interface GUI. La bibliothèque fournit des classes pour :
- Gestion des fenêtres
- Rendu de formes et textures
- Gestion des entrées utilisateur
- Manipulation de la caméra 3D

### RayGUICPP
Une encapsulation C++ de la bibliothèque raygui pour créer l'interface utilisateur du client GUI. Elle offre :
- Des widgets d'interface utilisateur
- Des contrôles interactifs
- Des boîtes de dialogue
- Des layouts et groupes

### DLLoader
Un système de chargement dynamique de bibliothèques qui permet :
- Le chargement de bibliothèques à l'exécution
- L'abstraction de l'implémentation graphique
- L'interchangeabilité des modules

## Flux de données

Les données circulent entre les composants comme suit :

1. **Serveur → IA** : État du jeu, résultats des actions
2. **IA → Serveur** : Commandes et actions du joueur  
3. **Serveur → GUI** : État complet du jeu pour visualisation
4. **GUI → Serveur** : Requêtes d'information uniquement (pas d'action)

## Mécanismes de jeu principaux

### Cycle de vie du joueur
1. Connexion au serveur et rejoindre une équipe
2. Exploration et collecte de ressources
3. Évolution par incantation
4. Mort éventuelle par manque de nourriture

### Génération et distribution des ressources
Les ressources sont générées aléatoirement sur la carte avec une distribution qui dépend du type de ressource. La régénération se produit périodiquement en fonction de la fréquence du serveur.

### Système d'énergie
Les joueurs consomment de la nourriture au fil du temps. Si un joueur n'a plus de nourriture, il meurt et est supprimé du jeu.

### Communication entre joueurs
Les joueurs peuvent communiquer entre eux via un système de diffusion qui permet à un joueur d'envoyer un message à tous les autres joueurs. Les messages sont reçus avec une indication de direction pour simuler une communication réaliste.
