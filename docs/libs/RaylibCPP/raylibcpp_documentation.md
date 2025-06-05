# Documentation de RaylibCPP

## Vue d'ensemble

RaylibCPP est une encapsulation C++ de la bibliothèque Raylib, conçue pour le projet Zappy. Elle permet d'utiliser les fonctionnalités de Raylib de manière orientée objet et avec une syntaxe moderne C++.

## Fonctionnalités

RaylibCPP fournit des classes et méthodes pour :
- **Gestion de fenêtre** : Création, configuration et fermeture de fenêtres
- **Rendu 2D/3D** : Dessin de formes, modèles et textures
- **Gestion de caméra** : Caméras 2D et 3D avec différents modes
- **Gestion d'entrées** : Clavier, souris et autre périphériques
- **Audio** : Lecture de sons et musiques
- **Textures et images** : Chargement et manipulation d'images et textures
- **Modèles 3D** : Chargement et rendu de modèles 3D

## Structure

La bibliothèque est divisée en plusieurs modules :
- **Window** : Gestion de la fenêtre et du contexte de rendu
- **Shape** : Primitives de dessin 2D et 3D
- **Texture** : Gestion des textures et images
- **Camera** : Contrôle des caméras
- **Input** : Gestion des entrées utilisateur
- **Audio** : Lecture et contrôle audio
- **Font** : Chargement et rendu de texte

## Utilisation

### Initialisation

```cpp
#include "RayLib.hpp"

int main() {
    // Création d'une fenêtre
    RaylibCPP::Window window(800, 600, "Mon Application");
    
    // Initialisation de la caméra 3D
    RaylibCPP::Camera camera(RaylibCPP::Camera::Mode::CAMERA_FREE);
    camera.setPosition({10.0f, 10.0f, 10.0f});
    
    while (!window.shouldClose()) {
        // Logique et rendu
        window.beginDrawing();
        window.clearBackground(RaylibCPP::Color::RAYWHITE);
        
        window.begin3DMode(camera);
        // Rendu 3D ici
        window.end3DMode();
        
        // Rendu 2D ici
        
        window.endDrawing();
    }
    
    return 0;
}
```

## Avantages de RaylibCPP

- **Orienté objet** : API plus intuitive pour les programmeurs C++
- **Gestion RAII** : Libération automatique des ressources
- **Chaînage de méthodes** : Style de programmation fluide
- **Encapsulation** : Meilleure organisation du code

## Compilation

Pour compiler un programme utilisant RaylibCPP :

```bash
g++ -std=c++17 -Ilibs/RaylibCPP mon_programme.cpp libs/RaylibCPP/*.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Intégration avec Zappy

RaylibCPP est utilisé dans le projet Zappy pour :
- Afficher la carte du jeu en 3D
- Visualiser les joueurs et leurs actions
- Gérer la caméra et les vues
- Dessiner l'interface utilisateur (en collaboration avec RayGUICPP)

## Classes principales

- **RaylibCPP::Window** : Gestion de la fenêtre et contexte de rendu
- **RaylibCPP::Shape** : Dessin de formes 2D/3D
- **RaylibCPP::Texture** : Gestion des textures
- **RaylibCPP::Camera** : Contrôle de la caméra
- **RaylibCPP::Input** : Gestion des entrées
- **RaylibCPP::Audio** : Lecture audio
- **RaylibCPP::Font** : Rendu de texte
