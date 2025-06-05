---
sidebar_position: 1
---

# Interface graphique (GUI)

L'interface graphique permet de visualiser en temps réel l'état du jeu Zappy, les joueurs, les ressources et les actions.

## Fonctionnalités

- Affichage de la carte de jeu en 2D ou 3D
- Visualisation des joueurs et de leur orientation
- Affichage des ressources présentes sur chaque case
- Visualisation des actions des joueurs (mouvement, prise d'objet, etc.)
- Interface pour surveiller l'état des équipes
- Visualisation des incantations
- Contrôles pour ajuster la vitesse de simulation

## Configuration

Le client GUI accepte les paramètres suivants :

| Paramètre | Description                    | Valeur par défaut |
|-----------|--------------------------------|-------------------|
| `-p`      | Numéro de port du serveur      | 4242              |
| `-h`      | Adresse du serveur             | localhost         |

## Protocole de communication

Le client GUI se connecte au serveur et reçoit des informations sur l'état du jeu. Il utilise un protocole texte sur TCP. Le client GUI ne peut pas envoyer de commandes qui affectent le jeu, il est uniquement un observateur.

## Interface utilisateur

L'interface utilisateur comprend plusieurs sections :

1. **Vue de la carte** : Affiche la grille de jeu avec les joueurs et les ressources
2. **Panneau d'information** : Affiche des détails sur l'élément sélectionné
3. **Contrôles de visualisation** : Permettent de zoomer, faire pivoter et déplacer la vue
4. **Liste des équipes** : Affiche les équipes et leurs statistiques
5. **Timeline** : Permet de suivre l'historique des événements

## Architecture

Le GUI est développé en C++ et utilise une architecture modulaire basée sur le chargement dynamique de bibliothèques (DLLoader) :

- **Core** : Boucle principale, gestion d'événements, coordinateur
- **NetworkManager** : Communication avec le serveur
- **Renderer** : Affichage graphique 3D et 2D
- **CameraController** : Gestion de la caméra et vues
- **GUI Components** : Éléments d'interface utilisateur

## Système de chargement dynamique (DLLoader)

Le GUI utilise un système de chargement dynamique de bibliothèques pour permettre l'utilisation de différentes implémentations graphiques :
- Interface `IGraphicsLib` : Définit les fonctions attendues pour les bibliothèques graphiques
- Interface `IGuiLib` : Définit les fonctions attendues pour les bibliothèques d'interface utilisateur
- Gestionnaire `LibraryManager` : Permet de charger dynamiquement les bibliothèques

## Visualisation 3D

L'interface utilise RaylibCPP pour le rendu 3D avec les caractéristiques suivantes :
- Rendu de la carte comme une grille 3D
- Modèles 3D pour les joueurs et ressources
- Caméra libre ou modes de suivi
- Effets visuels pour les événements spéciaux

## Compilation et Exécution

```bash
# Compilation
make -C src/GUI

# Exécution
./zappy_gui -p <port> -h <host>
```

## Fichiers importants
- `main.cpp` : Point d'entrée du programme
- `GameLoop.cpp` : Boucle principale du jeu
- `network/networkManager/NetworkManager.cpp` : Gestion de la communication réseau
- `renderer/Renderer.cpp` : Rendu graphique
- `cameraController/CameraController.cpp` : Contrôle de la caméra
