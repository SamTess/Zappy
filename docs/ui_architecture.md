# Zappy GUI User Interface Architecture

## General Overview

The user interface (UI) of the Zappy GUI project has been developed using several advanced design patterns to improve its modularity, maintainability and extensibility. This modern architecture allows efficient management of specialized windows and smooth interaction with the game system.

## Class Hierarchy

The UI system is based on a clear and extensible class hierarchy:

```
IUIWindow (Interface)
    |
    +-- AUIWindow (Abstract class)
            |
            +-- TileInfoWindow      (Detailed tile information)
            +-- PlayerInfoWindow    (Player statistics and actions)
            +-- TimeInfoWindow      (Temporal information and performance)
            +-- MenuWindow          (Global application configuration)
            +-- MapInfoWindow       (General map information)
            +-- [future windows to be added easily]
```

### Main Components

1. **UserInterface**: Main entry point for UI, coordinates all interactions
2. **UIWindowFactory**: Factory responsible for creation, management and coordination of windows
3. **IUIWindow**: Interface defining the strict contract for all UI windows
4. **AUIWindow**: Abstract class implementing common behaviors and basic logic
5. **Specialized windows**: Concrete classes for each window type with dedicated functionalities
6. **UIContext**: Shared context containing references to services and data
7. **UIDataProvider**: Centralized data provider for all windows

## Design Patterns Used

### Factory Pattern

The Factory pattern is used sophisticatedly to encapsulate the creation and management of different windows. The `UIWindowFactory` class is responsible for:

**Main functionalities:**
- Create and initialize all window instances
- Provide centralized and typed access to all windows
- Manage the complete lifecycle of windows (creation, update, destruction)
- Coordinate interactions between windows
- Manage mouse events and selections

**Advantages:**
- Complete centralization of window creation
- Maximum ease for adding new window types
- Clear separation of responsibilities between creation and usage
- Unified management of dependencies between windows

### Interface and Abstract Class

The combined use of an interface (`IUIWindow`) and an abstract class (`AUIWindow`) allows:

**Interface benefits:**
- Define a strict and non-negotiable contract for all windows
- Guarantee consistency of method signatures
- Facilitate unit testing with mocks

**Abstract class benefits:**
- Share common code (visibility management, positioning, state)
- Provide default implementations for standard behaviors
- Facilitate extensibility with minimal code duplication

### Provider Pattern

The `UIDataProvider` centralizes access to game data:
- **GameState**: Global game state
- **Temporal data**: Frequency, game time, FPS
- **Selections**: Selected tiles and players
- **Configuration**: User interface settings

## Detailed Window Architecture

### TileInfoWindow
**Responsibilities:**
- Detailed display of selected tile information
- Visualization of present resources with quantities
- List of players present on the tile
- Tile coordinates and statistics

### PlayerInfoWindow
**Responsibilities:**
- Complete player information (level, orientation, team)
- Detailed inventory with all resources
- Interactive commands (player tracking, extended information)
- Recent action history

### TimeInfoWindow
**Responsibilities:**
- Real-time temporal information (server frequency, game time)
- Performance metrics (FPS, network latency)
- Simulation statistics
- Game speed controls

### MenuWindow
**Responsibilities:**
- Audio configuration (music volume, sound effects)
- Gameplay settings (speed, UI transparency)
- Window visibility management
- Default position reset
- Rendering and display settings

### MapInfoWindow
**Responsibilities:**
- General map information (dimensions, total resources)
- Team and player statistics
- Overview of the playing field
- Map and zoom controls

## How to Add a New Window

### 1. Create the window class
```cpp
// NewWindow.hpp
class NewWindow : public AUIWindow {
public:
    NewWindow(std::shared_ptr<IGuiLib> guiLib);
    ~NewWindow() = default;
    
    // Methods specific to this window
    void setSpecificData(const SpecificData& data);
    
protected:
    void renderContent() override;
    void updateSpecificData(std::shared_ptr<const GameState> gameState) override;
    
private:
    // Data specific to this window
    SpecificData _specificData;
};
```

### 2. Implement the abstract methods
```cpp
// NewWindow.cpp
void NewWindow::renderContent() {
    // Specific rendering logic
    _guiLib->DrawLabel(10, 30, 200, 20, "Specific content");
    // ... other interface elements
}

void NewWindow::updateSpecificData(std::shared_ptr<const GameState> gameState) {
    // Update specific data from game state
    if (gameState) {
        // Extract and process necessary data
    }
}
```

### 3. Integrate into the factory
```cpp
// In UIWindowFactory::createAllWindows()
_windows["newWindow"] = std::make_shared<NewWindow>(_guiLib);
_windows["newWindow"]->setUIContext(_uiContext);
```

### 4. Add access methods
```cpp
// In UIWindowFactory.hpp
std::shared_ptr<NewWindow> getNewWindow() const;

// In UIWindowFactory.cpp
std::shared_ptr<NewWindow> UIWindowFactory::getNewWindow() const {
    return std::dynamic_pointer_cast<NewWindow>(_windows.at("newWindow"));
}
```

## Window Interaction

### Communication System
Windows communicate through several mechanisms:

**1. UIWindowFactory as mediator:**
- Centralized coordination of interactions
- Propagation of selections (tiles, players)
- State synchronization between windows

**2. Selection events:**
```cpp
// Tile selection
void UIWindowFactory::setSelectedTile(int x, int y) {
    auto tileInfoWindow = getTileInfoWindow();
    if (tileInfoWindow) {
        tileInfoWindow->setSelectedTile(x, y);
    }
    // Update other windows if necessary
}
```

**3. Shared UIContext:**
- Uniform access to services (network, commands)
- Sharing of common data
- Accessible global configuration

## Advanced Event Management

### Mouse Event Management System
The event management system is centralized in `UserInterface` and intelligently delegated:

**1. Interaction detection:**
```cpp
bool UserInterface::handleMouseEvents() {
    ZappyTypes::Vector2 mousePos = _guiLib->GetMousePosition();
    
    // Check interactions with windows
    if (_windowFactory->handleMouseEvents(mousePos)) {
        return true; // Event consumed by UI
    }
    
    return false; // Event free for camera/map
}
```

**2. Window dragging:**
- Automatic detection of title areas
- Smooth dragging with constraints
- Saving custom positions

**3. Adaptive resizing:**
- Automatic adjustment according to screen resolution
- Intelligent default positions
- Easy reset to original values

## Performance and Optimizations

### Conditional Rendering
- Rendering only visible windows
- Differential data updates
- Caching of expensive calculations

### Memory Management
- Intelligent resource sharing via smart pointers
- Automatic release of unused resources
- Window pool to avoid repeated allocations

## Future Extensibility

### Identified Extension Points
1. **Plugin System**: Architecture ready for plugin-based windows
2. **Customizable Themes**: Easy support for visual themes
3. **Configuration Saving**: User settings persistence
4. **Multi-screen**: Native support for multi-screen configurations
5. **Internationalization**: Architecture ready for translation

### Envisioned New Windows
- **StatisticsWindow**: Graphs and advanced statistics
- **DebugWindow**: Debug tools for developers
- **ReplayWindow**: Replay and recording controls
- **ChatWindow**: Chat interface between spectators

## Conclusion

This refactored and enriched architecture provides an extremely solid and extensible foundation for the Zappy GUI user interface. It not only facilitates the addition of new features, but also ensures high maintainability and clear separation of responsibilities. The combination of Factory, Provider and Observer patterns creates a coherent and efficient system that can easily evolve with the future needs of the project.
