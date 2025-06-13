# DLLoader for Graphics and GUI Libraries

This system allows dynamic loading of graphics and GUI libraries in the Zappy application.

## Structure

- `DLLoader.hpp`: Generic template for loading dynamic libraries
- `LibraryManager.hpp`: Library manager (Singleton pattern) for easy access to loaded libraries
- `Common.hpp`: Shared types (Vector3, Color, Rectangle, etc.) between the application and libraries
- `IGraphicsLib.hpp`: Interface for graphics libraries
- `IGuiLib.hpp`: Interface for GUI libraries

## Compilation

1. Compile the dynamic libraries:
```bash
make -C libs/RaylibCPP
make -C libs/RayGUICPP
```

2. Copy the libraries to the plugins folder:
```bash
mkdir -p plugins
cp libs/RaylibCPP/libraylibcpp.so plugins/
cp libs/RayGUICPP/libraygui.so plugins/
```

3. Compile the example:
```bash
make -f DLLoaderMakefile
```

## Usage

To load and use the libraries in your code:

```cpp
#include "Shared/LibraryManager.hpp"

// Loading the libraries
auto& libraryManager = LibraryManager::getInstance();
libraryManager.loadGraphicsLib("path/to/libraylibcpp.so");
libraryManager.loadGuiLib("path/to/libraygui.so");

// Using the libraries
auto& graphics = libraryManager.getGraphicsLib();
auto& gui = libraryManager.getGuiLib();

// Calls to the library functions
graphics.InitWindow(800, 600, "My application");
gui.DrawButton(10, 10, 200, 30, "My button");
```

## Extending the System

To add a new library implementation, you need to:

1. Create a class that implements the `IGraphicsLib` or `IGuiLib` interface
2. Expose a `extern "C"` creation function:
```cpp
extern "C" {
    IGraphicsLib* createGraphicsLib() {
        static MyNewLibrary instance;
        return &instance;
    }
}
```
3. Compile this class as a shared library (.so)
4. Load this library with the `LibraryManager`
