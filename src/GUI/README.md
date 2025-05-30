# Utilisation de DLLoader et des bibliothèques dynamiques graphiques dans Zappy GUI

Ce dossier contient le code source de l'interface graphique de Zappy. L'application utilise un système de chargement dynamique de bibliothèques (DLLoader) pour permettre l'utilisation de différentes implémentations graphiques et GUI sans modifier le code principal.

## Fonctionnement général

- Les interfaces `IGraphicsLib` et `IGuiLib` définissent les fonctions attendues pour les bibliothèques graphiques et GUI.
- Le gestionnaire `LibraryManager` (pattern Singleton) permet de charger dynamiquement des bibliothèques partagées (`.so`) et d'accéder à leurs fonctionnalités via les interfaces.
- Les bibliothèques concrètes (ex : RaylibCPP, RayGUICPP) sont compilées séparément et placées dans le dossier `plugins/`.

## Exemple d'utilisation

```cpp
#include "Shared/LibraryManager.hpp"

// Chargement des bibliothèques dynamiques
auto& libraryManager = LibraryManager::getInstance();
libraryManager.loadGraphicsLib("plugins/libraylibcpp.so");
libraryManager.loadGuiLib("plugins/libraygui.so");

// Récupération des interfaces
auto& graphics = libraryManager.getGraphicsLib();
auto& gui = libraryManager.getGuiLib();

// Utilisation
graphics.InitWindow(1280, 720, "Zappy");
gui.DrawButton(100, 100, 200, 50, "Lancer la partie");
```

## Ajouter une nouvelle bibliothèque

1. Implémentez l'interface `IGraphicsLib` ou `IGuiLib` dans une nouvelle classe.
2. Exposez une fonction de création compatible C :
```cpp
extern "C" {
    IGraphicsLib* createGraphicsLib() {
        static MaLib instance;
        return &instance;
    }
}
```
3. Compilez la bibliothèque en `.so` et placez-la dans `plugins/`.
4. Chargez-la via le `LibraryManager` dans votre code.

## Avantages
- Permet de changer de moteur graphique ou GUI sans recompiler toute l'application.
- Favorise l'extensibilité et la modularité du projet.

## Exemple minimal

Voir `test_dlloader.cpp` pour un exemple de code qui charge dynamiquement les bibliothèques graphiques et GUI, puis affiche une fenêtre avec un bouton.

### Compilation

Assurez-vous d'avoir compilé les bibliothèques dynamiques :
```sh
make -C ../../libs/RaylibCPP
make -C ../../libs/RayGUICPP
```

Puis compilez l'exemple :
```sh
g++ -I../../libs/RaylibCPP -I../../libs/RayGUICPP/include -I../Shared -o test_dlloader test_dlloader.cpp -ldl -lraylib -lGL -lm -lpthread -lrt -lX11
```

### Exécution

Placez les fichiers `libraylibcpp.so` et `libraygui.so` dans le dossier `plugins/` à la racine du projet, puis lancez :
```sh
./test_dlloader
```

## Fonctionnement

- Le `LibraryManager` charge dynamiquement les bibliothèques graphiques et GUI via leurs interfaces (`IGraphicsLib`, `IGuiLib`).
- Vous pouvez ensuite utiliser les méthodes de ces interfaces comme si elles étaient natives.

## Pour aller plus loin

- Consultez le README dans `src/Shared/` pour plus de détails sur le système de plugins.
- Adaptez l'exemple pour tester vos propres bibliothèques dynamiques !
