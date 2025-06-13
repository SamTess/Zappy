---
sidebar_position: 1
---

# Documentation de l'Intelligence Artificielle

## Vue d'ensemble

L'IA Zappy est développée en Python avec une architecture modulaire permettant des comportements adaptatifs et une communication sécurisée entre agents.

## Architecture de l'Agent

### Structure principale

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
        
        # Gestionnaires principaux
        self.decisionManager = DecisionManager(self)
        self.broadcastManager = BroadcastManager(self)
        self.socketManager = SocketManager(self.sock)
        self.logger = Logger("AI.log", message_prefix=f"(Agent n°{self.id}): ")
```

## Système de Comportements

### Gestionnaire de décisions

```python
class DecisionManager:
    def __init__(self, agent):
        self.agent = agent
        self.behaviors = {
            "Dyson": DysonBehavior(),       # Collecte efficace de ressources
            "Explorer": ExplorerBehavior(),  # Exploration de la carte
            "Incanter": IncanterBehavior(),  # Réalisation d'incantations
            "Breeder": BreederBehavior(),    # Reproduction (fork)
            "Helper": HelperBehavior()       # Assistance aux autres agents
        }
        self.current_behavior = None
        
    def decide_behavior(self):
        """Choix du comportement basé sur l'état actuel"""
        agent_state = self.analyze_agent_state()
        
        if agent_state.food_level < 10:
            return self.behaviors["Dyson"]  # Priorité à la survie
        elif agent_state.can_level_up:
            return self.behaviors["Incanter"]
        elif agent_state.exploration_needed:
            return self.behaviors["Explorer"]
        else:
            return self.behaviors["Dyson"]  # Comportement par défaut
    
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
        self.message_handlers = {
            'INCANTATION_CALL': self.handle_incantation_call,
            'RESOURCE_FOUND': self.handle_resource_found,
            'DANGER_ALERT': self.handle_danger_alert,
            'COORDINATION': self.handle_coordination
        }
        
    def send_encrypted_message(self, message_type, data):
        """Envoi d'un message chiffré aux autres agents de l'équipe"""
        message = f"{message_type}:{data}"
        encrypted = encryption.encrypt(message, self.agent.team)
        self.agent.send_command(f"Broadcast {encrypted}")
        
    def process_received_message(self, direction, encrypted_message):
        """Traitement d'un message reçu"""
        try:
            decrypted = encryption.decrypt(encrypted_message, self.agent.team)
            message_type, data = decrypted.split(':', 1)
            
            handler = self.message_handlers.get(message_type)
            if handler:
                handler(direction, data)
            else:
                self.agent.logger.log(f"Unknown message type: {message_type}")
                
        except Exception as e:
            # Message d'une autre équipe ou corrompu
            self.agent.logger.log(f"Failed to decrypt message: {e}")
    
    def handle_incantation_call(self, direction, data):
        """Réponse à un appel d'incantation"""
        try:
            x, y = map(int, data.split(','))
            distance = self.calculate_distance(self.agent.x, self.agent.y, x, y)
            
            # Répondre si on est proche et disponible
            if distance < 5 and self.agent.is_available_for_incantation():
                self.agent.set_target_position(x, y)
                self.send_encrypted_message('INCANTATION_RESPONSE', 
                                          f"{self.agent.x},{self.agent.y}")
        except ValueError:
            pass
    
    def handle_resource_found(self, direction, data):
        """Traitement d'une découverte de ressource"""
        try:
            resource_type, x, y = data.split(',')
            if self.agent.needs_resource(resource_type):
                self.agent.add_resource_target(resource_type, int(x), int(y))
        except ValueError:
            pass
```

### Système de chiffrement

```python
# utils/encryption.py
import base64
from cryptography.fernet import Fernet
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.kdf.pbkdf2 import PBKDF2HMAC

secret_key = "default_secret"

def generate_key(team_name):
    """Génération d'une clé de chiffrement basée sur le nom d'équipe"""
    password = (secret_key + team_name).encode()
    salt = b"zappy_salt"  # Salt fixe pour la compatibilité entre agents
    
    kdf = PBKDF2HMAC(
        algorithm=hashes.SHA256(),
        length=32,
        salt=salt,
        iterations=100000,
    )
    key = base64.urlsafe_b64encode(kdf.derive(password))
    return key

def encrypt(message, team_name):
    """Chiffrement d'un message"""
    key = generate_key(team_name)
    f = Fernet(key)
    encrypted = f.encrypt(message.encode())
    return base64.urlsafe_b64encode(encrypted).decode()

def decrypt(encrypted_message, team_name):
    """Déchiffrement d'un message"""
    key = generate_key(team_name)
    f = Fernet(key)
    decoded = base64.urlsafe_b64decode(encrypted_message.encode())
    decrypted = f.decrypt(decoded)
    return decrypted.decode()
```

## Gestion Réseau Asynchrone

### Gestionnaire de socket

```python
class SocketManager:
    def __init__(self, socket):
        self.socket = socket
        self.running = False
        self.thread = None
        self.message_queue = queue.Queue()
        self.response_queue = queue.Queue()
        
    def start(self):
        """Démarrage du thread de gestion réseau"""
        self.running = True
        self.thread = threading.Thread(target=self._network_loop)
        self.thread.daemon = True
        self.thread.start()
        
    def _network_loop(self):
        """Boucle principale de gestion réseau"""
        buffer = ""
        
        while self.running:
            try:
                # Lecture non-bloquante
                self.socket.settimeout(0.1)
                data = self.socket.recv(1024).decode('utf-8')
                
                if not data:
                    break
                    
                buffer += data
                
                # Traitement des messages complets
                while '\n' in buffer:
                    line, buffer = buffer.split('\n', 1)
                    if line.strip():
                        self.response_queue.put(line.strip())
                        
            except socket.timeout:
                continue
            except Exception as e:
                print(f"Network error: {e}")
                break
    
    def send_command(self, command):
        """Envoi d'une commande au serveur"""
        try:
            self.socket.send(f"{command}\n".encode('utf-8'))
        except Exception as e:
            print(f"Send error: {e}")
    
    def get_response(self, timeout=None):
        """Récupération d'une réponse du serveur"""
        try:
            return self.response_queue.get(timeout=timeout)
        except queue.Empty:
            return None
```

## Algorithmes d'Exploration

### Pattern en spirale

```python
class SpiralPattern:
    def __init__(self):
        self.steps = 0
        self.direction = 0  # 0=North, 1=East, 2=South, 3=West
        self.current_segment_length = 1
        self.steps_in_segment = 0
        self.segments_completed = 0
        
    def get_next_move(self, agent):
        """Calcul du prochain mouvement selon le pattern spiral"""
        # Ajuster l'orientation si nécessaire
        if agent.orientation != self.direction:
            if self.should_turn_right(agent.orientation, self.direction):
                return TurnRightAction()
            else:
                return TurnLeftAction()
        
        # Avancer dans la direction actuelle
        if self.steps_in_segment < self.current_segment_length:
            self.steps_in_segment += 1
            return ForwardAction()
        
        # Changer de segment
        self.steps_in_segment = 0
        self.segments_completed += 1
        self.direction = (self.direction + 1) % 4
        
        # Augmenter la longueur du segment tous les 2 segments
        if self.segments_completed % 2 == 0:
            self.current_segment_length += 1
            
        return TurnRightAction()
    
    def should_turn_right(self, current_orientation, target_orientation):
        """Détermine s'il faut tourner à droite pour atteindre l'orientation cible"""
        return (target_orientation - current_orientation) % 4 == 1
```

### Pathfinding A*

```python
class AStarPathfinder:
    def __init__(self, map_width, map_height):
        self.map_width = map_width
        self.map_height = map_height
        self.obstacles = set()
        
    def find_path(self, start, goal):
        """Algorithme A* pour trouver le chemin optimal"""
        open_set = []
        heapq.heappush(open_set, (0, start))
        
        came_from = {}
        g_score = {start: 0}
        f_score = {start: self.heuristic(start, goal)}
        
        while open_set:
            current = heapq.heappop(open_set)[1]
            
            if current == goal:
                return self.reconstruct_path(came_from, current)
            
            for neighbor in self.get_neighbors(current):
                tentative_g_score = g_score[current] + 1
                
                if neighbor not in g_score or tentative_g_score < g_score[neighbor]:
                    came_from[neighbor] = current
                    g_score[neighbor] = tentative_g_score
                    f_score[neighbor] = tentative_g_score + self.heuristic(neighbor, goal)
                    
                    if (f_score[neighbor], neighbor) not in open_set:
                        heapq.heappush(open_set, (f_score[neighbor], neighbor))
        
        return []  # Pas de chemin trouvé
    
    def heuristic(self, a, b):
        """Distance de Manhattan"""
        return abs(a[0] - b[0]) + abs(a[1] - b[1])
    
    def get_neighbors(self, pos):
        """Obtient les voisins valides d'une position"""
        x, y = pos
        neighbors = []
        
        for dx, dy in [(0, 1), (1, 0), (0, -1), (-1, 0)]:
            nx, ny = (x + dx) % self.map_width, (y + dy) % self.map_height
            if (nx, ny) not in self.obstacles:
                neighbors.append((nx, ny))
                
        return neighbors
```

## Système de Logging et Debug

### Logger personnalisé

```python
class Logger:
    def __init__(self, filename, message_prefix=""):
        self.filename = filename
        self.prefix = message_prefix
        self.log_level = LogLevel.INFO
        
    def log(self, message, level=LogLevel.INFO):
        if level >= self.log_level:
            timestamp = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
            formatted_message = f"[{timestamp}] {self.prefix}{message}"
            
            # Écriture dans le fichier
            with open(self.filename, 'a') as f:
                f.write(formatted_message + '\n')
            
            # Affichage console si debug
            if level >= LogLevel.DEBUG:
                print(formatted_message)
    
    def debug(self, message):
        self.log(message, LogLevel.DEBUG)
    
    def info(self, message):
        self.log(message, LogLevel.INFO)
    
    def warning(self, message):
        self.log(message, LogLevel.WARNING)
    
    def error(self, message):
        self.log(message, LogLevel.ERROR)

class LogLevel:
    DEBUG = 0
    INFO = 1
    WARNING = 2
    ERROR = 3
```

## Optimisations et Performance

### Cache des états

```python
class StateCache:
    def __init__(self, max_size=1000):
        self.cache = {}
        self.access_order = []
        self.max_size = max_size
        
    def get_environment_hash(self, environment_data):
        """Génère un hash de l'environnement pour la mise en cache"""
        return hash(str(sorted(environment_data)))
    
    def get_cached_decision(self, environment_hash):
        """Récupère une décision mise en cache"""
        if environment_hash in self.cache:
            # Mise à jour de l'ordre d'accès (LRU)
            self.access_order.remove(environment_hash)
            self.access_order.append(environment_hash)
            return self.cache[environment_hash]
        return None
    
    def cache_decision(self, environment_hash, decision):
        """Met en cache une décision"""
        if len(self.cache) >= self.max_size:
            # Suppression du plus ancien (LRU)
            oldest = self.access_order.pop(0)
            del self.cache[oldest]
        
        self.cache[environment_hash] = decision
        self.access_order.append(environment_hash)
```

## Configuration et Personnalisation

### Paramètres d'agent

```python
class AgentConfig:
    def __init__(self):
        # Comportement
        self.default_behavior = "Dyson"
        self.behavior_switch_threshold = 0.8
        
        # Communication
        self.broadcast_frequency = 10  # Secondes entre les broadcasts
        self.max_message_queue_size = 100
        
        # Exploration
        self.exploration_timeout = 30  # Secondes avant changement de pattern
        self.max_exploration_distance = 20
        
        # Survie
        self.food_panic_threshold = 5
        self.food_warning_threshold = 15
        
        # Performance
        self.action_delay = 0.1  # Délai entre les actions
        self.max_retries = 3
        
    @classmethod
    def load_from_file(cls, config_file):
        """Chargement de la configuration depuis un fichier"""
        config = cls()
        try:
            with open(config_file, 'r') as f:
                data = json.load(f)
                for key, value in data.items():
                    if hasattr(config, key):
                        setattr(config, key, value)
        except FileNotFoundError:
            pass  # Utiliser la configuration par défaut
        return config
```

## Tests et Validation

### Simulateur d'environnement

```python
class EnvironmentSimulator:
    def __init__(self, width, height):
        self.width = width
        self.height = height
        self.map = [[[] for _ in range(width)] for _ in range(height)]
        
    def add_resource(self, x, y, resource_type):
        """Ajoute une ressource à une position"""
        self.map[y][x].append(resource_type)
    
    def simulate_look(self, agent_x, agent_y, orientation):
        """Simule la commande Look d'un agent"""
        visible_tiles = []
        level = 1  # Niveau de vision
        
        for distance in range(level + 1):
            for offset in range(-distance, distance + 1):
                x, y = self.calculate_visible_position(agent_x, agent_y, 
                                                     orientation, distance, offset)
                if 0 <= x < self.width and 0 <= y < self.height:
                    visible_tiles.append(self.map[y][x].copy())
                else:
                    visible_tiles.append([])
        
        return visible_tiles

# Tests unitaires
class TestAgentBehavior(unittest.TestCase):
    def setUp(self):
        self.simulator = EnvironmentSimulator(10, 10)
        self.agent = Agent("localhost", 4242, "test_team", 0)
        
    def test_dyson_behavior_resource_collection(self):
        """Test de collecte de ressources par le comportement Dyson"""
        # Configuration de l'environnement
        self.simulator.add_resource(5, 5, "food")
        
        # Simulation du comportement
        behavior = DysonBehavior()
        environment = self.simulator.simulate_look(5, 4, 0)  # Agent au sud
        
        action = behavior.get_next_action_simulated(environment, self.agent)
        
        # Vérification que l'agent va vers la nourriture
        self.assertIsInstance(action, ForwardAction)
    
    def test_incantation_requirements(self):
        """Test des prérequis d'incantation"""
        behavior = IncanterBehavior()
        
        # Test niveau 1->2
        inventory = {'linemate': 1, 'food': 10}
        missing = behavior.check_missing_resources(inventory, 
                                                  behavior.elevation_requirements[1])
        self.assertEqual(len(missing), 0)
        
        # Test niveau 2->3 avec ressources manquantes
        inventory = {'linemate': 0, 'deraumere': 1, 'sibur': 1, 'food': 10}
        missing = behavior.check_missing_resources(inventory, 
                                                  behavior.elevation_requirements[2])
        self.assertEqual(missing, ['linemate'])
```
