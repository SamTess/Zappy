# RaylibCPP - C++ Encapsulation of Raylib

This folder provides a complete C++ encapsulation of Raylib for the Zappy project.

## Main Features
- Window management (creation, closing, drawing)
- 2D and 3D shape drawing (rectangle, circle, cube, sphere, grid, cylinder, 3D line, etc.)
- Advanced 2D and 3D camera management (creation, modes, manipulation)
- 2D and 3D texture management (loading, binding, unloading)
- 3D model management (loading, drawing, unloading)
- Font management and text rendering
- Audio management (sounds, music, volume)
- Keyboard/mouse input management

## Folder Structure
- `window/`: window management
- `shape/`: 2D/3D shapes
- `texture/`: 2D/3D textures and 3D models
- `font/`: fonts and text
- `audio/`: sounds and music
- `input/`: keyboard/mouse input
- `camera/`: 2D/3D cameras
- `utils/`: utilities (vectors, colors, etc.)

## Usage Example
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

## Dynamic Integration (DLLoader)

This library is designed to be dynamically loaded via Zappy's DLLoader system.

- Implements the `IGraphicsLib` interface (see `src/Shared/IGraphicsLib.hpp`).
- Exports the following function for dynamic integration :

```cpp
extern "C" {
    std::shared_ptr<IGraphicsLib> createGraphicsLib();
}
```

- To be compiled as a shared library (`libraylibcpp.so`) and placed in the `plugins/` folder.
- To be loaded via Zappy's `LibraryManager` (see README in `src/Shared/`).

## Usage Example with DLLoader

See `src/GUI/test_dlloader.cpp` for an example of dynamic usage.

## Advanced Notes
- Any new Raylib feature must be encapsulated in a dedicated class (see folder structure).
- To extend, follow the model of existing classes and expose methods via the interface.

---
For any questions, contact the Zappy team !
