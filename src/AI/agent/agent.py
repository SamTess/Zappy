from time import sleep
from agent.socketManager import SocketManager
from agent.decisionManager import DecisionManager
from agent.broadcastManager import BroadcastManager
from logger.logger import Logger
from constants.upgrades import get_total_upgrade_resources
import utils.encryption as encryption
import utils.zappy as zappy
import socket
import sys

class Agent:
  def __init__(self, ip, port, team, agent_id=0, performance_mode=False):
    try:
      self.ip = ip
      self.port = port

      self.level = 1
      self.team = team
      self.id = agent_id
      self.map_size_x = None
      self.map_size_y = None
      self.current_behaviour = "BigDyson"
      encryption.secret_key = encryption.secret_key + self.team

      self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.sock.connect((self.ip, self.port))

      self.logger = Logger("AI.log", message_prefix=f"(Agent n°{self.id}): ")

      self.decisionManager = DecisionManager(self)
      self.broadcastManager = BroadcastManager(self)
      self.socketManager = SocketManager(self.sock)
      self.socketManager.start()

      self.performance_mode = performance_mode

      # TODO(ms-tristan): garder une info sur tous les autres agents + sur la dernière direction ennemie connue
      self.other_agents = {}                #? {"id": {"direction": "N", "inventory": {}}}
      self.last_enemy_direction = None      #? 0 - 8

      # TODO(ms-tristan): garder des infos sur l'état actuel de l'agent -(rôle et phase)
      self.current_role = "miner"           #? "fighter", "miner"
      self.current_phase = "collecting"     #? "collecting", "rallying", "upgrading", "reproducing"

      # TODO(ms-tristan): garder en mémoire les dernières infos connues sur soi
      self.last_known_inventory = {}
      self.last_known_surroundings = {}

    except socket.error as e:
      print(f"Error connecting to server: {e}")
      sys.exit(1)
    except Exception as e:
      print(f"Unexpected error: {e}")
      sys.exit(1)

    finally:
      print(f"Connected to {self.ip}:{self.port} as team '{self.team}' with agent ID {self.id}.")


  def start(self):
    welcome_msg = self.get_message(timeout=4)
    team_slots = self.send_command(self.team)
    map_size = self.get_message(timeout=4)

    self.map_size_x = int(map_size.split()[0])
    self.map_size_y = int(map_size.split()[1])

    if welcome_msg == "WELCOME":
      print(f"Welcome message {welcome_msg}")
    else:
      print(f"Unexpected welcome message: {welcome_msg}")
      sys.exit(1)

    if (team_slots == "ko"):
        print("Failed to join team. Either the team is full, or its name is incorrect.")
        sys.exit(1)
    else:
      print(f"Joined team {self.team} successfully, {team_slots} slots left in the team.")

    print(f"Map size: {map_size}")
    self._run()
    self._stop()

  def _stop(self):
    self.logger.info(f"Stopping agent {self.id}...")
    self.socketManager.stop()
    self.sock.close()
    print(f"Agent {self.id} stopped.")


  def _run(self):
    while self.socketManager.running:
      try:
        self.broadcastManager.send_broadcast("I", f"{self.last_known_inventory}")  #? Envoyer ses infos aux autres
        self._process_server_message()
        self._update_self_state()           # TODO(ms-tristan): update l'état de l'agent actuel en fonction des informations reçues
        self.decisionManager.take_action()  # TODO(ms-tristan): recréer l'arbre de décision pour prendre en compte les nouvelles actions
        sleep(0.1)

      except BrokenPipeError:
        print(f"Agent {self.id}: Connection closed by server.")
      except Exception as e:
        print(f"Agent {self.id}: Error: {e}")


  def _process_server_message(self):
    while self.has_messages():
      message = self.get_message()
      if message.startswith("message "):
        self.broadcastManager.manage_broadcast(message)
      elif message.startswith("dead"):
        print("Agent has died.")
        self.stop()
      elif message.startswith("Current level: "):
        try:
          self.level = int(message.split(": ")[1])
          print(f"Current level set to: {self.level}")
        except ValueError:
          print(f"Failed to parse level from message: {message}")
      else:
        print(f"Unknown server message: {message}")


  def _update_self_state(self):

    #? On détermine le rôle de l'agent en fonction de son id
    agent_ids = list(self.other_agents.keys())
    agent_ids.append(self.id)
    agent_ids.sort()
    if agent_ids.index(self.id) < 6:
      self.current_role = "miner"
    else:
      self.current_role = "fighter"

    #? On check si l'inventaire de tout le monde permet d'upgrade de 0 à 8
    if self.current_phase == "collecting":
      required_total_amount_of_resources = get_total_upgrade_resources()
      team_total_amount_of_resources = zappy.inventory_to_dict(self.last_known_inventory)

      for agent_id, agent_info in self.other_agents.items():
        agent_inventory = zappy.inventory_to_dict(agent_info['inventory'])
        for key, value in agent_inventory.items():
            if key in team_total_amount_of_resources:
                team_total_amount_of_resources[key] += value
            else:
                team_total_amount_of_resources[key] = value

      have_enough_resources = True
      for key, required_value in required_total_amount_of_resources.items():
        available_value = team_total_amount_of_resources.get(key, 0)
        if available_value < required_value:
          have_enough_resources = False

      if have_enough_resources:
        print("All required resources for upgrade are available.")
        self.current_phase = "rallying"
        self.current_behaviour = "Dyson"
      else:
        print("Not all required resources for upgrade are available.")


  def send_command(self, command, timeout=2.0):
    if (self.performance_mode):
      command = " " + command
    return self.socketManager.send_command(command, timeout=timeout)

  def get_message(self, timeout=None):
    return self.socketManager.get_message(timeout=timeout)

  def has_messages(self):
    return self.socketManager.has_messages()

  def update_agent_info(self, agent_id, direction, inventory):
    self.other_agents[agent_id] = {
        "direction": direction,
        "inventory": inventory
    }

  def update_last_known_enemy_direction(self, direction):
    self.last_enemy_direction = direction
