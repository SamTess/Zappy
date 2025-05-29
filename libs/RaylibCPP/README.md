# RaylibCPP - Encapsulation C++ de Raylib

Ce dossier propose une encapsulation complète de Raylib en C++ pour le projet Zappy.

## Fonctionnalités principales
- Gestion de la fenêtre (création, fermeture, drawing)
- Dessin de formes 2D et 3D (rectangle, cercle, cube, sphère, grille, cylindre, ligne 3D, etc.)
- Gestion avancée des caméras 2D et 3D (création, modes, manipulation)
- Gestion des textures 2D et 3D (chargement, binding, déchargement)
- Gestion des modèles 3D (chargement, dessin, déchargement)
- Gestion des polices et rendu de texte
- Gestion de l'audio (sons, musiques, volume)
- Gestion des entrées clavier/souris

## Structure du dossier
- `window/` : gestion de la fenêtre
- `shape/` : formes 2D/3D
- `texture/` : textures 2D/3D et modèles 3D
- `font/` : polices et texte
- `audio/` : sons et musiques
- `input/` : entrées clavier/souris
- `camera/` : caméras 2D/3D
- `utils/` : utilitaires (vecteurs, couleurs, etc.)

## Exemple d'utilisation
```cpp
#include "RayLib.hpp"

int main() {
    RayLib gfx;
    gfx.InitWindow(800, 600, "Demo RaylibCPP");
    gfx.CreateCamera3D({0,10,10}, {0,0,0}, {0,1,0}, 45.0f, 0);
    while (!gfx.WindowShouldClose()) {
        gfx.BeginDrawing();
        gfx.ClearBackground({255,255,255,255});
        gfx.BeginCamera3D();
        gfx.DrawCube({0,0,0}, 2,2,2, {255,0,0,255});
        gfx.EndCamera3D();
        gfx.EndDrawing();
    }
    gfx.CloseWindow();
    return 0;
}
```

## Intégration dynamique (DLLoader)

Cette bibliothèque est conçue pour être chargée dynamiquement via le système DLLoader de Zappy.

- Implémente l'interface `IGraphicsLib` (voir `src/Shared/IGraphicsLib.hpp`).
- Exporte la fonction suivante pour l'intégration dynamique :

```cpp
extern "C" {
    std::shared_ptr<IGraphicsLib> createGraphicsLib();
}
```

- À compiler en bibliothèque partagée (`libraylibcpp.so`) et placer dans le dossier `plugins/`.
- À charger via le `LibraryManager` de Zappy (voir README dans `src/Shared/`).

## Exemple d'utilisation avec DLLoader

Voir `src/GUI/test_dlloader.cpp` pour un exemple d'utilisation dynamique.

## Notes avancées
- Toute nouvelle fonctionnalité Raylib doit être encapsulée dans une classe dédiée (voir structure du dossier).
- Pour étendre, suivez le modèle des classes existantes et exposez les méthodes via l'interface.

---
Pour toute question, contactez l'équipe Zappy !
