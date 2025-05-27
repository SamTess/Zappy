# Raylib C++ Encapsulation

Ce dossier contient une encapsulation complète de Raylib en C++, organisée par fonctionnalités.

## Structure des sous-dossiers

- `window/` : Gestion de la fenêtre et du contexte principal
- `texture/` : Gestion des textures et images, modèles 3D
- `audio/` : Gestion de l'audio, musiques et sons
- `font/` : Gestion des polices
- `shape/` : Dessin de formes géométriques 2D/3D
- `input/` : Gestion des entrées clavier/souris
- `camera/` : Gestion des caméras 2D/3D
- `utils/` : Utilitaires divers (Vector3, couleurs, etc.)

Chaque sous-dossier contient un header et un fichier source pour la fonctionnalité correspondante.

## Exemple d'utilisation

Voici un exemple minimal d'utilisation de l'encapsulation RaylibCPP pour une scène 3D :

```cpp
#include "raylibcpp.hpp"
#include "raymath.h"

int main() {
    raylibcpp::Window window(1200, 800, "Demo RaylibCPP");
    SetTargetFPS(60);

    raylibcpp::Camera3DWrap camera;
    Vector3 &camPos = camera.get().position;
    Vector3 &camTarget = camera.get().target;
    Vector3 &camUp = camera.get().up;

    raylibcpp::Vector3Wrap objPos(0.0f, 1.0f, 0.0f);
    raylibcpp::ModelWrap model("assets/bugatti.obj");
    raylibcpp::Font font("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");
    DisableCursor();

    while (!window.shouldClose()) {
        // Contrôles caméra (ZQSD + souris)
        float moveSpeed = 0.2f;
        float rotSpeed = 0.03f;
        Vector3 forward = raylibcpp::Vector3Wrap::normalize(raylibcpp::Vector3Wrap::sub(camTarget, camPos));
        Vector3 right = raylibcpp::Vector3Wrap::normalize(raylibcpp::Vector3Wrap::cross(forward, camUp));
        if (raylibcpp::Input::isKeyDown(KEY_W)) camPos = raylibcpp::Vector3Wrap::add(camPos, raylibcpp::Vector3Wrap::scale(forward, moveSpeed));
        if (raylibcpp::Input::isKeyDown(KEY_S)) camPos = raylibcpp::Vector3Wrap::sub(camPos, raylibcpp::Vector3Wrap::scale(forward, moveSpeed));
        if (raylibcpp::Input::isKeyDown(KEY_A)) camPos = raylibcpp::Vector3Wrap::sub(camPos, raylibcpp::Vector3Wrap::scale(right, moveSpeed));
        if (raylibcpp::Input::isKeyDown(KEY_D)) camPos = raylibcpp::Vector3Wrap::add(camPos, raylibcpp::Vector3Wrap::scale(right, moveSpeed));
        Vector2 mouseDelta = GetMouseDelta();
        Matrix rotY = MatrixRotateY(-mouseDelta.x * rotSpeed);
        Matrix rotX = MatrixRotate(raylibcpp::Vector3Wrap::cross(right, camUp), -mouseDelta.y * rotSpeed);
        Vector3 camDir = raylibcpp::Vector3Wrap::sub(camTarget, camPos);
        camDir = raylibcpp::Vector3Wrap::transform(camDir, rotY);
        camDir = raylibcpp::Vector3Wrap::transform(camDir, rotX);
        camTarget = raylibcpp::Vector3Wrap::add(camPos, camDir);

        window.beginDrawing();
        window.clear(RAYWHITE);
        camera.beginMode();
        raylibcpp::Shape::drawGrid(20, 1.0f);
        model.draw(objPos.get(), 1.0f, WHITE);
        camera.endMode();
        font.drawText("ZQSD: Camera | Souris: Regarder", 10, 10, 20, DARKGRAY);
        window.endDrawing();
    }
    return 0;
}
```

## Dépendances
- raylib (https://www.raylib.com/)

## Compilation

```sh
g++ -std=c++20 -I. -Ilibs/RaylibCPP \
    libs/RaylibCPP/test_raylibcpp.cpp \
    libs/RaylibCPP/window/Window.cpp \
    libs/RaylibCPP/font/Font.cpp \
    libs/RaylibCPP/texture/Texture.cpp \
    libs/RaylibCPP/shape/Shape.cpp \
    libs/RaylibCPP/input/Input.cpp \
    libs/RaylibCPP/utils/Utils.cpp \
    libs/RaylibCPP/camera/Camera.cpp \
    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
    -o test_raylibcpp
```

Adaptez les flags selon votre système.

## Extension
Ajoutez d'autres classes pour encapsuler davantage de fonctionnalités raylib (sons, polices, modèles, etc).
