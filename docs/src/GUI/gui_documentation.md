# Documentation de l'Interface Graphique (GUI) Zappy

## Vue d'ensemble

L'interface graphique Zappy est responsable de la visualisation en 3D de l'environnement de jeu, permettant d'observer en temps réel :
- Le terrain et ses ressources
- Les joueurs et leurs actions
- Les événements du jeu (incantations, éclosion d'œufs, etc.)

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

Ce système permet de changer facilement d'implémentation graphique sans modifier le code principal.

## Communication avec le serveur

Le GUI communique avec le serveur via un protocole spécifique décrit dans la documentation "GUI Protocol". Les principales commandes incluent :
- Informations sur la carte et les cellules
- État des joueurs (position, orientation, niveau)
- Ressources disponibles
- Événements (incantations, éclosions, etc.)

## Visualisation 3D

L'interface utilise RaylibCPP pour le rendu 3D avec les caractéristiques suivantes :
- Rendu de la carte comme une grille 3D
- Modèles 3D pour les joueurs et ressources
- Caméra libre ou modes de suivi
- Effets visuels pour les événements spéciaux

## Interface utilisateur

L'interface utilisateur utilise RayGUICPP et propose :
- Informations sur les joueurs sélectionnés
- Statistiques du jeu en temps réel
- Configuration de la visualisation
- Options de caméra

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
