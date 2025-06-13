---
sidebar_position: 3
---

# Guide de Développement

## Configuration de l'Environnement

### Prérequis

**Système :**
- Linux (Ubuntu 20.04+ recommandé)
- GCC 9.0+ ou Clang 10.0+
- Python 3.8+
- Make 4.0+
- Git

**Bibliothèques de développement :**
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake git python3-dev python3-pip
sudo apt install libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxinerama-dev
sudo apt install libxcursor-dev libxi-dev libxext-dev libwayland-dev libxkbcommon-dev

# Pour la documentation
sudo apt install doxygen graphviz
```

**Dépendances Python :**
```bash
cd src/AI
pip3 install -r requirements.txt
```

### Clone et Configuration

```bash
# Clone du dépôt
git clone https://github.com/Epitech/Zappy.git
cd Zappy

# Initialisation des sous-modules
git submodule update --init --recursive

# Compilation complète
make

# Vérification de la compilation
./zappy_server --help
./zappy_gui --help
python3 src/AI/main.py --help
```

## Structure de Développement

### Organisation des Branches

- **main** : Branche principale stable
- **develop** : Branche de développement principal
- **feature/xxx** : Branches de fonctionnalités
- **hotfix/xxx** : Corrections urgentes
- **release/xxx** : Préparation des versions

### Workflow Git

```bash
# Créer une nouvelle fonctionnalité
git checkout develop
git pull origin develop
git checkout -b feature/nouvelle-fonctionnalite

# Développement...
git add .
git commit -m "feat: ajout de la nouvelle fonctionnalité"

# Push et Pull Request
git push origin feature/nouvelle-fonctionnalite
# Créer une PR vers develop
```

## Standards de Code

### Conventions C/C++

**Style :**
- Indentation : 4 espaces
- Nom de fonctions : `snake_case`
- Nom de variables : `snake_case`
- Nom de structures : `snake_case_t`
- Nom de classes : `PascalCase`
- Constantes : `UPPER_CASE`

**Exemple C :**
```c
/**
 * @brief Fonction qui initialise un joueur
 * @param player Pointeur vers la structure joueur
 * @param team_name Nom de l'équipe
 * @return 0 en cas de succès, -1 en cas d'erreur
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

**Exemple C++ :**
```cpp
/**
 * @brief Classe de gestion des textures
 */
class TextureManager {
public:
    /**
     * @brief Charge une texture depuis un fichier
     * @param path Chemin vers le fichier
     * @return ID de la texture ou -1 en cas d'erreur
     */
    int loadTexture(const std::string& path);
    
private:
    std::unordered_map<std::string, int> m_textures;
    std::shared_ptr<IGraphicsLib> m_graphics;
};
```

### Conventions Python

**Style (PEP 8) :**
- Indentation : 4 espaces
- Nom de fonctions : `snake_case`
- Nom de variables : `snake_case`
- Nom de classes : `PascalCase`
- Constantes : `UPPER_CASE`

**Exemple :**
```python
class DecisionManager:
    """Gestionnaire de prise de décision pour l'IA."""
    
    def __init__(self, agent):
        """
        Initialise le gestionnaire de décision.
        
        Args:
            agent: Instance de l'agent IA
        """
        self.agent = agent
        self.behaviors = {}
        
    def decide_behavior(self):
        """
        Détermine le comportement optimal selon l'état actuel.
        
        Returns:
            IBehavior: Comportement sélectionné
        """
        agent_state = self._analyze_agent_state()
        
        if agent_state.food_level < 10:
            return self.behaviors["Dyson"]
        elif agent_state.can_level_up:
            return self.behaviors["Incanter"]
        
        return self.behaviors["Dyson"]
```

### Documentation du Code

**Commentaires obligatoires :**
- Toutes les fonctions publiques
- Structures et classes
- Algorithmes complexes
- Constantes importantes

**Format Doxygen :**
```c
/**
 * @file server.h
 * @brief En-têtes principales du serveur Zappy
 * @author Équipe Zappy
 * @date 2025
 */

/**
 * @struct server_s
 * @brief Structure principale du serveur
 * 
 * Cette structure contient tous les éléments nécessaires
 * au fonctionnement du serveur Zappy.
 */
typedef struct server_s {
    int s_fd;               ///< Socket d'écoute du serveur
    client_t *clients;      ///< Liste des clients connectés
    tile_t **map;          ///< Carte du jeu
} server_t;
```

## Tests et Qualité

### Tests Unitaires

**Structure des tests :**
```
tests/
├── unit/
│   ├── server/
│   │   ├── test_player.c
│   │   ├── test_map.c
│   │   └── test_commands.c
│   ├── gui/
│   │   ├── test_renderer.cpp
│   │   └── test_camera.cpp
│   └── ai/
│       ├── test_agent.py
│       └── test_behaviors.py
├── integration/
│   ├── test_server_ai.c
│   └── test_server_gui.cpp
└── functional/
    ├── test_complete_game.py
    └── test_performance.py
```

**Exemple de test C :**
```c
#include <criterion/criterion.h>
#include "player.h"

Test(player_tests, init_player_success) {
    player_t player;
    int result = init_player(&player, "team1");
    
    cr_assert_eq(result, 0);
    cr_assert_eq(player.level, 1);
    cr_assert_str_eq(player.team_name, "team1");
    
    free_player(&player);
}

Test(player_tests, init_player_null_param) {
    int result = init_player(NULL, "team1");
    cr_assert_eq(result, -1);
}
```

**Exemple de test Python :**
```python
import unittest
from agent.agent import Agent
from agent.decisionManager import DecisionManager

class TestDecisionManager(unittest.TestCase):
    def setUp(self):
        self.agent = Agent("localhost", 4242, "test_team", 0)
        self.decision_manager = DecisionManager(self.agent)
    
    def test_food_priority(self):
        """Test que la collecte de nourriture est prioritaire en cas de faim"""
        self.agent.food = 5  # Faim critique
        behavior = self.decision_manager.decide_behavior()
        self.assertEqual(behavior.__class__.__name__, "DysonBehavior")
    
    def test_incantation_ready(self):
        """Test de la sélection du comportement d'incantation"""
        self.agent.food = 20  # Bien nourri
        self.agent.level = 1
        self.agent.inventory = {"linemate": 1}
        
        behavior = self.decision_manager.decide_behavior()
        self.assertEqual(behavior.__class__.__name__, "IncanterBehavior")
```

### Exécution des Tests

```bash
# Tests unitaires serveur
make tests_run

# Tests fonctionnels
make functional_tests

# Tests Python
cd src/AI
python -m pytest tests/ -v

# Coverage
make coverage
```

### Linting et Formatage

**C/C++ (cpplint) :**
```bash
# Vérification du style
python3 tools/cpplint.py src/Server/*.c src/Server/include/*.h
python3 tools/cpplint.py src/GUI/*.cpp src/GUI/*.hpp

# Configuration dans .cpplint
echo "filter=-whitespace/line_length" > .cpplint
```

**Python (flake8, black) :**
```bash
# Installation des outils
pip install flake8 black mypy

# Formatage automatique
black src/AI/

# Vérification du style
flake8 src/AI/

# Vérification des types
mypy src/AI/
```

## Débogage et Profiling

### Debugging du Serveur

**Compilation debug :**
```bash
make debug
gdb ./zappy_server
```

**Commandes GDB utiles :**
```bash
# Points d'arrêt
(gdb) break main
(gdb) break server.c:125

# Exécution
(gdb) run -p 4242 -x 10 -y 10 -n team1 team2 -c 5 -f 100

# Inspection
(gdb) print server->current_tick
(gdb) print *client
(gdb) backtrace

# Memory debugging avec Valgrind
valgrind --leak-check=full ./zappy_server -p 4242 -x 10 -y 10 -n team1 -c 1 -f 100
```

### Debugging de l'IA

**Logging avancé :**
```python
import logging

# Configuration
logging.basicConfig(
    level=logging.DEBUG,
    format='%(asctime)s [%(levelname)s] %(name)s: %(message)s',
    handlers=[
        logging.FileHandler('debug.log'),
        logging.StreamHandler()
    ]
)

logger = logging.getLogger(__name__)

# Utilisation
logger.debug(f"Agent {self.id} executing action: {action}")
logger.info(f"Behavior changed to: {behavior.__class__.__name__}")
logger.warning(f"Food level low: {self.food}")
logger.error(f"Failed to connect to server: {e}")
```

**Profiling :**
```python
import cProfile
import pstats

# Profiling d'une session
profiler = cProfile.Profile()
profiler.enable()

# Code à profiler
agent.start()

profiler.disable()
stats = pstats.Stats(profiler)
stats.sort_stats('cumulative').print_stats(20)
```

### Performance Monitoring

**Métriques serveur :**
```c
typedef struct perf_metrics_s {
    uint64_t total_commands;
    uint64_t commands_per_second;
    uint64_t connected_clients;
    uint64_t memory_usage;
    double average_latency;
} perf_metrics_t;

void update_metrics(server_t *server) {
    // Collecte des métriques
    server->metrics.connected_clients = count_clients(server);
    server->metrics.memory_usage = get_memory_usage();
    
    // Log périodique
    if (server->current_tick % 1000 == 0) {
        printf("Metrics: %lu clients, %lu cmd/s, %.2fms latency\n",
               server->metrics.connected_clients,
               server->metrics.commands_per_second,
               server->metrics.average_latency);
    }
}
```

## Intégration Continue

### GitHub Actions

```yaml
# .github/workflows/ci.yml
name: CI

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Install dependencies
      run: |
        sudo apt update
        sudo apt install build-essential python3-dev
        
    - name: Build project
      run: make
      
    - name: Run tests
      run: make tests_run
      
    - name: Python tests
      run: |
        cd src/AI
        pip install -r requirements.txt
        python -m pytest tests/
```

### Docker

**Dockerfile :**
```dockerfile
FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    libgl1-mesa-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN make

EXPOSE 4242

CMD ["./zappy_server", "-p", "4242", "-x", "20", "-y", "20", "-n", "team1", "team2", "-c", "5", "-f", "100"]
```

**Docker Compose :**
```yaml
version: '3.8'

services:
  zappy-server:
    build: .
    ports:
      - "4242:4242"
    environment:
      - ZAPPY_TEAMS=team1,team2,team3
      
  zappy-ai-1:
    build: .
    command: python3 src/AI/main.py -p 4242 -n team1 -h zappy-server
    depends_on:
      - zappy-server
      
  zappy-ai-2:
    build: .
    command: python3 src/AI/main.py -p 4242 -n team2 -h zappy-server
    depends_on:
      - zappy-server
```

## Contribution

### Checklist avant PR

- [ ] Code compilé sans warnings
- [ ] Tests unitaires passent
- [ ] Tests fonctionnels passent
- [ ] Documentation mise à jour
- [ ] Code respecte les conventions
- [ ] Pas de fuite mémoire (Valgrind)
- [ ] Performance acceptable

### Review Process

1. **Self-review** : Relecture personnelle du code
2. **Automated checks** : CI/CD validé
3. **Peer review** : Validation par un autre développeur
4. **Integration testing** : Tests sur la branche develop
5. **Merge** : Intégration dans develop puis main

### Ressources

- **Documentation** : `/docs/`
- **Architecture** : `/docs/architecture.md`
- **Protocoles** : `/docs/protocols/`
- **API Reference** : Génération Doxygen dans `/docs/Doxygen/build/`
- **Issues** : GitHub Issues pour bugs et fonctionnalités
- **Discussions** : GitHub Discussions pour questions
