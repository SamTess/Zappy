# Utilisation de RayGUICPP (raygui C++ wrapper) dans le projet Zappy

## Prérequis
- raylib installé sur votre système (librairie graphique)
- Compilation sous Linux (dépendances : X11, pthread, dl, rt, GL)

## Structure
- `libs/RayGUICPP/include/RayGui.hpp` : encapsulation C++ de raygui
- `libs/RayGUICPP/raygui-4.0/src/raygui.h` : header raygui (fourni)
- `libs/RayGUICPP/raygui_impl.cpp` : fichier d'implémentation raygui (nécessaire pour l'édition de liens)
- `libs/RayGUICPP/test_rayguicpp.cpp` : exemple d'utilisation

## Compilation d'un exécutable utilisant RayGUICPP

Placez-vous dans `libs/RayGUICPP` puis compilez avec :

```sh
g++ -Iinclude -Iraygui-4.0/src -o test_rayguicpp test_rayguicpp.cpp raygui_impl.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

- `-Iinclude` : pour inclure RayGui.hpp
- `-Iraygui-4.0/src` : pour inclure raygui.h
- `raygui_impl.cpp` : contient l'implémentation de raygui (obligatoire)

## Exemple minimal

```cpp
#include <raylib.h>
#include "RayGui.hpp"

using namespace RayGUICPP;

int main() {
    InitWindow(800, 600, "Test RayGUICPP");
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Button::Draw({100,100,200,40}, "Hello Raygui");
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
```

## Utilisation dans le projet Zappy

- Inclure `RayGui.hpp` dans vos fichiers C++
- Utiliser les classes statiques du namespace `RayGUICPP` pour dessiner vos contrôles GUI (Button, Label, CheckBox, Slider, etc.)
- Voir `test_rayguicpp.cpp` pour un exemple complet

## Astuces
- Pour ajouter de nouveaux contrôles, suivez le modèle des classes existantes dans `RayGui.hpp`
- Pour personnaliser le style, utilisez `RayGui::LoadStyle("chemin/vers/style.rgs")`
- Pour compiler dans un autre dossier, adaptez les chemins d'inclusion `-I`

## Liens utiles
- [Documentation raygui](https://github.com/raysan5/raygui)
- [Documentation raylib](https://www.raylib.com/)

---
Pour toute question, contactez l'équipe Zappy !
