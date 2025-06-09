# Documentation du Projet Zappy

## Introduction

Bienvenue dans la documentation du projet Zappy. Ce projet est un écosystème virtuel composé de plusieurs composants interagissant ensemble pour créer un environnement de simulation multi-agents.

## Structure de la documentation

La documentation est organisée par composants principaux :

### Composants principaux
- [Documentation du serveur](/src/Server/server_documentation.md) - Le cœur du projet gérant la logique du jeu
- [Documentation de l'IA](/src/AI/ai_documentation.md) - Les agents intelligents évoluant dans l'environnement 
- [Documentation de l'interface graphique](/src/GUI/gui_documentation.md) - La visualisation 3D de l'environnement
- [Documentation DLLoader](/src/Shared/DLLoader.md) - Système de chargement dynamique de bibliothèques

### Bibliothèques
- [Documentation RaylibCPP](/libs/RaylibCPP/raylibcpp_documentation.md) - Encapsulation C++ de Raylib
- [Documentation RayGUICPP](/libs/RayGUICPP/rayguicpp_documentation.md) - Encapsulation C++ de raygui
- [Histoire de raygui](/libs/RayGUICPP/raygui_history.md) - Historique du développement de raygui

## Guides d'utilisation
- [Guide de démarrage](docusaurus/docs/getting-started.md) - Comment installer et configurer le projet
- [Architecture du projet](docusaurus/docs/architecture.md) - Vue d'ensemble de l'architecture

## Protocoles de communication
- [Protocole Serveur-IA](docs/protocol_server_ai.md) - Communication entre le serveur et les IA
- [Protocole GUI](docs/protocol_gui.md) - Communication entre le serveur et l'interface graphique

## Comment contribuer

Pour contribuer à la documentation, veuillez suivre ces étapes :
1. Créez une branche à partir de `main`
2. Ajoutez ou modifiez la documentation
3. Soumettez une Pull Request

## Conventions

La documentation suit ces conventions :
- Utilisation de Markdown pour tous les documents
- Images stockées dans un dossier `images` à côté des fichiers correspondants
- Exemples de code inclus lorsque pertinent
- Liens vers d'autres parties de la documentation lorsque approprié
