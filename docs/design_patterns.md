# Design Patterns in Zappy Graphical Interface

## Overview

The Zappy project's graphical interface implements a sophisticated architecture based on several modern design patterns. This modular architecture ensures maintainability, extensibility, and separation of concerns in a complex GUI system for real-time game visualization.

## Implemented Design Patterns

### 1. Factory Pattern

#### UIWindowFactory
The Factory pattern is used in a sophisticated manner to encapsulate the creation and management of different interface windows.

**Main Class:** `UIWindowFactory`

**Responsibilities:**
- Creation and initialization of all window instances
- Centralized and typed access to all windows
- Complete window lifecycle management (creation, update, destruction)
- Coordination of interactions between windows
- Mouse event and selection management

**Advantages:**
- Complete centralization of window creation
- Maximum ease for adding new window types
- Clear separation of responsibilities between creation and usage
- Unified management of dependencies between windows

```cpp
static const std::unordered_map<std::string, std::function<std::shared_ptr<GUI::IUIWindow>(std::shared_ptr<IGuiLib>)>> windowCreators = {
    {"tileInfo", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::TileInfoWindow>(lib); }},
    {"playerInfo", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::PlayerInfoWindow>(lib); }},
    {"mapInfo", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::MapInfoWindow>(lib); }},
    {"timeInfo", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::TimeInfoWindow>(lib); }},
    {"menu", [](std::shared_ptr<IGuiLib> lib) { return std::make_shared<GUI::MenuWindow>(lib); }}
};
```

#### EntityFactoryManager
The project also uses a Factory pattern for creating game entities.

**Main Class:** `EntityFactoryManager`

**Responsibilities:**
- Creation of business objects (Player, Egg, Tile, Resource, etc.)
- Abstraction of entity creation logic
- Facilitation of unit testing through dependency injection

```cpp
class IEntityFactory {
public:
    virtual ~IEntityFactory() = default;
    virtual std::shared_ptr<IPlayer> createPlayer(const PlayerInfoData& data) const = 0;
    virtual std::shared_ptr<IPlayerInventory> createPlayerInventory(const PlayerInventoryData& data) const = 0;
    virtual std::shared_ptr<IEgg> createEgg(const EggData& data) const = 0;
    virtual std::shared_ptr<ITile> createTile(int x, int y) const = 0;
    virtual std::shared_ptr<IResource> createResource(ResourceType type, int quantity = 0) const = 0;
};
```

#### NetworkFactory
The network system also uses the Factory pattern to create network components.

**Main Class:** `NetworkFactory`

```cpp
class INetworkFactory {
public:
    virtual ~INetworkFactory() = default;
    virtual std::unique_ptr<ISocket> createTcpSocket() = 0;
    virtual std::unique_ptr<INetworkAddress> createAddress(const std::string& host, int port) = 0;
    virtual std::unique_ptr<IPoller> createPoller() = 0;
};
```

### 2. Strategy Pattern

#### Tile Rendering
The Strategy pattern is used to encapsulate different tile rendering approaches based on zoom level and performance.

**Main Interface:** `ITileRenderStrategy`

**Available Strategies:**
- `SimpleTileRenderStrategy`: Simple rendering (colored cubes)
- `DetailedTileRenderStrategy`: Detailed rendering with resource visualization
- `ModelTileRenderStrategy`: Rendering with 3D models

```cpp
class ITileRenderStrategy {
public:
    virtual ~ITileRenderStrategy() = default;
    virtual void renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        int x, int y, float tileSize, float spacing) = 0;
};
```

**Advantages:**
- Runtime algorithm change for rendering
- Dynamic adaptation according to zoom level
- Automatic performance optimization
- Extensibility for new rendering strategies

### 3. Observer Pattern

#### Interface Updates
The Observer pattern is used for automatic view updates when game state changes.

**Components:**
- `UIContext`: Centralized management of update callbacks
- `IUIDataProvider`: Notification of data changes
- UI Windows: Observers of state changes

```cpp
class UIContext : public IUIContext {
private:
    std::map<std::string, std::function<void()>> _dataUpdateCallbacks;
    
public:
    void registerDataUpdateCallback(std::function<void()> callback) override;
    void unregisterDataUpdateCallback(const std::string& callbackId) override;
    void notifyDataUpdate();
};
```

#### Network Message System
The network system also uses the Observer pattern for handling incoming messages.

```cpp
void NetworkManager::setMessageHandler(MessageHandler handler);
using MessageHandler = std::function<void(const Message&)>;
```

### 4. Command Pattern

#### Command Execution
The Command pattern is used to encapsulate network commands and their execution.

**Main Interfaces:**
- `ICommand`: Interface for all commands
- `ICommandExecutor`: Interface for command execution
- `INetworkCommandSender`: Interface for sending network commands

```cpp
class ICommandExecutor {
public:
    virtual ~ICommandExecutor() = default;
    virtual void executeCommand(std::shared_ptr<ICommand> command) = 0;
};

class INetworkCommandSender {
public:
    virtual ~INetworkCommandSender() = default;
    virtual void sendCommand(const std::string& command) = 0;
    virtual bool isConnected() const = 0;
};
```

**Implementation:**
- `ComponentCoordinator`: Command coordination between components
- `NetworkCommandManager`: Specialized network command management
- `GameNetworkCommand`: Game-specific commands

```cpp
class ComponentCoordinator : public ICommandExecutor {
public:
    void executeCommand(std::shared_ptr<ICommand> command) override {
        if (command && command->getDescription().length() > 0) {
            if (auto nm = _networkManager.lock())
                nm->sendCommand(command->getDescription());
        }
    }
};
```

### 5. Bridge Pattern

#### Graphics Abstraction
The Bridge pattern is used to separate abstraction (graphical interface) from its implementation (graphics library).

**Components:**
- `IGraphicsLib`: Abstraction interface for graphics operations
- `IGuiLib`: Abstraction interface for UI components
- `TypeAdapter`: Type adapter between systems

```cpp
class IGuiLib {
public:
    virtual ~IGuiLib() = default;
    virtual bool ButtonPressed(float x, float y, float width, float height, const std::string& text) = 0;
    virtual void DrawLabel(float x, float y, float width, float height, const std::string& text) = 0;
    virtual bool DrawCheckBox(float x, float y, float width, float height, const std::string& text, bool checked) = 0;
    // ... other interface methods
};
```

**Advantage:** Complete decoupling between interface logic and graphics implementation (Raylib).

### 6. Adapter Pattern

#### Type Adaptation
The Adapter pattern is used to adapt types between different systems.

**Main Class:** `TypeAdapter`

```cpp
namespace TypeAdapter {
    inline ::Vector2 ToRaylib(const ZappyTypes::Vector2& vec);
    inline ZappyTypes::Vector2 FromRaylib(const ::Vector2& vec);
    inline ::Color ToRaylib(const ZappyTypes::Color& color);
    inline ZappyTypes::Color FromRaylib(const ::Color& color);
    inline ::Rectangle ToRaylib(const ZappyTypes::Rectangle& rect);
    inline ZappyTypes::Rectangle FromRaylib(const ::Rectangle& rect);
}
```

#### Game State Adaptation
`GameStateAdapter` adapts the interface between the state manager and rendering components.

```cpp
class GameStateAdapter {
public:
    explicit GameStateAdapter(std::shared_ptr<IGameStateManager> gameStateManager);
    
    int getMapWidth() const;
    int getMapHeight() const;
    std::shared_ptr<const ITile> getTile(int x, int y) const;
    std::shared_ptr<const IPlayer> getPlayerInfo(int playerId) const;
    void addOrUpdatePlayer(const PlayerInfoData& playerData);
    void updateTileResources(int x, int y, int food, int linemate, int deraumere,
        int sibur, int mendiane, int phiras, int thystame);
};
```

### 7. Provider Pattern

#### UI Data Provider
The Provider pattern centralizes access to game data for the user interface.

**Main Class:** `UIDataProvider`

**Responsibilities:**
- Global game state
- Temporal data (frequency, game time, FPS)
- Selections (selected tiles and players)
- User interface configuration

```cpp
class IUIDataProvider {
public:
    virtual ~IUIDataProvider() = default;
    virtual int getMapWidth() const = 0;
    virtual int getMapHeight() const = 0;
    virtual float getGameTime() const = 0;
    virtual int getFrequency() const = 0;
    virtual std::shared_ptr<const ITile> getTile(int x, int y) const = 0;
    virtual std::shared_ptr<const IPlayer> getPlayerInfo(int playerId) const = 0;
};
```

### 8. Template Method Pattern

#### Abstract Windows
The Template Method pattern is used in the UI window hierarchy.

**Abstract Class:** `AUIWindow`

```cpp
class AUIWindow : public IUIWindow {
public:
    bool render() override {
        // Common rendering logic
        if (_showWindowBox) {
            _guiLib->DrawWindowBox(_position.x, _position.y, _dimensions.x, _dimensions.y, _title);
        }
        
        // Delegation to specific rendering
        renderContent();
        
        return true;
    }

protected:
    virtual void renderContent() = 0; // Method to be implemented by subclasses
};
```

### 9. Singleton Pattern

#### Model Manager
The Singleton pattern is used for the 3D model manager.

**Class:** `ModelManager` (via `ModelManagerAdapter`)

```cpp
class ModelManagerAdapter {
public:
    explicit ModelManagerAdapter() : managerRef(ModelManager::getInstance()) {}
    ModelManager& getManager() { return managerRef; }
    static std::shared_ptr<ModelManagerAdapter> createShared() {
        return std::make_shared<ModelManagerAdapter>();
    }

private:
    ModelManager& managerRef;
};
```

### 10. Mediator Pattern

#### Component Coordination
The Mediator pattern is used to coordinate interactions between different system components.

**Main Class:** `ComponentCoordinator`

```cpp
class ComponentCoordinator : public ICommandExecutor {
public:
    void setNetworkManager(std::shared_ptr<NetworkManager> networkManager);
    void setGameController(std::shared_ptr<IGameController> gameController);
    void setUINotifier(std::shared_ptr<IUINotifier> uiNotifier);
    void setupConnections();

private:
    void onMessageReceived(const Message& message);
};
```

## Global Architecture and Interactions

### Main Data Flow

1. **Network → Parsing → Game State**
   - `NetworkManager` receives messages
   - `ProtocolParser` parses messages (Strategy pattern for different types)
   - `GameController` updates state via handlers

2. **Game State → User Interface**
   - `UIDataProvider` provides data access (Provider pattern)
   - `UIWindowFactory` coordinates windows (Factory pattern)
   - Each window updates via Observer pattern

3. **Interface → Network Commands**
   - User interactions generate commands (Command pattern)
   - `ComponentCoordinator` routes commands (Mediator pattern)
   - `NetworkManager` sends commands to server

### Architecture Advantages

1. **Modularity**: Each component has a clear and well-defined responsibility
2. **Extensibility**: New window types, rendering strategies, and commands easily added
3. **Testability**: Interfaces allow mock injection for testing
4. **Maintainability**: Separation of concerns and low coupling
5. **Reusability**: Independent components reusable in other contexts
6. **Flexibility**: Implementation changes without impact on other components

### Identified Extension Points

1. **Plugin System**: Architecture ready for plugin-based windows
2. **Customizable Themes**: Easy support for visual themes
3. **Configuration Saving**: User parameter persistence
4. **Multi-screen**: Native support for multi-screen configurations
5. **Internationalization**: Architecture ready for translation

## Conclusion

The Zappy graphical interface architecture demonstrates expert use of modern design patterns. This approach not only facilitates adding new features but also ensures high maintainability and clear separation of responsibilities. The combination of Factory, Strategy, Observer, Command, and Bridge patterns creates a coherent and efficient system that can easily evolve with future project needs.

The implementation respects SOLID principles and favors a hexagonal architecture where implementation details are isolated from business rules, thus ensuring a solid foundation for future development.
