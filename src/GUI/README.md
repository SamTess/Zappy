# Using DLLoader and Dynamic Graphics Libraries in Zappy GUI

This folder contains the source code for the Zappy graphical interface. The application uses a dynamic library loading system (DLLoader) to allow the use of different graphics and GUI implementations without modifying the main code.

## General Operation

- The `IGraphicsLib` and `IGuiLib` interfaces define the expected functions for graphics and GUI libraries.
- The `LibraryManager` (Singleton pattern) allows dynamic loading of shared libraries (`.so`) and access to their features via the interfaces.
- Concrete libraries (e.g., RaylibCPP, RayGUICPP) are compiled separately and placed in the `plugins/` folder.

## Usage Example

```cpp
#include "Shared/LibraryManager.hpp"

// Loading dynamic libraries
auto& libraryManager = LibraryManager::getInstance();
libraryManager.loadGraphicsLib("plugins/libraylibcpp.so");
libraryManager.loadGuiLib("plugins/libraygui.so");

// Retrieving interfaces
auto& graphics = libraryManager.getGraphicsLib();
auto& gui = libraryManager.getGuiLib();

// Usage
graphics.InitWindow(1280, 720, "Zappy");
gui.DrawButton(100, 100, 200, 50, "Start Game");
```

## Adding a New Library

1. Implement the `IGraphicsLib` or `IGuiLib` interface in a new class.
2. Expose a C-compatible creation function:
```cpp
extern "C" {
    IGraphicsLib* createGraphicsLib() {
        static MyLib instance;
        return &instance;
    }
}
```
3. Compile the library as a `.so` and place it in `plugins/`.
4. Load it via the `LibraryManager` in your code.

## Advantages
- Allows switching graphic or GUI engines without recompiling the entire application.
- Promotes extensibility and modularity of the project.

## Minimal Example

See `test_dlloader.cpp` for an example code that dynamically loads graphic and GUI libraries, then displays a window with a button.

### Compilation

Make sure you have compiled the dynamic libraries:
```sh
make -C ../../libs/RaylibCPP
make -C ../../libs/RayGUICPP
```

Then compile the example:
```sh
g++ -I../../libs/RaylibCPP -I../../libs/RayGUICPP/include -I../Shared -o test_dlloader test_dlloader.cpp -ldl -lraylib -lGL -lm -lpthread -lrt -lX11
```

### Execution

Place the `libraylibcpp.so` and `libraygui.so` files in the `plugins/` folder at the root of the project, then run:
```sh
./test_dlloader
```

## Operation

- The `LibraryManager` dynamically loads graphic and GUI libraries via their interfaces (`IGraphicsLib`, `IGuiLib`).
- You can then use the methods of these interfaces as if they were native.

## Further Exploration

- Check the README in `src/Shared/` for more details on the plugin system.
- Adapt the example to test your own dynamic libraries!
