# RaylibCPP Documentation

# RaylibCPP - Modern C++ Wrapper for Raylib

## Overview

RaylibCPP is a modern, object-oriented C++ wrapper for the Raylib library, specifically designed for the Zappy project. It implements advanced design patterns and offers an intuitive API with automatic RAII resource management.

## Architecture and Design Patterns

### Implemented Patterns

**RAII (Resource Acquisition Is Initialization):**
- Automatic OpenGL context management
- Automatic cleanup of textures, models, and audio resources
- Constructors and destructors ensuring state consistency

**Facade Pattern:**
- Simplified interface hiding Raylib C complexity
- Consistent API for all graphics operations
- Reduced coupling with underlying implementation

**Builder Pattern:**
- Fluid configuration of complex objects (Camera, Window)
- Chainable methods for expressive syntax
- Parameter validation during construction

## Advanced Features

RaylibCPP offers a complete abstraction layer for:

### Rendering and Graphics
- **Window Management:** Creation, configuration, event handling with multi-screen support
- **2D/3D Rendering:** Optimized drawing primitives, batch rendering, matrix transformations
- **Camera Management:** 2D/3D cameras with FREE, ORBITAL, FIRST_PERSON modes
- **3D Models:** OBJ/GLB/GLTF loading, animations, skeletal animation
- **Textures:** Multi-format loading, compression, automatic mipmapping
- **Shaders:** Custom shader support, dynamic uniforms

### Advanced Systems
- **Spatial Audio:** 3D positional sound, Doppler effects, reverb
- **Physics:** Collision detection integration, raycast, AABB
- **Performance:** Frustum culling, occlusion culling, automatic LOD
- **Input:** Keyboard/mouse/gamepad handling with customizable mapping

### System Integration
- **Multi-threading:** Asynchronous rendering, background asset loading
- **Memory:** Pool allocators, smart pointers, automatic GPU/CPU management
- **Debugging:** Integrated profiling, real-time performance metrics

## Modular Architecture

### Class Hierarchy

```cpp
namespace RaylibCPP {
    // Core Systems
    class Window;                    // Window and OpenGL context management
    class Camera;                    // 2D/3D cameras with advanced controls
    
    // Rendering
    class Renderer;                  // Main rendering engine
    class Model;                     // 3D models with animations
    class Texture;                   // Textures and materials
    class Shader;                    // Custom shaders
    
    // Resources
    class ResourceManager;           // Centralized resource manager
    class AssetLoader;              // Asynchronous asset loading
    
    // Input & Audio
    class InputManager;             // User input management
    class AudioEngine;              // 3D spatial audio system
    
    // Utilities
    class Math;                     // Mathematical utilities
    class Debug;                    // Debugging and profiling tools
}
```

### Module Organization

- **`core/`**: Fundamental classes (Window, Camera, Context)
- **`rendering/`**: Rendering system (Renderer, Model, Texture, Shader)
- **`resources/`**: Asset management (ResourceManager, AssetLoader)
- **`input/`**: Input handling (InputManager, EventSystem)
- **`audio/`**: Audio system (AudioEngine, SpatialAudio)
- **`math/`**: Mathematics (Vector3, Matrix4, Quaternion)
- **`utils/`**: Utilities (Debug, Profiler, Logger)

## Usage

## Advanced Usage

### Initialization and Configuration

```cpp
#include "RayLib.hpp"
#include "ResourceManager.hpp"

int main() {
    // Advanced window configuration
    RaylibCPP::Window window;
    window.setSize(1920, 1080)
          .setTitle("Zappy - 3D Graphical Interface")
          .setTargetFPS(60)
          .setVSync(true)
          .setAntialiasing(4)
          .create();
    
    // Centralized resource manager
    auto& resourceManager = RaylibCPP::ResourceManager::getInstance();
    resourceManager.setAssetPath("assets/");
    
    // Advanced 3D camera configuration
    RaylibCPP::Camera camera(RaylibCPP::Camera::Mode::CAMERA_FREE);
    camera.setPosition({15.0f, 15.0f, 15.0f})
          .setTarget({0.0f, 0.0f, 0.0f})
          .setFOV(60.0f)
          .setNearPlane(0.1f)
          .setFarPlane(1000.0f);
    
    // Système d'entrées avec callbacks
    auto& input = RaylibCPP::InputManager::getInstance();
    input.onKeyPressed(KEY_ESCAPE, []() { /* quit application */ });
    input.onMouseWheel([&camera](float delta) { camera.zoom(delta); });
    
    while (!window.shouldClose()) {
        input.update();
        
        window.beginDrawing();
        window.clearBackground({20, 20, 40, 255});
        
        camera.beginMode3D();
        renderScene(resourceManager, camera);
        camera.endMode3D();
        
        renderUI();
        
        window.endDrawing();
    }
    
    return 0;
}
```

### Gestion Avancée des Modèles 3D

```cpp
// Chargement et optimisation de modèles
class ModelManager {
private:
    std::unordered_map<std::string, std::shared_ptr<RaylibCPP::Model>> m_models;
    std::thread m_loaderThread;
    
public:
    std::shared_ptr<RaylibCPP::Model> loadModel(const std::string& path) {
        auto model = std::make_shared<RaylibCPP::Model>();
        
        // Asynchronous loading with callback
        m_loaderThread = std::thread([model, path]() {
            model->loadFromFile(path)
                 .generateMipmaps()
                 .optimizeMesh()
                 .calculateBoundingBox();
        });
        
        return model;
    }
    
    void renderModel(const std::string& name, const Vector3& position, float scale) {
        if (auto model = m_models[name]) {
            model->setPosition(position)
                 .setScale(scale)
                 .setTint({255, 255, 255, 255})
                 .render();
        }
    }
};
```

### Custom Shader System

```cpp
// Using custom shaders
class CustomShader {
private:
    RaylibCPP::Shader m_shader;
    
public:
    void initialize() {
        m_shader.loadFromFile("shaders/lighting.vs", "shaders/lighting.fs");
        
        // Configure uniforms
        m_shader.setUniform("lightPosition", Vector3{10.0f, 10.0f, 10.0f});
        m_shader.setUniform("lightColor", Vector3{1.0f, 1.0f, 1.0f});
        m_shader.setUniform("ambientStrength", 0.3f);
    }
    
    void render(const RaylibCPP::Model& model) {
        m_shader.begin();
        
        // Update dynamic uniforms
        m_shader.setUniform("time", GetTime());
        m_shader.setUniform("viewMatrix", camera.getViewMatrix());
        
        model.render();
        
        m_shader.end();
    }
};
```

## Technical Advantages of RaylibCPP

### Modern Memory Management
- **Automatic RAII:** Guaranteed cleanup of OpenGL resources (textures, VBOs, shaders)
- **Smart Pointers:** Use of `std::shared_ptr` and `std::unique_ptr` to prevent leaks
- **Pool Allocators:** Optimized memory management for frequently allocated objects
- **GPU Memory Management:** Automatic CPU/GPU synchronization with double buffering

### Performance and Optimizations
- **Batch Rendering:** Automatic grouping of similar render calls
- **Frustum Culling:** Automatic elimination of objects outside field of view
- **LOD System:** Adaptive level of detail based on distance to camera
- **Multi-threading:** Background asset loading and calculations

### Modern C++ API
- **Method Chaining:** Fluent interface for object configuration
- **Template Metaprogramming:** Compile-time optimizations
- **Exception Safety:** Robust error handling with RAII
- **STL Integration:** Native compatibility with STL containers and algorithms

### Debugging and Profiling
- **Performance Metrics:** Real-time metrics (FPS, draw calls, rendered triangles)
- **Memory Profiler:** GPU/CPU memory usage tracking
- **OpenGL Debug:** Automatic OpenGL state validation in debug mode
- **Asset Inspector:** Inspection tools for loaded models, textures, and shaders

## Compilation and Integration

### Modern CMake Configuration

```cmake
# CMakeLists.txt for a project using RaylibCPP
cmake_minimum_required(VERSION 3.16)
project(ZappyGUI VERSION 1.0.0 LANGUAGES CXX)

# C++17 configuration with optimizations
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG -fsanitize=address")

# Recherche des dépendances
find_package(OpenGL REQUIRED)
find_package(raylib 4.5 QUIET)

# Configuration RaylibCPP
add_subdirectory(libs/RaylibCPP)

# Création de l'exécutable
add_executable(zappy_gui
    src/main.cpp
    src/GameLoop.cpp
    src/renderer/MapRenderer.cpp
    # ... autres sources
)

# Liaison des bibliothèques
target_link_libraries(zappy_gui
    PRIVATE
        RaylibCPP::RaylibCPP
        OpenGL::GL
        ${CMAKE_DL_LIBS}
        pthread
)

# Configuration des includes
target_include_directories(zappy_gui
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/RaylibCPP/include
)
```

### Compilation Manuelle

```bash
# Compilation optimisée pour la production
g++ -std=c++17 -O3 -march=native \
    -Ilibs/RaylibCPP/include \
    -Isrc \
    src/*.cpp \
    libs/RaylibCPP/src/*.cpp \
    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
    -o zappy_gui

# Compilation debug avec sanitizers
g++ -std=c++17 -g -O0 -DDEBUG \
    -fsanitize=address -fsanitize=undefined \
    -Ilibs/RaylibCPP/include \
    src/*.cpp \
    libs/RaylibCPP/src/*.cpp \
    -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 \
    -o zappy_gui_debug
```

## Intégration avec Zappy

### Architecture du Rendu 3D

RaylibCPP est utilisé dans Zappy pour créer une expérience visuelle immersive :

```cpp
// Exemple d'utilisation dans le moteur de rendu Zappy
class ZappyRenderer {
private:
    RaylibCPP::Window m_window;
    RaylibCPP::Camera m_camera;
    RaylibCPP::ResourceManager m_resourceManager;
    
    // Modèles spécialisés pour Zappy
    std::shared_ptr<RaylibCPP::Model> m_playerModel;
    std::shared_ptr<RaylibCPP::Model> m_tileModel;
    std::shared_ptr<RaylibCPP::Model> m_resourceModels[7]; // 7 types de ressources
    
    // Système de particules pour les effets
    ParticleSystem m_incantationEffect;
    ParticleSystem m_deathEffect;
    
public:
    void initialize() {
        m_window.setSize(1920, 1080)
                .setTitle("Zappy - Visualisation 3D")
                .setTargetFPS(60)
                .create();
        
        // Chargement des assets Zappy
        loadZappyAssets();
        
        // Configuration de la caméra isométrique
        setupIsometricCamera();
    }
    
    void renderGameWorld(const GameState& state) {
        m_window.beginDrawing();
        m_window.clearBackground({30, 30, 50, 255});
        
        m_camera.beginMode3D();
        
        // Rendu de la carte avec LOD
        renderMap(state.map);
        
        // Rendu des joueurs avec animations
        renderPlayers(state.players);
        
        // Rendu des ressources avec instancing
        renderResources(state.resources);
        
        // Effets spéciaux (incantations, morts)
        renderEffects(state.events);
        
        m_camera.endMode3D();
        
        // Interface utilisateur 2D
        renderUI(state);
        
        m_window.endDrawing();
    }
};
```

### Optimisations Spécifiques Zappy

**Rendu de Carte Large :**
- **Instanced Rendering :** Rendu efficace de milliers de tuiles identiques
- **Terrain Chunking :** Division de la carte en chunks pour l'optimisation LOD
- **Texture Atlasing :** Regroupement des textures de ressources pour réduire les draw calls

**Animation des Joueurs :**
- **Skeletal Animation :** Animation fluide des modèles de joueurs
- **State Interpolation :** Interpolation des positions pour des mouvements fluides
- **Animation Blending :** Transition douce entre les animations (marche, collecte, incantation)

**Effets Visuels :**
- **Particle Systems :** Effets d'incantation, de mort, de collecte de ressources
- **Dynamic Lighting :** Éclairage dynamique pour les incantations et événements spéciaux
- **Post-processing :** Effets de bloom, color grading, ambient occlusion

## Classes Principales et API

### Core Classes

**RaylibCPP::Window**
```cpp
class Window {
public:
    Window& setSize(int width, int height);
    Window& setTitle(const std::string& title);
    Window& setTargetFPS(int fps);
    Window& setVSync(bool enabled);
    Window& setFullscreen(bool fullscreen);
    Window& setAntialiasing(int samples);
    
    bool create();
    bool shouldClose() const;
    void beginDrawing();
    void endDrawing();
    void clearBackground(const Color& color);
    
    // Getters
    Vector2 getSize() const;
    int getFPS() const;
    float getDeltaTime() const;
};
```

**RaylibCPP::Camera**
```cpp
class Camera {
public:
    enum class Mode { CAMERA_FREE, CAMERA_ORBITAL, CAMERA_FIRST_PERSON };
    
    Camera(Mode mode = Mode::CAMERA_FREE);
    
    Camera& setPosition(const Vector3& position);
    Camera& setTarget(const Vector3& target);
    Camera& setFOV(float fov);
    Camera& setNearPlane(float nearPlane);
    Camera& setFarPlane(float farPlane);
    
    void beginMode3D();
    void endMode3D();
    void update();
    
    // Contrôles avancés
    void zoom(float delta);
    void rotate(float deltaX, float deltaY);
    void pan(const Vector2& delta);
    
    // Getters
    Vector3 getPosition() const;
    Vector3 getTarget() const;
    Matrix getViewMatrix() const;
    Matrix getProjectionMatrix() const;
};
```

**RaylibCPP::Model**
```cpp
class Model {
public:
    Model();
    ~Model();
    
    bool loadFromFile(const std::string& path);
    Model& setPosition(const Vector3& position);
    Model& setRotation(const Vector3& rotation);
    Model& setScale(float scale);
    Model& setTint(const Color& tint);
    
    void render();
    void renderInstanced(const std::vector<Matrix>& transforms);
    
    // Animation
    bool loadAnimation(const std::string& path);
    void playAnimation(const std::string& name, bool loop = true);
    void updateAnimation(float deltaTime);
    
    // Optimisations
    Model& generateMipmaps();
    Model& optimizeMesh();
    Model& calculateBoundingBox();
    
    BoundingBox getBoundingBox() const;
    bool isLoaded() const;
};
```

**RaylibCPP::ResourceManager**
```cpp
class ResourceManager {
public:
    static ResourceManager& getInstance();
    
    void setAssetPath(const std::string& path);
    
    // Chargement synchrone
    std::shared_ptr<Model> loadModel(const std::string& path);
    std::shared_ptr<Texture> loadTexture(const std::string& path);
    std::shared_ptr<Shader> loadShader(const std::string& vs, const std::string& fs);
    
    // Chargement asynchrone
    std::future<std::shared_ptr<Model>> loadModelAsync(const std::string& path);
    std::future<std::shared_ptr<Texture>> loadTextureAsync(const std::string& path);
    
    // Gestion du cache
    void clearCache();
    size_t getCacheSize() const;
    void preloadAssets(const std::vector<std::string>& paths);
    
    // Métriques
    size_t getMemoryUsage() const;
    size_t getLoadedResourceCount() const;
};
```

### Utility Classes

**RaylibCPP::Math**
```cpp
namespace Math {
    // Vecteurs et matrices
    Vector3 lerp(const Vector3& a, const Vector3& b, float t);
    Vector3 slerp(const Vector3& a, const Vector3& b, float t);
    Matrix createTransform(const Vector3& position, const Vector3& rotation, float scale);
    
    // Intersections et collisions
    bool rayIntersectSphere(const Ray& ray, const Vector3& center, float radius);
    bool rayIntersectBox(const Ray& ray, const BoundingBox& box);
    bool boxIntersectBox(const BoundingBox& a, const BoundingBox& b);
    
    // Optimisations mathématiques
    float fastSqrt(float x);
    float fastInvSqrt(float x);
    Vector3 fastNormalize(const Vector3& v);
}
```

**RaylibCPP::Debug**
```cpp
class Debug {
public:
    static void enableProfiling(bool enable);
    static void drawBoundingBox(const BoundingBox& box, const Color& color);
    static void drawWireframe(const Model& model, const Color& color);
    static void drawNormals(const Model& model, float length = 1.0f);
    
    // Métriques de performance
    static float getFrameTime();
    static int getDrawCalls();
    static int getTriangleCount();
    static size_t getVideoMemoryUsage();
    
    // Logging
    static void log(const std::string& message);
    static void logError(const std::string& error);
    static void logWarning(const std::string& warning);
};
```
