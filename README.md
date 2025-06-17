# 🎮 Zappy

<div align="center">
  <strong>Jeu multijoueur basé sur l'IA en réseau</strong><br>
  Contrôlez des agents autonomes, collectez des ressources, évoluez et conquérez un monde régi par TCP et unités temporelles.
</div>

<div align="center">
  <img src="https://img.shields.io/badge/langage-C%2FC%2B%2B%2FPython-blue" alt="Languages">
  <img src="https://img.shields.io/badge/architecture-client%2Fserver-green" alt="Architecture">
  <img src="https://img.shields.io/badge/protocole-TCP%2FIP-orange" alt="Protocol">
</div>

---

## 📑 Table des matières

- [Vue d'ensemble](#-vue-densemble)
- [Architecture](#-architecture)
- [Interface utilisateur](#-interface-utilisateur)
- [Prérequis](#-prérequis)
- [Installation](#-installation)
- [Utilisation](#-utilisation)
- [Composants](#-composants)
  - [Serveur](#serveur)
  - [Interface Graphique](#interface-graphique)
  - [Intelligence Artificielle](#intelligence-artificielle)
- [Releases](#-releases)
- [Documentation](#-documentation)
- [Développement](#-développement)

---

## 🌐 Vue d'ensemble

Zappy est un projet multi-composants développé en C/C++ et Python, reproduisant un écosystème virtuel où des agents contrôlés par IA évoluent en collectant des ressources, en communiquant, et en réalisant des rituels d'élévation. Le jeu est composé de trois éléments principaux :

- **Serveur** : Écrit en C, il gère la logique du jeu, les règles, et l'état du monde
- **Interface Graphique** : Développée en C++, elle visualise l'état du jeu en temps réel
- **Clients IA** : Implémentés en Python, ils contrôlent automatiquement les joueurs pour accomplir des objectifs stratégiques

## 🏗 Architecture

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

- Compilateur C/C++ (GCC ou Clang)
- Python 3.x
- Make
- Bibliothèques de développement graphique (pour le client GUI)

## 💻 Installation

```bash
# Cloner le dépôt
git clone https://github.com/Epitech/Zappy.git
cd Zappy

# Compiler tous les composants
make

# Ou compiler des composants spécifiques
make zappy_server
make zappy_gui
make zappy_ai
```

## 🚀 Utilisation

### Démarrer le serveur

```bash
./zappy_server -p <port> -x <width> -y <height> -n <team names> -c <max clients> -f <freq>
```

Exemple :
```bash
./zappy_server -p 4242 -x 20 -y 20 -n team1 team2 -c 5 -f 100
```

### Démarrer l'interface graphique

```bash
./zappy_gui -p <port> -h <host>
```

Exemple :
```bash
./zappy_gui -p 4242 -h localhost
```

### Démarrer l'IA

```bash
./zappy_ai -p <port> -n <team name>
```

Exemple :
```bash
./zappy_ai -p 4242 -n team1
```

## 📦 Composants

### Serveur

Le serveur est le composant central qui gère :
- La carte de jeu et les ressources
- Les connexions des clients (IA et GUI)
- La logique et les règles du jeu
- La gestion du temps et des événements
- Le protocole de communication
- Les rituels d'élévation et évolution des joueurs

### Interface Graphique

L'interface graphique offre :
- Visualisation en temps réel de la carte du jeu
- Affichage des joueurs et leurs inventaires
- Visualisation des ressources sur chaque case
- Suivi des événements (incantations, reproductions, etc.)
- Contrôles interactifs pour la navigation et observation

### Intelligence Artificielle

L'IA des clients comprend :
- Modules de perception pour l'analyse de l'environnement
- Algorithmes de décision stratégique
- Gestion des ressources et planification
- Communication et coordination entre agents
- Stratégies pour les rituels d'élévation

## 🚀 Releases

Le projet utilise un système de release automatisé via GitHub Actions avec versioning sémantique :

### 📦 Télécharger une release
Rendez-vous sur la [page des releases](../../releases) pour télécharger la dernière version.

### 🔄 Créer une nouvelle release
Les releases sont créées automatiquement via GitHub Actions :

1. **Accéder aux Actions** : GitHub → onglet Actions → workflow "🚀 Release Zappy"
2. **Choisir le type de version** :
   - `nouvelle-version` : Version majeure (v1.0.0 → v2.0.0)
   - `nouvelle-feature` : Nouvelle fonctionnalité (v1.0.0 → v1.1.0)  
   - `resolution-bug` : Correction de bug (v1.0.0 → v1.0.1)
3. **Lancer le workflow** : Le système génère automatiquement la version, compile le projet et crée la release

Chaque release contient :
- Tous les binaires (serveur, GUI, IA)
- Bibliothèques statiques et dynamiques
- Script d'installation automatique
- Documentation et changelog

📖 **Guide détaillé** : [docs/release-workflow.md](docs/release-workflow.md)

## 📚 Documentation

Une documentation détaillée est disponible dans le dossier `docs/` :
- Architecture et conception : `docs/architecture.md`
- Guide de démarrage : `docs/getting-started.md`
- Documentation du serveur : `docs/server/`
- Documentation de l'interface graphique : `docs/gui/`
- Documentation de l'IA : `docs/ai/`

La documentation Docusaurus peut être consultée localement :
```bash
cd docs/docusaurus
yarn start
```

## 🛠 Développement

### Structure du projet

```
├── src/
│   ├── Server/    # Implémentation du serveur en C
│   ├── GUI/       # Interface graphique en C++
│   ├── AI/        # Client IA en Python
│   └── Shared/    # Code partagé entre les composants
├── libs/          # Bibliothèques externes (RaylibCPP, RayGUICPP)
├── docs/          # Documentation du projet
├── tests/         # Tests unitaires et fonctionnels
├── assets/        # Ressources graphiques
└── tools/         # Outils de développement
```

### Tests

```bash
# Exécuter tous les tests
make tests_run

# Tester le réseau GUI
make functionnal_tests

# Compiler en mode debug
make debug
```

---

<div align="center">
  <p>Développé avec ❤️ par l'équipe Zappy</p>
  <p>© 2025 Epitech</p>
</div>

