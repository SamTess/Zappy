---
sidebar_position: 1
---

# AI Client Architecture

## Overview

The Zappy AI client is developed in Python with a modular architecture that enables adaptive behaviors and secure communication between agents. This section provides a detailed technical overview of the AI system architecture.

## Agent Architecture

### Core Structure

The AI system is built around a central `Agent` class that manages all aspects of an AI player's behavior:

```python
class Agent:
    def __init__(self, ip, port, team, agent_id=0):
        self.ip = ip
        self.port = port
        self.level = 1
        self.team = team
        self.id = agent_id
        self.map_size_x = None
        self.map_size_y = None
        self.current_behaviour = "Dyson"
        
        # Core managers
        self.decisionManager = DecisionManager(self)
        self.broadcastManager = BroadcastManager(self)
        self.socketManager = SocketManager(self.sock)
        self.logger = Logger("AI.log", message_prefix=f"(Agent #{self.id}): ")
```

## Behavior System

### Decision Making Engine

The decision-making system uses a sophisticated behavior selection algorithm:

```python
class DecisionManager:
    def __init__(self, agent):
        self.agent = agent
        self.behaviors = {
            "Dyson": DysonBehavior(),       # Efficient resource collection
            "Explorer": ExplorerBehavior(),  # Map exploration
            "Incanter": IncanterBehavior(),  # Ritual performer
            "Breeder": BreederBehavior(),    # Reproduction (fork)
            "Helper": HelperBehavior()       # Assistance to other agents
        }
        self.current_behavior = None
        
    def decide_behavior(self):
        """Selects behavior based on current state"""
        agent_state = self.analyze_agent_state()
        
        # Survival is the highest priority
        if agent_state.food_level < 10:
            return self.behaviors["Dyson"]
        # Level up when conditions are met
        elif agent_state.can_level_up:
            return self.behaviors["Incanter"]
        # Explore when needed
        elif agent_state.exploration_needed:
            return self.behaviors["Explorer"]
        # Default to resource collection
        else:
            return self.behaviors["Dyson"]
    
    def execute_behavior(self):
        """Exécution du comportement sélectionné"""
        behavior = self.decide_behavior()
        
        if behavior != self.current_behavior:
            self.agent.logger.log(f"Switching to behavior: {behavior.__class__.__name__}")
            self.current_behavior = behavior
            
        action = behavior.get_next_action(self.agent)
        if action:
            self.agent.execute_action(action)
```

### Comportement Dyson (Collecte optimisée)

```python
class DysonBehavior(IBehavior):
    def __init__(self):
        self.target_resource = None
        self.path_to_target = []
        self.exploration_pattern = SpiralPattern()
        
    def get_next_action(self, agent):
        # Analyse de l'environnement local
        environment = agent.look()
        
        # Recherche de ressources à proximité
        local_resources = self.analyze_resources(environment)
        
        if local_resources:
            return self.collect_nearest_resource(local_resources)
        
        # Si pas de ressources locales, explorer
        if not self.target_resource:
            self.target_resource = self.find_target_resource(agent)
            
        if self.target_resource:
            return self.move_towards_target(agent)
        else:
            return self.exploration_pattern.get_next_move(agent)
    
    def analyze_resources(self, environment):
        """Analyse les ressources dans l'environnement visible"""
        resources = []
        for i, tile in enumerate(environment):
            for item in tile:
                if item in ['food', 'linemate', 'deraumere', 'sibur', 
                           'mendiane', 'phiras', 'thystame']:
                    resources.append({
                        'type': item,
                        'distance': self.calculate_distance_from_index(i),
                        'priority': self.get_resource_priority(item)
                    })
        
        # Tri par priorité et distance
        return sorted(resources, key=lambda x: (x['priority'], x['distance']))
    
    def get_resource_priority(self, resource):
        """Priorité des ressources selon les besoins actuels"""
        priority_map = {
            'food': 1,      # Priorité maximale pour la survie
            'linemate': 2,  # Nécessaire pour toutes les élévations
            'deraumere': 3,
            'sibur': 3,
            'mendiane': 4,
            'phiras': 4,
            'thystame': 5   # Rare et pour hauts niveaux seulement
        }
        return priority_map.get(resource, 10)
```

### Comportement Incanter (Évolution)

```python
class IncanterBehavior(IBehavior):
    def __init__(self):
        self.elevation_requirements = {
            1: {'players': 1, 'linemate': 1},
            2: {'players': 2, 'linemate': 1, 'deraumere': 1, 'sibur': 1},
            3: {'players': 2, 'linemate': 2, 'sibur': 1, 'phiras': 2},
            4: {'players': 4, 'linemate': 1, 'deraumere': 1, 'sibur': 2, 'phiras': 1},
            5: {'players': 4, 'linemate': 1, 'deraumere': 2, 'sibur': 1, 'mendiane': 3},
            6: {'players': 6, 'linemate': 1, 'deraumere': 2, 'sibur': 3, 'phiras': 1},
            7: {'players': 6, 'linemate': 2, 'deraumere': 2, 'sibur': 2, 
                'mendiane': 2, 'phiras': 2, 'thystame': 1}
        }
        
    def get_next_action(self, agent):
        current_level = agent.level
        requirements = self.elevation_requirements.get(current_level)
        
        if not requirements:
            return None  # Niveau maximum atteint
        
        # Vérification des ressources nécessaires
        inventory = agent.get_inventory()
        missing_resources = self.check_missing_resources(inventory, requirements)
        
        if missing_resources:
            # Collecter les ressources manquantes
            return CollectResourceAction(missing_resources[0])
        
        # Vérification du nombre de joueurs
        players_on_tile = self.count_players_on_tile(agent)
        required_players = requirements['players']
        
        if players_on_tile < required_players:
            # Appeler d'autres joueurs de l'équipe
            return BroadcastAction(f"INCANTATION_CALL:{agent.x},{agent.y}")
        
        # Toutes les conditions remplies, lancer l'incantation
        return IncantationAction()
    
    def check_missing_resources(self, inventory, requirements):
        """Retourne la liste des ressources manquantes"""
        missing = []
        for resource, needed in requirements.items():
            if resource == 'players':
                continue
            current = inventory.get(resource, 0)
            if current < needed:
                missing.extend([resource] * (needed - current))
        return missing
```

## Communication Inter-Agents

### Gestionnaire de broadcast

```python
class BroadcastManager:
    def __init__(self, agent):
        self.agent = agent
        self.message_queue = asyncio.Queue()
        self.team_key = self.generate_team_key(agent.team)
        self.message_types = {
            "RESOURCE": 1,
            "INCANTATION": 2,
            "DANGER": 3,
            "POSITION": 4,
            "REQUEST": 5
        }
    
    def generate_team_key(self, team_name):
        """Create encryption key from team name"""
        return hashlib.sha256(team_name.encode()).digest()[:16]
    
    def encrypt_message(self, message):
        """Encrypt message for team-only communication"""
        cipher = AES.new(self.team_key, AES.MODE_CFB, iv=b'0123456789abcdef')
        return base64.b64encode(cipher.encrypt(message.encode())).decode()
    
    def decrypt_message(self, encrypted_message):
        """Decrypt message from team member"""
        try:
            cipher = AES.new(self.team_key, AES.MODE_CFB, iv=b'0123456789abcdef')
            message = base64.b64decode(encrypted_message)
            return cipher.decrypt(message).decode()
        except:
            return None  # Message from another team or invalid
```

## Pathfinding System

Movement planning uses a sophisticated A* algorithm adapted for the toroidal game world:

```python
class AStarPathfinder:
    def __init__(self, world_map):
        self.world_map = world_map
        
    def find_path(self, start, goal):
        """Find optimal path using A* algorithm"""
        # Initialize open and closed sets
        open_set = PriorityQueue()
        open_set.put((0, start))
        came_from = {start: None}
        cost_so_far = {start: 0}
        
        while not open_set.empty():
            _, current = open_set.get()
            
            if current == goal:
                return self.reconstruct_path(came_from, start, goal)
            
            for neighbor in self.get_neighbors(current):
                # Calculate new cost
                new_cost = cost_so_far[current] + 1
                
                if neighbor not in cost_so_far or new_cost < cost_so_far[neighbor]:
                    # Update path info
                    cost_so_far[neighbor] = new_cost
                    priority = new_cost + self.toroidal_heuristic(neighbor, goal)
                    open_set.put((priority, neighbor))
                    came_from[neighbor] = current
        
        return []  # No path found
    
    def toroidal_heuristic(self, point1, point2):
        """Calculate Manhattan distance on a toroidal map"""
        dx = min(abs(point2.x - point1.x), 
                 self.world_map.width - abs(point2.x - point1.x))
        dy = min(abs(point2.y - point1.y),
                 self.world_map.height - abs(point2.y - point1.y))
        return dx + dy
```

## Level-Specific Strategies

The AI implements different strategies based on the player's level:

| Level | Strategy Focus | Key Resources | Team Coordination |
|-------|---------------|---------------|-------------------|
| 1     | Food collection, basic exploration | Food, Linemate | Minimal |
| 2-3   | Map exploration, linemate collection | Linemate, Deraumere, Sibur | Broadcast positions |
| 4-5   | Group formation, targeted resource gathering | Phiras, Mendiane | Position coordination |
| 6-8   | Complex ritual planning, efficient resource sharing | Thystame | Full coordination |

## State Machine Implementation

```python
class StateMachine:
    def __init__(self):
        self.states = {
            "Explore": ExploreState(),
            "Gather": GatherState(),
            "Incantation": IncantationState(),
            "Survival": SurvivalState(),
            "Reproduction": ReproductionState(),
            "Assist": AssistState()
        }
        self.current_state = "Explore"
        
    async def update(self, agent):
        """Execute current state and check transitions"""
        # Get current state
        state = self.states[self.current_state]
        
        # Execute state action
        result = await state.execute(agent)
        
        # Check for state transitions
        next_state = state.check_transitions(agent, result)
        if next_state != self.current_state:
            await self.transition_to(next_state, agent)
            
        return result
        
    async def transition_to(self, new_state, agent):
        """Handle state transition"""
        old_state = self.states[self.current_state]
        new_state_obj = self.states[new_state]
        
        # Exit current state
        await old_state.exit(agent)
        
        # Update state
        self.current_state = new_state
        
        # Enter new state
        await new_state_obj.enter(agent)
```

## Resource Requirements by Level

The AI manages resources based on the level requirements:

```
# Level 1→2: 1 player, 1 linemate
# Level 2→3: 2 players, 1 linemate, 1 deraumere, 1 sibur
# Level 3→4: 2 players, 2 linemate, 0 deraumere, 1 sibur, 2 phiras
# Level 4→5: 4 players, 1 linemate, 1 deraumere, 2 sibur, 1 phiras
# Level 5→6: 4 players, 1 linemate, 2 deraumere, 1 sibur, 3 phiras
# Level 6→7: 6 players, 1 linemate, 2 deraumere, 3 sibur, 0 phiras, 1 thystame
# Level 7→8: 6 players, 2 linemate, 2 deraumere, 2 sibur, 2 phiras, 2 thystame
```

## Performance Optimization

The AI implementation includes several optimizations:

1. **Command Batching**: Grouping commands to reduce network overhead
2. **Perception Caching**: Storing and updating world state to minimize redundant lookups
3. **Parallel Processing**: Using asyncio for non-blocking operations
4. **Memory Management**: Efficient data structures for world representation
5. **Adaptive Timeout Handling**: Dynamic timeout adjustments based on server response times

---

This documentation provides a comprehensive overview of the AI client architecture. For implementation details, refer to the code in the `src/AI/` directory.
