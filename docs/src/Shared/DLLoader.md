# Dynamic Loading System (DLLoader)

The DLLoader system enables dynamic loading and management of graphics and user interface libraries in the Zappy application, providing a modular and extensible architecture.

## Architecture Overview

The DLLoader system implements several design patterns to ensure a robust architecture:
- **Template Pattern**: `DLLoader<T>` for generic loading
- **Singleton Pattern**: `LibraryManager` for centralized access
- **Factory Pattern**: Instance creation via external functions
- **Bridge Pattern**: Decoupling between abstraction and implementation

## Modular Structure

### Main Components

- **`DLLoader.hpp`**: Generic template for dynamic library loading with error handling
- **`LibraryManager.hpp`**: Centralized manager (Singleton) for access to loaded libraries
- **`Common.hpp`**: Shared types (Vector3, Color, Rectangle, etc.) between application and libraries
- **`IGraphicsLib.hpp`**: Complete interface for graphics libraries with 3D and 2D rendering
- **`IGuiLib.hpp`**: Interface for user interface libraries with advanced controls

### Class Hierarchy

```cpp
LibraryManager (Singleton)
├── DLLoader<IGraphicsLib> (Template)
│   └── RaylibCPP Implementation
├── DLLoader<IGuiLib> (Template)
│   └── RayGUICPP Implementation
└── Common Types (Vector3, Color, etc.)
```

## Compilation and Deployment

### 1. Dynamic Library Compilation

```bash
# Compile RaylibCPP graphics library
make -C libs/RaylibCPP
# Generates: libs/RaylibCPP/libraylibcpp.so

# Compile RayGUICPP GUI library
make -C libs/RayGUICPP
# Generates: libs/RayGUICPP/libraygui.so
```

### 2. Plugin Deployment

```bash
# Create plugins directory
mkdir -p plugins

# Copy compiled libraries
cp libs/RaylibCPP/libraylibcpp.so plugins/
cp libs/RayGUICPP/libraygui.so plugins/

# Verify dependencies
ldd plugins/libraylibcpp.so
ldd plugins/libraygui.so
```

### 3. Application Compilation

```bash
# Compile with DLLoader support
make -f DLLoaderMakefile

# Or via main Makefile
make GUI
```

## Advanced Usage

### Loading and Initialization

```cpp
#include "Shared/LibraryManager.hpp"
#include "Shared/Common.hpp"

// Get singleton manager instance
auto& libraryManager = LibraryManager::getInstance();

try {
    // Load libraries with error handling
    libraryManager.loadGraphicsLib("plugins/libraylibcpp.so");
    libraryManager.loadGuiLib("plugins/libraygui.so");
    
    // Access loaded libraries
    auto& graphics = libraryManager.getGraphicsLib();
    auto& gui = libraryManager.getGuiLib();
    
    // Initialize graphics window
    graphics.InitWindow(1920, 1080, "Zappy - Graphical Interface");
    graphics.SetTargetFPS(60);
    
    // Configure user interface
    gui.SetStyle(0, 16); // Default style
    
} catch (const std::runtime_error& e) {
    std::cerr << "Loading error: " << e.what() << std::endl;
    return -1;
}
```

### Usage in Game Loop

```cpp
void GameLoop::run() {
    auto& graphics = LibraryManager::getInstance().getGraphicsLib();
    auto& gui = LibraryManager::getInstance().getGuiLib();
    
    while (!graphics.WindowShouldClose()) {
        // Begin rendering
        graphics.BeginDrawing();
        graphics.ClearBackground({20, 20, 40, 255});
        
        // 3D rendering
        graphics.BeginMode3D(camera);
        render3DScene(graphics);
        graphics.EndMode3D();
        
        // User interface
        renderUI(gui);
        
        graphics.EndDrawing();
    }
    
    graphics.CloseWindow();
}
```

### Resource Management

```cpp
// Load 3D models
int modelId = graphics.LoadModel("assets/models/player.obj");
int textureId = graphics.LoadTexture("assets/textures/player.png");

// Render with resource management
graphics.DrawModel(modelId, {0, 0, 0}, 1.0f, {255, 255, 255, 255});

// Automatic resource cleanup
graphics.UnloadModel(modelId);
graphics.UnloadTexture(textureId);
```

## System Extension

### Creating a New Graphics Library

To add a new graphics implementation:

```cpp
// MyCustomGraphics.hpp
#include "Shared/IGraphicsLib.hpp"

class MyCustomGraphics : public IGraphicsLib {
public:
    // Implementation of all virtual methods
    void InitWindow(int width, int height, const std::string& title) override {
        // Custom implementation
    }
    
    void BeginDrawing() override {
        // Rendering start logic
    }
    
    void EndDrawing() override {
        // Rendering end logic
    }
    
    // ... other methods
};

// Export function for dynamic loading
extern "C" {
    IGraphicsLib* createGraphicsLib() {
        static MyCustomGraphics instance;
        return &instance;
    }
    
    void destroyGraphicsLib(IGraphicsLib* lib) {
        // Cleanup if necessary
    }
}
```

### Custom Library Compilation

```bash
# Compile as shared library
g++ -shared -fPIC -o libmycustomgraphics.so MyCustomGraphics.cpp \
    -I../../src/Shared -std=c++17

# Install in plugins directory
cp libmycustomgraphics.so plugins/
```

### Loading and Testing

```cpp
// Test the new library
try {
    auto& manager = LibraryManager::getInstance();
    manager.loadGraphicsLib("plugins/libmycustomgraphics.so");
    
    auto& graphics = manager.getGraphicsLib();
    // Test basic functionality
    graphics.InitWindow(800, 600, "Test Custom Graphics");
    
    std::cout << "Custom library loaded successfully!" << std::endl;
    
} catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
```

## Error Handling and Robustness

### Loading Validation

```cpp
// Verify library validity
bool LibraryManager::validateLibraries() const {
    try {
        if (!m_graphicsLoader || !m_guiLoader) {
            return false;
        }
        
        // Test critical functions
        auto& graphics = getGraphicsLib();
        auto& gui = getGuiLib();
        
        // Specific verifications
        return true;
    } catch (...) {
        return false;
    }
}
```

### Error Recovery

```cpp
// Fallback strategy in case of failure
void GameApplication::initializeLibraries() {
    std::vector<std::string> graphicsLibs = {
        "plugins/libraylibcpp.so",
        "plugins/libraylibcpp_fallback.so",
        "plugins/libraylibcpp_basic.so"
    };
    
    auto& manager = LibraryManager::getInstance();
    
    for (const auto& lib : graphicsLibs) {
        try {
            manager.loadGraphicsLib(lib);
            std::cout << "Library loaded: " << lib << std::endl;
            break;
        } catch (const std::exception& e) {
            std::cerr << "Failed to load " << lib << ": " << e.what() << std::endl;
        }
    }
}
```

## Optimizations and Performance

### Lazy Loading

```cpp
// Load libraries on demand
class LazyLibraryManager {
private:
    std::optional<std::unique_ptr<DLLoader<IGraphicsLib>>> m_graphicsLoader;
    std::string m_graphicsLibPath;
    
public:
    IGraphicsLib& getGraphicsLib() {
        if (!m_graphicsLoader) {
            loadGraphicsLib(m_graphicsLibPath);
        }
        return m_graphicsLoader->get()->getInstance();
    }
};
```

### Instance Caching

```cpp
// Cache to avoid repeated calls
class CachedLibraryManager {
private:
    mutable std::optional<std::reference_wrapper<IGraphicsLib>> m_cachedGraphics;
    mutable std::optional<std::reference_wrapper<IGuiLib>> m_cachedGui;
    
public:
    IGraphicsLib& getGraphicsLib() const {
        if (!m_cachedGraphics) {
            m_cachedGraphics = std::ref(m_graphicsLoader->getInstance());
        }
        return m_cachedGraphics->get();
    }
};
```

## Best Practices

### 1. Version Management

```cpp
// Version compatibility checking
struct LibraryVersion {
    int major, minor, patch;
};

extern "C" {
    LibraryVersion getLibraryVersion() {
        return {1, 2, 3};
    }
}
```

### 2. Interface Documentation

```cpp
/**
 * @brief Interface for graphics libraries
 * 
 * This interface defines all required methods
 * for implementing a graphics library
 * compatible with the DLLoader system.
 */
class IGraphicsLib {
    /**
     * @brief Initialize a graphics window
     * @param width Width in pixels
     * @param height Height in pixels
     * @param title Window title
     */
    virtual void InitWindow(int width, int height, const std::string& title) = 0;
};
```

### 3. Unit Testing

```cpp
// Tests to validate the DLLoader system
TEST(DLLoaderTest, LoadValidLibrary) {
    auto loader = std::make_unique<DLLoader<IGraphicsLib>>();
    EXPECT_NO_THROW(loader->load("plugins/libraylibcpp.so"));
}

TEST(DLLoaderTest, LoadInvalidLibrary) {
    auto loader = std::make_unique<DLLoader<IGraphicsLib>>();
    EXPECT_THROW(loader->load("nonexistent.so"), std::runtime_error);
}
```
