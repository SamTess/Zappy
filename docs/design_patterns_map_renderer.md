# Design Patterns Used for Map Rendering

## Problem Statement
Implement map rendering from server data using a flexible architecture that can adapt to different types of rendering and easily change visualization strategies.

## Implemented Design Patterns

### 1. Observer Pattern
The Observer pattern is used to react to changes in the `GraphicalContext`. It allows decoupling the data update logic (in `GraphicalContext`) from the display logic (in `MapRenderer`).

- `IGraphicalContextObserver`: Interface for observers
- `MapRendererObserver`: Concrete observer that adapts rendering based on changes

Advantage: Automatic rendering updates when data changes, without strong coupling between classes.

### 2. Strategy Pattern
The Strategy pattern is used to encapsulate different tile rendering approaches.

- `ITileRenderStrategy`: Interface defining common behavior
- `SimpleTileRenderStrategy`: Implementation for simple rendering (cubes)
- `ModelTileRenderStrategy`: Implementation for rendering with 3D models
- `DetailedTileRenderStrategy`: Implementation for detailed rendering with resource visualization

Advantage: Allows changing the tile rendering algorithm at runtime, without modifying client code.

### 3. Factory Pattern
The Factory pattern is used to create different rendering strategies.

- `TileRenderStrategyFactory`: Factory that creates different strategies

Advantage: Centralizes creation logic and facilitates strategy changes.

### 4. Bridge Pattern
The Bridge pattern is used to separate abstraction (map rendering) from its implementation (graphics library).

- `MapRenderer`: Abstraction that uses an implementation
- `IGraphicsLib`: Implementation interface

Advantage: Decoupling between logical rendering and underlying graphics library.

### 5. Level of Detail (LOD) Pattern
The Level of Detail pattern is used to adapt rendering detail level based on zoom level.

- Distant zoom: Use of simple or model strategies for optimal performance
- Close zoom: Use of detailed strategy to display resources individually

Advantage: Optimizes performance while providing a rich user experience.

## Modular Design

The architecture allows:
1. **Flexibility**: Easily change tile appearance without modifying renderer code
2. **Extensibility**: Add new rendering strategies without disturbing existing code
3. **Reusability**: Components are independent and can be reused in other parts of the application
4. **Decoupling**: Rendering is decoupled from game logic and data
5. **Adaptability**: The system adapts to different map scales and zoom levels

## ModelManager Usage

The `ModelManager` is integrated via the Strategy pattern, which allows:

1. Using 3D models for tiles when desired
2. Maintaining a simple default implementation
3. Changing the model used at runtime

To define specific rendering by resource:
```cpp
// Example of using ModelManager for different resource types
modelManager.loadModel("../assets/models/food.obj"); // ID 1
mapRenderer.setResourceRenderStrategy(
   static_cast<int>(ResourceType::FOOD), 
   strategyFactory.createModelTileStrategy(1)
);
```

## Detailed Resource Visualization

The system allows displaying resource details when the user zooms into an area:

1. Resources are visualized as small 3D objects on tiles
2. The quantity of each resource is represented visually
3. Players and incantations are clearly indicated
4. The transition between different detail levels is smooth

## Performance and Optimizations

For large maps:
1. The renderer automatically adjusts tile size to fit
2. The rendering strategy is simplified for distant tiles
3. The system observes only tiles that change to minimize updates
4. Adaptive level of detail maintains smooth performance

## Integration Example

Here's an example of integrating MapRenderer into the GameLoop:

```cpp
// In GameLoop.hpp
private:
    // ...
    std::shared_ptr<GraphicalContext> m_context;
    std::shared_ptr<MapRenderer> m_mapRenderer;
    float m_currentZoomLevel;

// In GameLoop.cpp
void GameLoop::setupComponents() {
    // Initialize graphics context
    m_context = std::make_shared<GraphicalContext>();
    
    // Get ModelManager instance
    auto& modelManager = ModelManager::getInstance();
    
    // Create MapRenderer
    m_mapRenderer = std::make_shared<MapRenderer>(m_graphics, m_context, modelManager);
    m_mapRenderer->initialize();
    m_currentZoomLevel = 1.0f;
    
    // Preload some models for special tiles
    int foodModelId = modelManager.loadModel("../assets/models/food.obj");
    int crystalModelId = modelManager.loadModel("../assets/models/crystal.obj");
    
    // Configure specific rendering strategies for certain resources
    m_mapRenderer->setResourceRenderStrategy(
        static_cast<int>(ResourceType::FOOD),
        m_mapRenderer->getStrategyFactory().createModelTileStrategy(foodModelId)
    );
    
    m_mapRenderer->setResourceRenderStrategy(
        static_cast<int>(ResourceType::THYSTAME),
        m_mapRenderer->getStrategyFactory().createModelTileStrategy(crystalModelId)
    );
}

void GameLoop::render() {
    // Check if user is zooming in/out
    float mouseWheel = m_graphics->GetMouseWheelMove();
    if (mouseWheel != 0.0f) {
        // Update zoom level
        m_currentZoomLevel += mouseWheel * 0.1f;
        m_currentZoomLevel = std::max(0.5f, std::min(5.0f, m_currentZoomLevel));
        m_mapRenderer->setZoomLevel(m_currentZoomLevel);
        
        // Also adjust camera
        m_camera->setZoomLevel(m_currentZoomLevel);
    }
    
    // Begin rendering
    m_graphics->BeginDrawing();
    m_graphics->ClearBackground({20, 20, 40, 255}); // Dark blue background
    
    m_graphics->BeginMode3D(m_camera->getCamera());
    
    // Render map
    m_mapRenderer->render();
    
    m_graphics->EndMode3D();
    
    // Render user interface
    m_uiRenderer->render(m_graphics, m_gui);
    
    m_graphics->EndDrawing();
}
```

## Large Map Management

For large maps, the system implements several optimizations:

1. **Automatic size adjustment** - Tiles are automatically resized:
```cpp
void MapRendererObserver::onMapSizeChanged(int width, int height) {
    if (width > 20 || height > 20) {
        float newSize = 10.0f / std::max(width, height);
        renderer.setTileSize(newSize);
        renderer.setTileSpacing(newSize * 0.1f);
    }
}
```

2. **Frustum culling** - Extension option to render only visible tiles:
```cpp
// Potential code for frustum culling (not implemented in current version)
bool isTileVisible(int x, int y, const Camera3D& camera) {
    ZappyTypes::Vector3 position = {
        x * (tileSize + tileSpacing),
        0.0f,
        y * (tileSize + tileSpacing)
    };
    return checkPointInFrustum(position, camera);
}
```
