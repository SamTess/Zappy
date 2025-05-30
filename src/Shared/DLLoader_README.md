# DLLoader pour les bibliothèques graphiques et GUI

Ce système permet de charger dynamiquement des bibliothèques graphiques et GUI dans l'application Zappy.

## Structure

- `DLLoader.hpp` : Template générique pour charger des bibliothèques dynamiques
- `LibraryManager.hpp` : Gestionnaire de bibliothèques (pattern Singleton) pour accéder facilement aux bibliothèques chargées
- `Common.hpp` : Types partagés (Vector3, Color, Rectangle, etc.) entre l'application et les bibliothèques
- `IGraphicsLib.hpp` : Interface pour les bibliothèques graphiques
- `IGuiLib.hpp` : Interface pour les bibliothèques GUI

## Compilation

1. Compiler les bibliothèques dynamiques :
```bash
make -C libs/RaylibCPP
make -C libs/RayGUICPP
```

2. Copier les bibliothèques dans le dossier plugins :
```bash
mkdir -p plugins
cp libs/RaylibCPP/libraylibcpp.so plugins/
cp libs/RayGUICPP/libraygui.so plugins/
```

3. Compiler l'exemple :
```bash
make -f DLLoaderMakefile
```

## Utilisation

Pour charger et utiliser les bibliothèques dans votre code :

```cpp
#include "Shared/LibraryManager.hpp"

// Chargement des bibliothèques
auto& libraryManager = LibraryManager::getInstance();
libraryManager.loadGraphicsLib("chemin/vers/libraylibcpp.so");
libraryManager.loadGuiLib("chemin/vers/libraygui.so");

// Utilisation des bibliothèques
auto& graphics = libraryManager.getGraphicsLib();
auto& gui = libraryManager.getGuiLib();

// Appels aux fonctions des bibliothèques
graphics.InitWindow(800, 600, "Mon application");
gui.DrawButton(10, 10, 200, 30, "Mon bouton");
```

## Extension du système

Pour ajouter une nouvelle implémentation de bibliothèque, vous devez :

1. Créer une classe qui implémente l'interface `IGraphicsLib` ou `IGuiLib`
2. Exposer une fonction de création `extern "C"` :
```cpp
extern "C" {
    IGraphicsLib* createGraphicsLib() {
        static MaNouvelleBibliotheque instance;
        return &instance;
    }
}
```
3. Compiler cette classe en tant que bibliothèque partagée (.so)
4. Charger cette bibliothèque avec le `LibraryManager`
