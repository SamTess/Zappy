---
sidebar_position: 3
---

# Development Guide

This guide provides specific information for developers contributing to the Zappy project. It covers development standards, workflows, and technical references tailored to this project.

## Environment Setup

### Technical Requirements

**System:**
- Linux (Ubuntu 20.04+ recommended)
- GCC 9.0+ or Clang 10.0+
- Python 3.8+
- Make 4.0+

**Library Dependencies:**
```bash
# Core dependencies
sudo apt install build-essential cmake git python3-dev python3-pip
sudo apt install libgl1-mesa-dev libxrandr-dev libxinerama-dev
sudo apt install libxcursor-dev libxi-dev libxext-dev

# Development tools
sudo apt install valgrind gdb doxygen graphviz
```

**Python Dependencies:**
```bash
cd src/AI
pip3 install -r requirements.txt
```

## Development Workflow

### Git Branching Strategy

- **main**: Stable release branch
- **develop**: Main development branch
- **feature/xxx**: Feature branches
- **hotfix/xxx**: Urgent fixes
- **release/xxx**: Release preparation

### Git Workflow

```bash
# Create new feature
git checkout develop
git pull origin develop
git checkout -b feature/new-feature

# Development...
git add .
git commit -m "feat: add new feature"

# Push and create Pull Request
git push origin feature/new-feature
# Create PR to develop branch
```

## Code Standards

### C/C++ Conventions

**Style:**
- Indentation: 4 spaces
- Function names: `snake_case`
- Variable names: `snake_case`
- Structure names: `snake_case_t`
- Class names: `PascalCase`
- Constants: `UPPER_CASE`

**C Example:**
```c
/**
 * @brief Initialize a player
 * @param player Pointer to player structure
 * @param team_name Team name
 * @return 0 on success, -1 on error
 */
int init_player(player_t *player, const char *team_name)
{
    if (!player || !team_name)
        return -1;
    
    player->x = 0;
    player->y = 0;
    player->level = 1;
    player->team_name = strdup(team_name);
    
    return 0;
}
```

**C++ Example:**
```cpp
/**
 * @brief Texture management class
 */
class TextureManager {
private:
    std::unordered_map<std::string, Texture> m_textures;

public:
    /**
     * @brief Load a texture from file
     * @param filename Path to texture file
     * @return Texture ID or -1 on error
     */
    int loadTexture(const std::string& filename);
};
```

### Python Conventions

**Style:**
- PEP 8 compliance
- Function names: `snake_case`
- Variable names: `snake_case`
- Class names: `PascalCase`
- Constants: `UPPER_CASE`

**Example:**
```python
class DecisionManager:
    """
    Manages AI decision making processes
    """
    def __init__(self, agent):
        self.agent = agent
        self.behaviors = {
            "Dyson": DysonBehavior(),
            "Explorer": ExplorerBehavior()
        }
    
    def select_behavior(self, state):
        """
        Select appropriate behavior based on agent state
        
        Args:
            state: Current agent state
            
        Returns:
            Selected behavior object
        """
        if state.food < 5:
            return self.behaviors["Dyson"]
        return self.behaviors["Explorer"]
```

## Component Integration

### Server-AI Integration

The AI clients communicate with the server using the documented protocol. Key integration points:
- Connection handshake sequence
- Command processing pipeline
- Response parsing
- State tracking

### Server-GUI Integration

The GUI connects as a special observer client. Integration focuses on:
- Initial state synchronization
- Real-time updates
- Event visualization
- Command transmission

### Dynamic Library System

The GUI uses a robust dynamic library loading system:
```cpp
// Load a graphics library
auto& libraryManager = LibraryManager::getInstance();
auto graphicsLib = libraryManager.getInterface<IGraphicsLib>(
    "plugins/libraylibcpp.so", "createGraphicsLib");
```

## Testing Strategy

### Unit Testing

```bash
# Run all unit tests
make unit_tests

# Run specific test suite
make test_server_network
make test_ai_pathfinding
make test_gui_renderer
```

### Integration Testing

```bash
# Run full system integration test
make integration_test

# Run specific scenario
./tests/scenarios/incantation_test.sh
./tests/scenarios/resource_collection_test.sh
```

### Memory Analysis

```bash
# Check for memory leaks
valgrind --leak-check=full ./zappy_server -p 4242 -n team1 -x 20 -y 20
```

## Documentation

### Code Documentation

- C/C++: Doxygen-style comments
- Python: Google-style docstrings
- All public APIs must be documented
- Update documentation when changing interfaces

### Building Documentation

```bash
# Generate code documentation
make docs

# Generate protocol documentation
make protocol_docs

# View documentation
firefox docs/html/index.html
```

---

For more detailed information about specific components, refer to the architecture documentation and technical specifications.
