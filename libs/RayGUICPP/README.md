# RayGUICPP - Encapsulation C++ de raygui pour Zappy

Ce dossier fournit une encapsulation moderne de la bibliothèque raygui (UI pour Raylib) en C++.

## Prérequis
- Raylib installé sur votre système
- Compilation sous Linux (X11, pthread, dl, rt, GL)

## Structure
- `include/RayGuiEncap.hpp` : encapsulation C++ de raygui
- `raygui/src/raygui.h` : header raygui (fourni)
- `RayGui.cpp` et `raygui_impl.cpp` : implémentation C++ et pont avec raygui
- `test_rayguicpp.cpp` : exemple d'utilisation

## Compilation
Placez-vous dans `libs/RayGUICPP` puis compilez avec :
```sh
g++ -Iinclude -Iraygui/src -o test_rayguicpp test_rayguicpp.cpp raygui_impl.cpp RayGui.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Exemple minimal
```cpp
#include <raylib.h>
#include "RayGuiEncap.hpp"

int main() {
    InitWindow(800, 600, "Test RayGUICPP");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        RayGUICPP::Button::Draw({100,100,200,40}, "Hello Raygui");
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
```

## Utilisation dans Zappy
- Inclure `RayGuiEncap.hpp` dans vos fichiers C++
- Utiliser les classes statiques du namespace `RayGUICPP` pour dessiner vos contrôles GUI (Button, Label, CheckBox, Slider, etc.)
- Voir `test_rayguicpp.cpp` pour un exemple complet

## Intégration dynamique (DLLoader)

- RayGUICPP implémente l'interface `IGuiLib` (voir `src/Shared/IGuiLib.hpp`).
- Exporte la fonction suivante pour l'intégration dynamique :

```cpp
extern "C" {
    std::shared_ptr<IGuiLib> createGuiLib();
}
```

- À compiler en bibliothèque partagée (`libraygui.so`) et placer dans le dossier `plugins/`.
- À charger via le `LibraryManager` de Zappy (voir README dans `src/Shared/`).

## Exemple d'utilisation avec DLLoader

Voir `src/GUI/test_dlloader.cpp` pour un exemple d'utilisation dynamique.

## Astuces
- Pour ajouter de nouveaux contrôles, suivez le modèle des classes existantes dans `RayGuiEncap.hpp`
- Pour personnaliser le style, utilisez `RayGui::LoadStyle("chemin/vers/style.rgs")`
- Pour compiler dans un autre dossier, adaptez les chemins d'inclusion `-I`

## Conseils d'extension
- Pour ajouter un nouveau contrôle, créez une nouvelle classe dans `RayGuiEncap.hpp` et exposez-la dans le namespace `RayGUICPP`.
- Respectez l'interface pour garantir la compatibilité avec le système de plugins.

## Liens utiles
- [Documentation raygui](https://github.com/raysan5/raygui)
- [Documentation raylib](https://www.raylib.com/)

---
Pour toute question, contactez l'équipe Zappy !
