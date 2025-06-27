# Zappy Graphical User Interface (GUI) Documentation

## Vue d'ensemble

L'interface graphique Zappy est responsable de la visualisation 3D de l'environnement de jeu, permettant l'observation en temps réel de :
- Le terrain et ses ressources avec indicateurs visuels
- Les joueurs et leurs actions avec modèles 3D animés
- Les événements de jeu (incantations, éclosions d'œufs, combats, etc.)
- Les statistiques d'équipes et informations temporelles
- Les effets visuels et animations spécialisées

## Architecture avancée

L'interface graphique est développée en C++ et utilise une architecture modulaire basée sur le chargement dynamique de bibliothèques (DLLoader) :

### Composants principaux
- **GameLoop** : Boucle de jeu principale avec gestion des événements
- **NetworkManager** : Communication bidirectionnelle avec le serveur
- **Renderer** : Pipeline de rendu 3D avec support des effets visuels
- **MapRenderer** : Rendu spécialisé de la carte avec niveaux de détail (LOD)
- **CameraController** : Gestion de caméra avancée avec transitions fluides
- **UserInterface** : Système d'interface utilisateur modulaire
- **GameController** : Coordination des composants et gestion d'état

### Système de chargement dynamique (DLLoader)

L'interface graphique utilise un système de chargement dynamique permettant l'utilisation de différentes implémentations graphiques :

**Interfaces principales :**
- **IGraphicsLib** : Définit les fonctions attendues pour les bibliothèques graphiques
- **IGuiLib** : Définit les fonctions attendues pour les bibliothèques d'interface utilisateur
- **LibraryManager** : Permet le chargement dynamique des bibliothèques

**Avantages :**
- Commutation facile entre implémentations graphiques
- Modularité sans modification du code principal
- Support de plugins graphiques personnalisés

### Architecture de l'interface utilisateur

Le système d'interface utilisateur repose sur plusieurs patterns de conception :

**Pattern Factory (`UIWindowFactory`) :**
- Création centralisée des fenêtres spécialisées
- Gestion du cycle de vie des composants UI
- Extension facile pour nouveaux types de fenêtres

**Hiérarchie de classes :**
```
IUIWindow (Interface)
    |
    +-- AUIWindow (Classe abstraite)
            |
            +-- TileInfoWindow      (Informations sur les tuiles)
            +-- PlayerInfoWindow    (Informations sur les joueurs)
            +-- TimeInfoWindow      (Informations temporelles)
            +-- MenuWindow          (Menu principal)
            +-- MapInfoWindow       (Informations sur la carte)
```

## Communication avec le serveur

L'interface graphique communique avec le serveur via un protocole spécialisé :

### Commandes GUI
- **Identification** : `GRAPHIC\n` pour s'identifier comme client graphique
- **Informations initiales** : Réception automatique de l'état complet du jeu
- **Contrôle temporel** : `sst T\n` pour modifier la fréquence du serveur
- **Requêtes spécifiques** : `pin #\n`, `ppo #\n`, `plv #\n` pour des informations de joueurs

### Notifications du serveur
- **Carte** : `msz X Y\n`, `bct X Y q0 q1 q2 q3 q4 q5 q6\n`
- **Joueurs** : `pnw # X Y O L N\n`, `ppo # X Y O\n`, `plv # L\n`
- **Événements** : `pic X Y L #1 #2 ...\n`, `pie X Y R\n`, `pex #\n`
- **Équipes** : `tna N\n`, `seg N\n`

## Visualisation 3D avancée

L'interface utilise RaylibCPP pour un rendu 3D sophistiqué :

### Fonctionnalités de rendu
- **Carte 3D** : Rendu de la carte comme grille 3D avec ajustement automatique
- **Modèles 3D** : Joueurs et ressources avec modèles personnalisés (.glb)
- **Système de caméra** : Caméra libre avec contrôles intuitifs et modes de suivi
- **Effets visuels** : Particules, animations et effets spéciaux pour les événements
- **Éclairage** : Système d'éclairage dynamique avec skybox
- **Optimisations** : Level of Detail (LOD) et frustum culling pour les grandes cartes

### Système d'effets visuels
- **ParticleSystem** : Gestionnaire de particules pour effets spéciaux
- **EjectionAnimationManager** : Animations d'éjection de joueurs
- **DeathAnimationManager** : Animations de mort avec effets visuels
- **BroadcastEffect** : Effets visuels pour les messages broadcast

## Interface utilisateur complète

L'interface utilisateur utilise RayGUICPP et offre :

### Fenêtres spécialisées
- **TileInfoWindow** : Informations détaillées sur les tuiles sélectionnées
- **PlayerInfoWindow** : Statistiques complètes des joueurs avec commandes
- **TimeInfoWindow** : Informations temporelles et FPS en temps réel
- **MenuWindow** : Configuration audio, visuelle et de jeu
- **MapInfoWindow** : Informations générales sur la carte et les équipes

### Fonctionnalités interactives
- **Sélection** : Clic sur tuiles et joueurs pour affichage d'informations
- **Contrôles de caméra** : Rotation, zoom et déplacement intuitifs
- **Configuration temps réel** : Modification des paramètres sans redémarrage
- **Tracking de joueurs** : Suivi automatique de joueurs spécifiques

## Gestion des ressources

### TextureManager et ModelManager
- **Cache optimisé** : Chargement et mise en cache des textures et modèles
- **Formats supportés** : Support des formats .glb, .jpg, .png
- **Gestion mémoire** : Libération automatique des ressources inutilisées

### Assets du jeu
- **Modèles 3D** : Collection de modèles pour joueurs et objets
- **Textures** : Skybox et textures de terrain
- **Audio** : Musique de fond et effets sonores

## Compilation et exécution

### Construction
```bash
# Compilation complète
make -C src/GUI

# Modes spéciaux
make -C src/GUI debug      # Mode debug avec symboles
make -C src/GUI coverage   # Couverture de code
make -C src/GUI clean      # Nettoyage
```

### Dépendances
- **Raylib** : Bibliothèque graphique 3D
- **OpenGL** : Rendu graphique accéléré
- **Bibliothèques systèmes** : X11, pthread, dl, m

### Exécution
```bash
./zappy_gui -p <port> -h <host>
```

**Contrôles de l'interface :**
- **Caméra** : Clic gauche + glisser pour rotation, molette pour zoom
- **Mouvement** : Touches WASD pour déplacement de la caméra
- **Sélection** : Clic sur éléments pour affichage d'informations
- **Interface** : Fenêtres redimensionnables et repositionnables

## Fichiers importants

### Structure principale
- `main.cpp` : Point d'entrée avec parsing des arguments
- `GameLoop.cpp` : Boucle de jeu principale et coordination
- `GameLoopUI.cpp` : Interface utilisateur de la boucle de jeu

### Réseau et communication
- `network/networkManager/NetworkManager.cpp` : Gestion de la communication serveur
- `network/protocol/ProtocolParser.cpp` : Analyseur de protocole GUI
- `network/buffer/CircularBuffer.cpp` : Buffers de communication

### Rendu et graphisme
- `renderer/Renderer.cpp` : Moteur de rendu principal
- `renderer/MapRenderer.cpp` : Rendu spécialisé de la carte
- `renderer/Skybox.cpp` : Gestion de la skybox
- `renderer/ParticleSystem.cpp` : Système de particules

### Interface utilisateur
- `ui/UserInterface.cpp` : Interface utilisateur principale
- `ui/UIWindowFactory.cpp` : Factory pour les fenêtres
- `ui/windows/*/` : Fenêtres spécialisées

### Contrôles et gestion
- `cameraController/CameraController.cpp` : Contrôle de caméra avancé
- `gameController/GameController.cpp` : Coordination des composants
- `textureManager/ModelManager.cpp` : Gestion des modèles 3D
