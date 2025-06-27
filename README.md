# 🎮 Zappy

<div align="center">
  <strong>Multiplayer AI-based network game</strong><br>
  Control autonomous agents, collect resources, evolve, and conquer a world governed by TCP and time units.
</div>

<div align="center">
  <img src="https://img.shields.io/badge/language-C%2FC%2B%2B%2FPython-blue" alt="Languages">
  <img src="https://img.shields.io/badge/protocol-TCP%2FIP-orange" alt="Protocol">
  <img src="https://img.shields.io/badge/platform-Linux-lightgrey" alt="Platform">
  <img src="https://img.shields.io/badge/version-4.0.0-green" alt="Version">
</div>

<div align="center">
  <img src="https://img.shields.io/github/license/SamTess/Zappy" alt="License">
  <img src="https://img.shields.io/github/actions/workflow/status/SamTess/Zappy/zappyCICD.yaml" alt="CI Status">
  <img src="https://img.shields.io/github/issues/SamTess/Zappy" alt="Issues">
  <img src="https://img.shields.io/github/forks/SamTess/Zappy" alt="Forks">
  <img src="https://img.shields.io/github/stars/SamTess/Zappy" alt="Stars">
</div>

<div align="center">
  <img src="https://img.shields.io/github/last-commit/SamTess/Zappy" alt="Last Commit">
  <img src="https://img.shields.io/github/contributors/SamTess/Zappy" alt="Contributors">
  <img src="https://img.shields.io/github/release/SamTess/Zappy" alt="Release">
  <img src="https://img.shields.io/github/downloads/SamTess/Zappy/total" alt="Downloads">
</div>

---

## 📑 Table of Contents

- [Overview](#-overview)
- [Architecture](#-architecture)
- [Interface utilisateur](#-interface-utilisateur)
- [Prérequis](#-prérequis)
- [Installation](#-installation)
- [Usage](#-usage)
- [Components](#-components)
  - [Server](#server)
  - [Artificial Intelligence](#artificial-intelligence)
- [Releases](#-releases)
- [Documentation](#-documentation)
- [Development](#-development)

---

## 🌐 Overview

Zappy is a multi-component project developed in C/C++ and Python, reproducing a virtual ecosystem where AI-controlled agents evolve by collecting resources, communicating, and performing elevation rituals. The game consists of three main elements :

- **Server** : Written in C, it manages the game logic, rules, and world state
- **Graphical Interface** : Developed in C++, it visualizes the game state in real-time
- **AI Clients** : Implemented in Python, they automatically control players to achieve strategic objectives

## 🏗 Architecture

The components communicate via TCP/IP network protocols:

```
┌────────┐    Commands/Responses    ┌──────────┐
│        │◄────────────────────────►│          │
│ Client │                          │          │
│  (AI)  │                          │          │
│        │                          │          │
└────────┘                          │          │
                                    │  Server  │
┌────────┐    Game data             │          │
│        │◄────────────────────────►│          │
│  GUI   │                          │          │
│        │                          │          │
└────────┘                          └──────────┘
```

## 💎 Interface Utilisateur

L'interface utilisateur du GUI a été conçue selon les principes de conception orientée objet avancés :

- **Pattern Factory** : Utilisation de `UIWindowFactory` pour la création et gestion des fenêtres
- **Hiérarchie de classes** : Organisation autour de l'interface `IUIWindow` et la classe abstraite `AUIWindow`
- **Modularité** : Chaque type de fenêtre est encapsulé dans sa propre classe spécialisée

Types de fenêtres disponibles :
- Journaux (`LogsWindow`) - affiche les messages système et événements
- Informations sur les tuiles (`TileInfoWindow`) - détails sur les cases sélectionnées
- Informations sur les joueurs (`PlayerInfoWindow`) - statistiques des joueurs
- Diffusions (`BroadcastsWindow`) - messages entre les joueurs
- Contrôles (`ControlsWindow`) - options de contrôle de la caméra et du jeu
- Informations temporelles (`TimeInfoWindow`) - statistiques de temps et fréquence
- Menu (`MenuWindow`) - paramètres globaux de l'application

Documentation détaillée disponible dans [docs/ui_architecture.md](docs/ui_architecture.md).

## 🔧 Prérequis

Pour compiler et exécuter Zappy, vous aurez besoin de :

### Prérequis système
- **Compilateur C/C++** (GCC 9.0+ ou Clang 10.0+)
- **Python 3.8+** avec pip3
- **Make 4.0+**
- **Git** (pour les sous-modules et Raylib)
- **CMake** (pour la compilation de Raylib depuis les sources si nécessaire)

### Installation automatique des dépendances
```bash
# Installation automatique de toutes les dépendances
make install_requirements
```

Cette commande installe automatiquement :
- Raylib (via APT ou compilation depuis GitHub)
- Dépendances Python pour l'IA (cryptography, dotenv)
- Initialisation des sous-modules Git

### Dépendances système pour Raylib
Le projet nécessite les bibliothèques suivantes pour l'interface graphique :
- **OpenGL** : `libgl1-mesa-dev`
- **X11** : `libx11-dev`, `libxrandr-dev`, `libxinerama-dev`, `libxcursor-dev`, `libxi-dev`, `libxext-dev`
- **Outils de build** : `build-essential`, `cmake`, `pkg-config`

```bash
# Installation manuelle des dépendances système (Ubuntu/Debian)
sudo apt update
sudo apt install -y build-essential cmake git pkg-config
sudo apt install -y libgl1-mesa-dev libx11-dev libxrandr-dev libxinerama-dev
sudo apt install -y libxcursor-dev libxi-dev libxext-dev
```

### Dépendances Python pour l'IA
Le fichier `src/AI/requirements.txt` contient :
- **cryptography** : Chiffrement AES pour les communications sécurisées entre agents
- **dotenv** : Gestion des variables d'environnement

```bash
# Installation manuelle (si make install_requirements échoue)
cd src/AI && pip3 install -r requirements.txt
```

## 💻 Installation

```bash
# Clone the repository
git clone https://github.com/Epitech/Zappy.git
cd Zappy

# Compile all components
make

# Or compile specific components
make zappy_server
make zappy_gui
make zappy_ai
```

## 🚀 Usage

### Démarrer le serveur

```bash
./zappy_server -p <port> -x <width> -y <height> -n <team names> -c <max clients> -f <freq>
```

**Paramètres obligatoires :**
- `-p <port>` : Port d'écoute du serveur
- `-x <width>` : Largeur de la carte (minimum 10)
- `-y <height>` : Hauteur de la carte (minimum 10)
- `-n <team1> [team2] ...` : Noms des équipes (séparés par des espaces)
- `-c <max_clients>` : Nombre maximum de clients par équipe
- `-f <freq>` : Fréquence du serveur (unités de temps par seconde)

**Paramètres optionnels :**
- `--auto-start on|off` : Démarrage automatique (par défaut: off)
- `--display-eggs true|false` : Affichage des œufs dans les logs (par défaut: false)
- `-v` ou `--verbose` : Mode verbeux pour plus de logs
- `--game_duration <time>` : Durée maximale de la partie en secondes

**Exemple :**
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 team3 -c 5 -f 100 --auto-start on
```

### Démarrer l'interface graphique

```bash
./zappy_gui -p <port> -h <host>
```

**Paramètres :**
- `-p <port>` : Port du serveur
- `-h <host>` : Adresse du serveur (par défaut : localhost)

**Exemple :**
```bash
./zappy_gui -p 4242 -h localhost
```

**Contrôles de l'interface :**
- **Caméra** : Clic gauche + glisser pour tourner, molette pour zoomer
- **Vue** : Touches WASD pour déplacer la caméra
- **Interface** : Panneaux d'information en temps réel sur les équipes et ressources
- **Sélection** : Clic sur une tuile ou un joueur pour afficher les détails
- **Menu** : Accès aux paramètres audio, visuels et de jeu

### Démarrer l'IA

#### Prérequis
Avant de démarrer l'IA, assurez-vous d'avoir activé l'environnement virtuel Python :
```bash
source venv/bin/activate
```

#### Commande de démarrage

```bash
./zappy_ai -p <port> -n <team name> [-h <host>] [--performance]
```

**Paramètres :**
- `-p <port>` : Port du serveur
- `-n <team>` : Nom de l'équipe
- `-h <host>` : Adresse du serveur (optionnel, par défaut : localhost)
- `--performance` : Mode performance pour les simulations (optionnel)

**Exemple :**
```bash
./zappy_ai -p 4242 -n team1 --performance
```

**Comportements de l'IA :**
- **Collecte automatique** : Exploration et collecte optimisée des ressources
- **Communication inter-agents** : Coordination via broadcast chiffré
- **Évolution** : Exécution automatique des incantations pour monter de niveau
- **Stratégie adaptative** : Changement de comportement selon le contexte
- **Gestion intelligente** : Optimisation des trajets et gestion des priorités

#### Multi-agents
Le système lance automatiquement plusieurs agents pour l'équipe spécifiée. Le nombre d'agents est configurable dans le code (par défaut : 10 agents par équipe).

## 📦 Components

### Server

The Zappy server is the core of the system, developed in C for optimal performance.

**Main features:**
- Game map and resource management
- Real-time processing of client commands
- Physical and temporal simulation of the world
- TCP/IP communication protocols
- Multi-threaded management for performance

**Modular architecture:**
- `network/` : Connection and protocol management
- `map/` : Map and tiles system
- `player/` : Player management and their states
- `command/` : Command processing
- `buffer/` : Buffer system for communication

### Graphical Interface

3D interface developed in C++ using Raylib for real-time visualization.

**Features:**
- 3D rendering of the map and entities
- Intuitive user interface with RayGUI
- Free camera system with smooth controls
- Real-time display of statistics
- Texture and 3D model management

**Components:**
- `renderer/` : 3D rendering engine
- `cameraController/` : Camera management
- `textureManager/` : Loading and management of textures
- `network/` : Communication with the server
- `graphicalContext/` : Graphic context and state

### Artificial Intelligence

Autonomous bots developed in Python with a modular and scalable architecture.

**Characteristics:**
- Adaptive behavioral architecture
- Secure communication system between agents
- Exploration and optimization algorithms
- Evolution and cooperation strategies
- Advanced logging for debugging and analysis
./zappy_ai -p 4242 -n team1

## 📦 Components

### Server

Le serveur Zappy est développé en C pour des performances optimales et gère :

**Fonctionnalités principales :**
- Gestion de la carte du jeu et des ressources avec distribution automatique
- Connexions multiples (clients IA et interface graphique) via TCP/IP
- Logique du jeu et application des règles de l'univers virtuel
- Gestionnaire de temps avec système de ticks configurables
- Protocoles de communication textuels distincts pour IA et GUI
- Rituels d'élévation avec vérification des conditions et coordination des joueurs
- Système de ponte d'œufs et génération de nouveaux agents

**Architecture modulaire :**
- `network/` : Gestion des connexions TCP/IP et communication
- `map/` : Système de carte avec tuiles et ressources
- `player/` : Gestion des joueurs, inventaires et actions
- `command/` : Processeur de commandes avec file d'attente
- `buffer/` : Système de buffers circulaires pour la communication
- `graphical_command/` : Commandes spécialisées pour l'interface graphique

### Interface Graphique

L'interface graphique est développée en C++ avec Raylib et offre :

**Visualisation 3D avancée :**
- Rendu en temps réel de la carte du jeu avec système de niveaux de détail (LOD)
- Affichage des joueurs avec modèles 3D animés et orientations
- Visualisation des ressources sur chaque tuile avec indicateurs visuels
- Système de caméra libre avec contrôles fluides et transitions
- Effets visuels pour les événements spéciaux (incantations, éjections, morts)
- Skybox et éclairage 3D pour une immersion accrue

**Interface utilisateur modulaire :**
- Système de fenêtres basé sur le pattern Factory (`UIWindowFactory`)
- Panneaux d'information spécialisés (joueurs, tuiles, équipes, temps)
- Contrôles interactifs pour la navigation et l'observation
- Système de logging des événements en temps réel
- Menu de configuration avec paramètres audio et visuels
- Architecture extensible permettant l'ajout facile de nouvelles fenêtres

**Système de chargement dynamique :**
- DLLoader pour les bibliothèques graphiques et d'interface
- Support des plugins avec interfaces `IGraphicsLib` et `IGuiLib`
- Gestion des textures et modèles 3D avec cache optimisé

### Intelligence Artificielle

L'IA des clients développée en Python comprend :

**Architecture comportementale adaptative :**
- Machine à états hiérarchique avec transitions intelligentes
- Modules de perception pour l'analyse de l'environnement (`Look`, `Inventory`)
- Algorithmes de prise de décision basés sur la priorité des objectifs
- Système de gestion des ressources avec optimisation des trajets
- Coordination multi-agents via communication chiffrée AES

**Stratégies et comportements :**
- Exploration intelligente avec cartographie interne de l'univers
- Collecte optimisée des ressources avec évitement des conflits
- Stratégies de regroupement pour les rituels d'élévation
- Comportements d'urgence en cas de famine ou de danger
- Système de rôles dynamiques (mineur, combattant, reproducteur)

**Communication et collaboration :**
- Protocole de broadcast sécurisé entre agents de la même équipe
- Échange d'informations sur les positions des ressources
- Coordination pour les rituels d'incantation multi-joueurs
- Système de chiffrement des messages pour éviter l'espionnage

**Fonctionnalités avancées :**
- Algorithme de pathfinding A* optimisé pour les cartes toriques
- Représentation interne du monde avec mise à jour temps réel
- Gestion intelligente du temps et des priorités d'actions
- Système de logging complet pour l'analyse et le débogage
- Mode performance pour les simulations à grande échelle

## 🚀 Releases

The project uses an automated release system via GitHub Actions with semantic versioning :

### 📦 Download a release
Go to the [releases page](../../releases) to download the latest version.

### 🔄 Create a new release
Releases are automatically created via GitHub Actions :

1. **Access Actions** : GitHub → Actions tab → "🚀 Release Zappy" workflow
2. **Choose the version type** :
   - `nouvelle-version` : Major version (v1.0.0 → v2.0.0)
   - `nouvelle-feature` : New feature (v1.0.0 → v1.1.0)  
   - `resolution-bug` : Bug fix (v1.0.0 → v1.0.1)
3. **Run the workflow** : The system automatically generates the version, compiles the project, and creates the release

Each release contains :
- All binaries (server, GUI, AI)
- Static and dynamic libraries
- Automatic installation script
- Documentation and changelog

📖 **Detailed guide** : [docs/release-workflow.md](docs/release-workflow.md)

## 📚 Documentation

Detailed documentation is available in the `docs/` folder :
- Architecture and design : `docs/architecture.md`
- Getting started guide : `docs/getting-started.md`
- Server documentation : `docs/server/`
- Graphical interface documentation : `docs/gui/`
- AI documentation : `docs/ai/`

The Docusaurus documentation can be viewed locally :
```bash
cd docs/docusaurus
yarn start
```

## 🛠 Development

### Project structure

```
├── src/
│   ├── Server/    # Server implementation in C
│   ├── GUI/       # Graphical interface in C++
│   ├── AI/        # AI client in Python
│   └── Shared/    # Code shared between components
├── libs/          # External libraries (RaylibCPP, RayGUICPP)
├── docs/          # Project documentation
├── tests/         # Unit and functional tests
├── assets/        # Graphic resources
└── tools/         # Development tools
```

### Tests

```bash
# Run all tests
make tests_run

# Test the GUI network
make functionnal_tests

# Compile in debug mode
make debug
```

---

<div align="center">
  <p>Developed with ❤️ by the Zappy team</p>
  <p>© 2025 Epitech</p>
</div>

