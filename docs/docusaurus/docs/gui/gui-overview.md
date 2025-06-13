---
sidebar_position: 1
---

# Graphical Interface Architecture

## Overview

The Zappy graphical interface is developed in C++ leveraging Raylib for 3D rendering. It provides a real-time visualization of the game state through a sophisticated, modular architecture built on dynamic library loading and component-based design.

## Core Architecture

### Main Components

The GUI is structured around a central game loop that integrates multiple specialized components:

```cpp
class GameLoop : public IGraphicalContextObserver {
private:
    // Core systems
    std::shared_ptr<IGraphicsLib> m_graphics;      // Graphics abstraction (Raylib)
    std::shared_ptr<IGuiLib> m_gui;                // UI framework interface
    GraphicalContext m_context;                    // Game state context
    std::unique_ptr<Renderer> m_renderer;          // 3D rendering pipeline
    std::unique_ptr<CameraController> m_camera;    // Camera management
    std::unique_ptr<NetworkManager> m_network;     // Server communication
    
    // State management
    GameState m_currentState;                      // Cached game state
    std::atomic<bool> m_running;                   // Main loop control
    
public:
    void run() override;
    void onStateUpdate(const GameState& state) override;
    void handleEvent(const Event& event) override;
    
private:
    void processNetworkEvents();
    void updateSystems(float deltaTime);
    void renderFrame();
    void handleInput();
};
```

## Dynamic Library System

The GUI uses an advanced dynamic library loading system that enables modular development and component replacement without recompilation.

### Library Interfaces

```cpp
// Abstract graphics interface
class IGraphicsLib {
public:
    virtual ~IGraphicsLib() = default;
    
    // Core functions
    virtual bool init(int width, int height, const std::string& title) = 0;
    virtual void beginDrawing() = 0;
    virtual void endDrawing() = 0;
    virtual void clearBackground(ZappyTypes::Color color) = 0;
    virtual bool windowShouldClose() = 0;
    
    // Camera operations
    virtual void setCameraMode(ZappyTypes::Camera camera, int mode) = 0;
    virtual void updateCamera(ZappyTypes::Camera* camera) = 0;
    virtual void setCameraPosition(ZappyTypes::Camera* camera, ZappyTypes::Vector3 position) = 0;
    
    // Shape rendering
    virtual void drawCube(ZappyTypes::Vector3 position, float width, float height, float length, ZappyTypes::Color color) = 0;
    virtual void drawModel(ZappyTypes::Model model, ZappyTypes::Vector3 position, float scale, ZappyTypes::Color tint) = 0;
    virtual void drawText3D(const std::string& text, ZappyTypes::Vector3 position, float fontSize, ZappyTypes::Color color) = 0;
    
    // 2D UI rendering
    virtual void drawRectangle(int x, int y, int width, int height, ZappyTypes::Color color) = 0;
    virtual void drawText(const std::string& text, int x, int y, int fontSize, ZappyTypes::Color color) = 0;
    
    // Resource loading
    virtual ZappyTypes::Model loadModel(const std::string& filename) = 0;
    virtual ZappyTypes::Texture2D loadTexture(const std::string& filename) = 0;
    virtual void unloadModel(ZappyTypes::Model model) = 0;
    virtual void unloadTexture(ZappyTypes::Texture2D texture) = 0;
};

// Abstract GUI interface
class IGuiLib {
public:
    virtual ~IGuiLib() = default;
    
    // UI components
    virtual bool button(ZappyTypes::Rectangle bounds, const std::string& text) = 0;
    virtual bool checkbox(ZappyTypes::Rectangle bounds, const std::string& text, bool& checked) = 0;
    virtual bool slider(ZappyTypes::Rectangle bounds, const std::string& text, float& value, float min, float max) = 0;
    virtual bool dropdownBox(ZappyTypes::Rectangle bounds, const std::vector<std::string>& items, int& active) = 0;
    
    // Windows and panels
    virtual bool windowBox(ZappyTypes::Rectangle bounds, const std::string& title) = 0;
    virtual bool groupBox(ZappyTypes::Rectangle bounds, const std::string& text) = 0;
    virtual bool panel(ZappyTypes::Rectangle bounds) = 0;
    
    // Text controls
    virtual void label(ZappyTypes::Rectangle bounds, const std::string& text) = 0;
    virtual bool textBox(ZappyTypes::Rectangle bounds, std::string& text, int maxLength, bool editMode) = 0;
    
    // Visuals
    virtual void statusBar(ZappyTypes::Rectangle bounds, const std::string& text) = 0;
    virtual void progressBar(ZappyTypes::Rectangle bounds, const std::string& text, float value, float min, float max) = 0;
};
```

### Library Manager

The GUI implements a powerful library manager that handles dynamic library loading and interfaces:

```cpp
class LibraryManager {
private:
    std::unordered_map<std::string, void*> m_loadedLibraries;
    std::mutex m_libraryMutex;
    
public:
    // Singleton pattern
    static LibraryManager& getInstance() {
        static LibraryManager instance;
        return instance;
    }
    
    template<typename T>
    std::shared_ptr<T> getInterface(const std::string& libraryPath, const std::string& factoryFunction) {
        std::lock_guard<std::mutex> lock(m_libraryMutex);
        
        void* handle = loadLibrary(libraryPath);
        if (!handle) {
            std::cerr << "Failed to load library: " << libraryPath << std::endl;
            return nullptr;
        }
        
        // Get factory function from library
        typedef T* (*CreateInstanceFunc)();
        CreateInstanceFunc createFunc = (CreateInstanceFunc)dlsym(handle, factoryFunction.c_str());
        
        if (!createFunc) {
            std::cerr << "Failed to find factory function: " << factoryFunction << std::endl;
            return nullptr;
        }
        
        // Create interface instance with custom deleter
        return std::shared_ptr<T>(createFunc(), 
            [this, libraryPath](T* ptr) {
                delete ptr;
                unloadLibrary(libraryPath);
            });
    }
    
private:
    LibraryManager() = default;
    void* loadLibrary(const std::string& path);
    void unloadLibrary(const std::string& path);
};
```

## Rendering System

The rendering system uses the Strategy pattern to support multiple visualization techniques:

```cpp
// Tile rendering strategy interface
class ITileRenderStrategy {
public:
    virtual ~ITileRenderStrategy() = default;
    virtual void renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        int x, int y,
        const ZappyTypes::Color& color,
        float tileSize,
        float spacing) = 0;
};

// Implementation strategies
class SimpleTileRenderStrategy : public ITileRenderStrategy {
    // Simple flat tile rendering
};

class DetailedTileRenderStrategy : public ITileRenderStrategy {
    // Enhanced tile with resource visualization
};

class ModelTileRenderStrategy : public ITileRenderStrategy {
    // 3D model-based tile rendering
};

// Factory for creating tile rendering strategies
class TileRenderStrategyFactory {
public:
    static std::unique_ptr<ITileRenderStrategy> createStrategy(
        TileRenderStrategyType type,
        const std::shared_ptr<GraphicalContext>& context) {
        
        switch (type) {
            case TileRenderStrategyType::SIMPLE:
                return std::make_unique<SimpleTileRenderStrategy>();
            case TileRenderStrategyType::DETAILED:
                return std::make_unique<DetailedTileRenderStrategy>(context);
            case TileRenderStrategyType::MODEL:
                return std::make_unique<ModelTileRenderStrategy>(context);
            default:
                return std::make_unique<SimpleTileRenderStrategy>();
        }
    }
};
```

## Camera Control System

The camera system provides multiple viewing modes and smooth transitions:

```cpp
class CameraController {
private:
    std::shared_ptr<IGraphicsLib> m_graphicsLib;
    ZappyTypes::Camera m_camera;
    CameraMode m_currentMode;
    
    // Animation parameters
    ZappyTypes::Vector3 m_targetPosition;
    ZappyTypes::Vector3 m_targetTarget;
    float m_transitionSpeed;
    bool m_inTransition;

public:
    CameraController(std::shared_ptr<IGraphicsLib> graphicsLib);
    
    // Camera modes
    void setOverheadView();
    void setIsometricView();
    void setFollowPlayerView(int playerId);
    void setFreeView();
    
    // Camera operations
    void update(float deltaTime);
    void handleInput(const InputState& input);
    void zoomIn(float factor);
    void zoomOut(float factor);
    void rotateLeft(float angle);
    void rotateRight(float angle);
    
    // Transitions
    void transitionTo(const ZappyTypes::Vector3& position, const ZappyTypes::Vector3& target, float duration);
};
```

## Network Communication

The network component manages bidirectional communication with the server:

```cpp
class NetworkManager {
private:
    std::string m_host;
    int m_port;
    int m_socket;
    std::atomic<bool> m_connected;
    
    std::thread m_receiveThread;
    std::mutex m_queueMutex;
    std::queue<std::string> m_messageQueue;
    
    GameState* m_gameState;

public:
    NetworkManager(const std::string& host, int port, GameState* gameState);
    ~NetworkManager();
    
    // Connection management
    bool connect();
    void disconnect();
    bool isConnected() const;
    
    // Communication
    bool sendCommand(const std::string& command);
    std::optional<std::string> getNextMessage();
    
    // Protocol handling
    void processServerUpdates();
    void parseMapSizeMessage(const std::string& message);
    void parseTileContentMessage(const std::string& message);
    void parsePlayerMessage(const std::string& message);
    
private:
    void receiveThreadFunction();
    void updateGameState(const std::string& message);
};
```

## Texture and Model Management

Resource management is handled through a specialized system:

```cpp
class TextureManager {
private:
    std::shared_ptr<IGraphicsLib> m_graphicsLib;
    std::unordered_map<std::string, ZappyTypes::Texture2D> m_textures;
    std::unordered_map<std::string, ZappyTypes::Model> m_models;

public:
    explicit TextureManager(std::shared_ptr<IGraphicsLib> graphicsLib);
    ~TextureManager();
    
    // Resource loading
    ZappyTypes::Texture2D loadTexture(const std::string& path);
    ZappyTypes::Model loadModel(const std::string& path);
    
    // Resource access
    ZappyTypes::Texture2D getTexture(const std::string& name);
    ZappyTypes::Model getModel(const std::string& name);
    
    // Resource unloading
    void unloadAll();
};
```

## User Interface Components

The UI system includes specialized components for game state visualization:

```cpp
class TeamPanel {
public:
    void render(const std::shared_ptr<IGuiLib>& guiLib, const GameState& gameState);
};

class PlayerInfoPanel {
public:
    void render(const std::shared_ptr<IGuiLib>& guiLib, const Player& player);
};

class MapControlPanel {
public:
    void render(const std::shared_ptr<IGuiLib>& guiLib, CameraController& cameraController);
};

class TimeControlPanel {
public:
    void render(const std::shared_ptr<IGuiLib>& guiLib, NetworkManager& networkManager);
};
```

## Performance Optimizations

The GUI implements several optimizations for smooth performance:

1. **Frustum Culling**: Only renders tiles within the camera view
2. **Level of Detail (LOD)**: Adjusts rendering detail based on camera distance
3. **Texture Atlasing**: Combines multiple textures to reduce draw calls
4. **Instanced Rendering**: Efficiently renders multiple identical objects
5. **Asynchronous Resource Loading**: Loads textures and models in background threads

---

This document provides a technical overview of the GUI architecture. For implementation details, refer to the source code in `src/GUI/` directory.
