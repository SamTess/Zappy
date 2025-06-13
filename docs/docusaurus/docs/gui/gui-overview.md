---
sidebar_position: 1
---

# Graphical Interface Documentation

## Overview

The Zappy graphical interface is developed in C++ using Raylib for 3D rendering. It provides real-time visualization of the game state with an intuitive user interface.

## GUI Architecture

### Main Components

```cpp
class GameLoop : public IGraphicalContextObserver {
private:
    std::shared_ptr<IGraphicsLib> m_graphics;      // Graphics abstraction (Raylib)
    GraphicalContext m_context;                    // Graphical context
};
```

## Dynamic Library System

### Library Abstraction

The project uses an abstract interface system to decouple components:

```cpp
// Abstract graphics interface
class IGraphicsLib {
public:
    virtual bool init(int width, int height, const std::string& title) = 0;
};
```

### Library Manager

```cpp
class LibraryManager {
private:
    std::unordered_map<std::string, void*> m_loadedLibraries;
    
public:
    template<typename T>
    std::shared_ptr<T> getInterface(const std::string& libraryName, 
                                   const std::string& factoryFunction) {
        void* handle = loadLibrary(libraryName);
        if (!handle) return nullptr;
        
        typedef T* (*FactoryFunc)();
        FactoryFunc factory = (FactoryFunc)dlsym(handle, factoryFunction.c_str());
        
        if (factory) {
            return std::shared_ptr<T>(factory());
        }
        return nullptr;
    }
    
private:
    void* loadLibrary(const std::string& name);
    void unloadLibrary(const std::string& name);
};
```

## 3D Rendering System

### Map Rendering

```cpp
class MapRenderer {
public:
    void renderMap(const std::vector<std::vector<TileData>>& mapData) {
        for (int y = 0; y < mapData.size(); y++) {
            for (int x = 0; x < mapData[y].size(); x++) {
                renderTile(x, y, mapData[y][x]);
            }
        }
    }
    
private:
    void renderTile(int x, int y, const TileData& tile) {
        Vector3 position = {(float)x, 0.0f, (float)y};
        
        // Render the base of the tile
        m_graphics->drawCube(position, 1.0f, 0.1f, 1.0f, LIGHTGRAY);
        
        // Render resources
        renderResources(position, tile);
        
        // Render players
        renderPlayers(position, tile.players);
        
        // Render eggs
        renderEggs(position, tile.eggs);
    }
    
    void renderResources(const Vector3& basePos, const TileData& tile) {
        float offset = 0.0f;
        
        if (tile.food > 0) {
            Vector3 pos = {basePos.x - 0.3f + offset, 0.2f, basePos.z};
            m_graphics->drawCube(pos, 0.1f, 0.1f, 0.1f, GREEN);
            offset += 0.1f;
        }
        
        if (tile.linemate > 0) {
            Vector3 pos = {basePos.x - 0.3f + offset, 0.2f, basePos.z};
            m_graphics->drawCube(pos, 0.1f, 0.1f, 0.1f, WHITE);
            offset += 0.1f;
        }
        
        // ... autres ressources
    }
};
```

### Camera System

```cpp
class CameraController {
private:
    Camera3D m_camera;
    float m_distance = 20.0f;
    float m_angleH = 0.0f;      // Horizontal angle
    float m_angleV = -30.0f;    // Vertical angle
    Vector3 m_target = {10.0f, 0.0f, 10.0f};
    
public:
    void update() {
        handleInput();
        updateCameraPosition();
    }
    
private:
    void handleInput() {
        // Rotation with the mouse
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mouseDelta = GetMouseDelta();
            m_angleH += mouseDelta.x * 0.5f;
            m_angleV += mouseDelta.y * 0.5f;
            
            // Limit the vertical angle
            m_angleV = Clamp(m_angleV, -89.0f, 89.0f);
        }
        
        // Zoom with the wheel
        float wheel = GetMouseWheelMove();
        m_distance -= wheel * 2.0f;
        m_distance = Clamp(m_distance, 2.0f, 50.0f);
        
        // Movement with WASD
        if (IsKeyDown(KEY_W)) m_target.z -= 0.5f;
        if (IsKeyDown(KEY_S)) m_target.z += 0.5f;
        if (IsKeyDown(KEY_A)) m_target.x -= 0.5f;
        if (IsKeyDown(KEY_D)) m_target.x += 0.5f;
    }
    
    void updateCameraPosition() {
        float radH = m_angleH * DEG2RAD;
        float radV = m_angleV * DEG2RAD;
        
        m_camera.position.x = m_target.x + m_distance * cos(radV) * cos(radH);
        m_camera.position.y = m_target.y + m_distance * sin(radV);
        m_camera.position.z = m_target.z + m_distance * cos(radV) * sin(radH);
        
        m_camera.target = m_target;
        m_camera.up = {0.0f, 1.0f, 0.0f};
    }
};
```

## User Interface

### Information Panels

```cpp
class UIRenderer {
public:
    void render(const GameState& state) {
        renderTeamPanel(state.teams);
        renderPlayerPanel(state.selectedPlayer);
        renderResourcePanel(state.resources);
        renderControlPanel();
    }
    
private:
    void renderTeamPanel(const std::vector<TeamData>& teams) {
        if (m_gui->windowBox({10, 10, 300, 200}, "Teams")) {
            float y = 40;
            for (const auto& team : teams) {
                Color teamColor = getTeamColor(team.name);
                
                m_gui->label({20, y, 100, 20}, team.name.c_str());
                m_gui->label({130, y, 50, 20}, 
                           std::to_string(team.playerCount).c_str());
                m_gui->label({190, y, 50, 20}, 
                           std::to_string(team.maxLevel).c_str());
                
                // Progress bar
                float progress = (float)team.maxLevel / 8.0f;
                m_gui->progressBar({250, y, 40, 15}, "", "", 
                                  progress, 0.0f, 1.0f);
                
                y += 25;
            }
        }
    }
    
    void renderPlayerPanel(const PlayerData* player) {
        if (!player) return;
        
        if (m_gui->windowBox({10, 220, 300, 180}, "Selected Player")) {
            m_gui->label({20, 250, 100, 20}, "Position:");
            m_gui->label({130, 250, 100, 20}, 
                        (std::to_string(player->x) + ", " + 
                         std::to_string(player->y)).c_str());
            
            m_gui->label({20, 275, 100, 20}, "Level:");
            m_gui->label({130, 275, 50, 20}, 
                        std::to_string(player->level).c_str());
            
            m_gui->label({20, 300, 100, 20}, "Orientation:");
            m_gui->label({130, 300, 100, 20}, 
                        getOrientationString(player->orientation).c_str());
            
            // Inventory
            renderInventory(player->inventory, {20, 325});
        }
    }
    
    void renderInventory(const Inventory& inv, const Vector2& pos) {
        const char* resources[] = {"Food", "Linemate", "Deraumere", 
                                  "Sibur", "Mendiane", "Phiras", "Thystame"};
        int counts[] = {inv.food, inv.linemate, inv.deraumere, 
                       inv.sibur, inv.mendiane, inv.phiras, inv.thystame};
        
        for (int i = 0; i < 7; i++) {
            m_gui->label({pos.x, pos.y + i * 15, 80, 15}, resources[i]);
            m_gui->label({pos.x + 90, pos.y + i * 15, 30, 15}, 
                        std::to_string(counts[i]).c_str());
        }
    }
};
```

## Network Communication

### Network Manager

```cpp
class NetworkManager {
private:
    int m_socket;
    std::string m_host;
    int m_port;
    std::queue<std::string> m_messageQueue;
    std::mutex m_queueMutex;
    std::atomic<bool> m_running;
    
public:
    bool connect(const std::string& host, int port) {
        m_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_socket < 0) return false;
        
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port);
        inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr);
        
        if (::connect(m_socket, (struct sockaddr*)&serverAddr, 
                     sizeof(serverAddr)) < 0) {
            close(m_socket);
            return false;
        }
        
        // Send "GRAPHIC" to identify as a graphic client
        send("GRAPHIC\n");
        
        return true;
    }
    
    void networkThreadLoop() {
        char buffer[1024];
        std::string messageBuffer;
        
        while (m_running) {
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(m_socket, &readfds);
            
            struct timeval timeout = {0, 100000}; // 100ms timeout
            int result = select(m_socket + 1, &readfds, nullptr, 
                               nullptr, &timeout);
            
            if (result > 0 && FD_ISSET(m_socket, &readfds)) {
                int bytesRead = recv(m_socket, buffer, sizeof(buffer) - 1, 0);
                if (bytesRead > 0) {
                    buffer[bytesRead] = '\0';
                    messageBuffer += buffer;
                    processMessages(messageBuffer);
                }
            }
        }
    }
    
private:
    void processMessages(std::string& buffer) {
        size_t pos;
        while ((pos = buffer.find('\n')) != std::string::npos) {
            std::string message = buffer.substr(0, pos);
            buffer.erase(0, pos + 1);
            
            std::lock_guard<std::mutex> lock(m_queueMutex);
            m_messageQueue.push(message);
        }
    }
};
```

## Texture and Model Management

### Texture Manager

```cpp
class TextureManager {
private:
    std::unordered_map<std::string, int> m_textures;
    std::shared_ptr<IGraphicsLib> m_graphics;
    
public:
    int loadTexture(const std::string& path) {
        auto it = m_textures.find(path);
        if (it != m_textures.end()) {
            return it->second;
        }
        
        int textureId = m_graphics->loadTexture(path);
        if (textureId >= 0) {
            m_textures[path] = textureId;
        }
        
        return textureId;
    }
    
    void preloadAssets() {
        // Load base textures
        loadTexture("assets/textures/grass.png");
        loadTexture("assets/textures/stone.png");
        loadTexture("assets/textures/food.png");
        
        // Load 3D models
        loadModel("assets/models/player.obj");
        loadModel("assets/models/castle.obj");
    }
};
```

## Performance Optimizations

### Culling and LOD

```cpp
class Renderer {
private:
    bool isInFrustum(const Vector3& position, float radius) {
        // Basic frustum culling calculation
        Vector3 toObject = Vector3Subtract(position, m_camera.position);
        float distance = Vector3Length(toObject);
        
        return distance < m_viewDistance;
    }
    
    int getLODLevel(const Vector3& position) {
        float distance = Vector3Distance(position, m_camera.position);
        
        if (distance < 10.0f) return 0;      // High quality LOD
        if (distance < 25.0f) return 1;      // Medium quality LOD
        return 2;                            // Low quality LOD
    }
    
public:
    void renderScene(const GameState& state) {
        // Sort by distance for alpha blending
        std::vector<RenderObject> objects;
        collectRenderObjects(state, objects);
        
        std::sort(objects.begin(), objects.end(), 
                 [this](const RenderObject& a, const RenderObject& b) {
                     float distA = Vector3Distance(a.position, m_camera.position);
                     float distB = Vector3Distance(b.position, m_camera.position);
                     return distA > distB;
                 });
        
        // Render with culling
        for (const auto& obj : objects) {
            if (isInFrustum(obj.position, obj.radius)) {
                int lod = getLODLevel(obj.position);
                renderObject(obj, lod);
            }
        }
    }
};
```

### Rendering Batching

```cpp
class BatchRenderer {
private:
    struct CubeBatch {
        std::vector<Vector3> positions;
        std::vector<Color> colors;
        std::vector<Vector3> scales;
    };
    
    CubeBatch m_cubeBatch;
    
public:
    void addCube(const Vector3& pos, const Vector3& scale, const Color& color) {
        m_cubeBatch.positions.push_back(pos);
        m_cubeBatch.scales.push_back(scale);
        m_cubeBatch.colors.push_back(color);
    }
    
    void flush() {
        if (m_cubeBatch.positions.empty()) return;
        
        // Batch rendering to reduce draw calls
        m_graphics->drawCubeBatch(m_cubeBatch.positions, 
                                 m_cubeBatch.scales, 
                                 m_cubeBatch.colors);
        
        // Clear the batch
        m_cubeBatch.positions.clear();
        m_cubeBatch.scales.clear();
        m_cubeBatch.colors.clear();
    }
};
```

## Configuration and Settings

### Graphics Settings

```cpp
struct GraphicsSettings {
    int windowWidth = 1920;
    int windowHeight = 1080;
    bool fullscreen = false;
    int targetFPS = 60;
    float viewDistance = 50.0f;
    bool enableShadows = true;
    bool enableAntialiasing = true;
    int msaaSamples = 4;
};

class SettingsManager {
public:
    bool loadSettings(const std::string& configFile) {
        std::ifstream file(configFile);
        if (!file.is_open()) return false;
        
        std::string line;
        while (std::getline(file, line)) {
            parseConfigLine(line);
        }
        
        return true;
    }
    
private:
    void parseConfigLine(const std::string& line) {
        size_t pos = line.find('=');
        if (pos == std::string::npos) return;
        
        std::string key = line.substr(0, pos);
        std::string value = line.substr(pos + 1);
        
        if (key == "window_width") m_settings.windowWidth = std::stoi(value);
        else if (key == "window_height") m_settings.windowHeight = std::stoi(value);
        else if (key == "fullscreen") m_settings.fullscreen = (value == "true");
        // ... autres paramètres
    }
};
```

## Testing and Debugging

### Debug Rendering

```cpp
class DebugRenderer {
public:
    void renderDebugInfo(const GameState& state) {
        if (!m_debugEnabled) return;
        
        // Display FPS
        std::string fpsText = "FPS: " + std::to_string(GetFPS());
        m_gui->label({10, 10, 100, 20}, fpsText.c_str());
        
        // Display camera position
        Vector3 camPos = m_camera.getPosition();
        std::string posText = "Camera: " + 
                             std::to_string(camPos.x) + ", " +
                             std::to_string(camPos.y) + ", " +
                             std::to_string(camPos.z);
        m_gui->label({10, 30, 200, 20}, posText.c_str());
        
        // Debug grid
        m_graphics->drawGrid(20, 1.0f);
        
        // Coordinate axes
        m_graphics->drawLine3D({0, 0, 0}, {5, 0, 0}, RED);   // X
        m_graphics->drawLine3D({0, 0, 0}, {0, 5, 0}, GREEN); // Y
        m_graphics->drawLine3D({0, 0, 0}, {0, 0, 5}, BLUE);  // Z
    }
};
```
