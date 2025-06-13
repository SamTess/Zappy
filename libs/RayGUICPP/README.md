# RayGUICPP - C++ Encapsulation of raygui for Zappy

This folder provides a modern C++ encapsulation of the raygui library (UI for Raylib).

## Prerequisites
- Raylib installed on your system
- Compilation under Linux (X11, pthread, dl, rt, GL)

## Structure
- `include/RayGuiEncap.hpp`: C++ encapsulation of raygui
- `raygui/src/raygui.h`: raygui header (provided)
- `RayGui.cpp` and `raygui_impl.cpp`: C++ implementation and bridge with raygui
- `test_rayguicpp.cpp`: usage example

## Compilation
Go to `libs/RayGUICPP` and compile with:
```sh
g++ -Iinclude -Iraygui/src -o test_rayguicpp test_rayguicpp.cpp raygui_impl.cpp RayGui.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Minimal Example
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

## Usage in Zappy
- Include `RayGuiEncap.hpp` in your C++ files
- Use the static classes from the `RayGUICPP` namespace to draw your GUI controls (Button, Label, CheckBox, Slider, etc.)
- See `test_rayguicpp.cpp` for a complete example

## Dynamic Integration (DLLoader)

- RayGUICPP implements the `IGuiLib` interface (see `src/Shared/IGuiLib.hpp`).
- Exports the following function for dynamic integration :

```cpp
extern "C" {
    std::shared_ptr<IGuiLib> createGuiLib();
}
```

- To be compiled as a shared library (`libraygui.so`) and placed in the `plugins/` folder.
- To be loaded via Zappy's `LibraryManager` (see README in `src/Shared/`).

## Example of use with DLLoader

See `src/GUI/test_dlloader.cpp` for an example of dynamic use.

## Tips
- To add new controls, follow the model of existing classes in `RayGuiEncap.hpp`
- To customize the style, use `RayGui::LoadStyle("path/to/style.rgs")`
- To compile in another folder, adapt the inclusion paths `-I`

## Extension advice
- To add a new control, create a new class in `RayGuiEncap.hpp` and expose it in the `RayGUICPP` namespace.
- Respect the interface to ensure compatibility with the plugin system.

## Useful links
- [raygui documentation](https://github.com/raysan5/raygui)
- [raylib documentation](https://www.raylib.com/)

---
For any questions, contact the Zappy team !
