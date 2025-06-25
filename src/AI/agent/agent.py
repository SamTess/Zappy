from time import sleep
from agent.socketManager import SocketManager
from agent.decisionManager import DecisionManager
from agent.broadcastManager import BroadcastManager
from logger.logger import Logger
from constants.upgrades import get_total_upgrade_resources, minimum_players_for_upgrade
import utils.encryption as encryption
import utils.zappy as zappy
import socket
import sys


class Agent:
  def __init__(self, ip, port, team, performance_mode=False):
    try:
      self.ip = ip
      self.port = port

      self.level = 1
      self.team = team
      self.id = 0
      self.map_size_x = None
      self.map_size_y = None
      self.current_behaviour = "BigDyson"
      self.tick = 0
      encryption.secret_key = encryption.base_key + self.team

      self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.sock.connect((self.ip, self.port))

      self.logger = Logger("AI.log", message_prefix=f"(Agent n°{self.id}): ")

      self.decisionManager = DecisionManager(self)
      self.broadcastManager = BroadcastManager(self)
      self.socketManager = SocketManager(self.sock)
      self.socketManager.start()

      self.performance_mode = performance_mode

      self.other_agents = {}                #? {"id": {"direction": "N", "inventory": {}}}
      self.last_enemy_direction = None      #? 0 - 8

      self.current_role = "miner"           #? "fighter", "miner"
      self.current_phase = "collecting"

      self.last_known_inventory = {}
      self.last_known_surroundings = {}

      self.running = False

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

    if welcome_msg is None or team_slots is None or map_size is None \
    or welcome_msg == "ko" or team_slots == "ko" or map_size == "ko":
      print("Failed to retrieve necessary information from server.")
      sys.exit(1)

    self.id = int(team_slots)
    self.map_size_x = int(map_size.split()[0])
    self.map_size_y = int(map_size.split()[1])

    print(f"Welcome message {welcome_msg}")
    print(f"Joined team {self.team} successfully, {team_slots} slots left in the team.")
    print(f"Map size: {map_size}")

    self.running = True
    self._run()
    self.stop()


  def stop(self):
    if hasattr(self, '_stopping') and self._stopping:
      return

    self._stopping = True

    if self.running:
      self.logger.info(f"Stopping agent {self.id}...")
      self.running = False

      try:
        if hasattr(self, 'socketManager') and self.socketManager:
          self.socketManager.stop()
      except Exception as e:
        print(f"Error stopping socket manager: {e}")

      try:
        if hasattr(self, 'sock') and self.sock:
          self.sock.close()
      except Exception as e:
        print(f"Error closing socket: {e}")

      print(f"Agent {self.id} stopped.")


  def _run(self):
    while self.socketManager.running and self.running:
      try:
        self.broadcastManager.send_broadcast("I", f"{self.last_known_inventory}")  #? Envoyer ses infos aux autres
        self.process_server_message()
        self.decisionManager.take_action()
        # on laisse l'update en dessous de la prise de décision
        # pour que les agents ne rebougent pas après avoir reçu un broadcast sur la même case
        self._update_self_state()
        sleep(0.1)
        self.tick += 1
        if self.tick > 20:
          self.tick = 0
      except BrokenPipeError:
        print(f"Agent {self.id}: Connection closed by server.")
      except Exception as e:
        print(f"Agent {self.id}: Error: {e}")


  def process_server_message(self):
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
    if self.id >= max(agent_ids) - minimum_players_for_upgrade - 2:
      self.current_role = "miner"
    else:
      self.current_role = "fighter"

    # #? L'agent "originel" regarde si il y a assez de monde pour upgrade, sinon il fork et appelle les autres à fork aussi
    # #? L'agent originel est celui qui a l'id le plus élevé
    # if len(self.other_agents) < minimum_players_for_upgrade * 2:
    #   if self.id >= max(agent_ids):
    #     #? plus de place dans l'équipe, on fork
    #     if self.id == 0:
    #       self.fork()
    #       self.fill_team()
    #     #? sinon on demande aux autres de fork
    #     else:
    #       self.fill_team()

    #? On retire les agents qui n'ont pas envoyé de message depuis trop longtemps
    agents_to_remove = []
    for agent_id, items in self.other_agents.items():
      if items['last_ping'] > self.tick:
        agents_to_remove.append(agent_id)

    for agent_id in agents_to_remove:
      print(f"Removing agent {agent_id} from other_agents due to inactivity.")
      del self.other_agents[agent_id]

    if  self.current_phase == "collecting":
      #? On check si l'inventaire de tout le monde permet d'upgrade de 0 à 8
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
        print(f"Enough agents for upgrade. Current count: {len(self.other_agents)}")
        self.current_phase = "rallying"
      else:
        print("Not all required resources for upgrade are available.")

    elif self.current_phase == "reproducing":
      self.current_phase = "rallying"

    #? On check si assez de monde est rassemblé pour passer au setting
    elif self.current_phase == "rallying":
      if len(self.other_agents) < minimum_players_for_upgrade * 2:
        if self.current_role == "miner":
          self.current_phase = "reproducing"
          return

      i = 0
      for agent_info in self.other_agents.items():
        if agent_info[1]['direction'] is None or agent_info[1]['direction'] != 0:
          print(f"Agent {agent_info[0]} direction: {agent_info[1]['direction']}")
          print("Waiting for all agents to be ready for setting.")
          return
        if i >= minimum_players_for_upgrade:
          break
      print("All agents are ready for setting.")
      self.current_phase = "setting"

    #? On check si assez de ressources ont été lachées pour passer à l'upgrade
    elif self.current_phase == "setting":
      last_surroundings = self.last_known_surroundings
      if last_surroundings is None or "ko" in last_surroundings:
        print("Failed to retrieve surroundings for setting phase.")
        return

      required_total_amount_of_resources = get_total_upgrade_resources()
      for key, value in required_total_amount_of_resources.items():
          distance_to_item, amount_found = zappy.get_closest_of_item(last_surroundings, key)
          if distance_to_item == -1 or amount_found < value:
              print(f"Not enough {key} for upgrade. Found: {amount_found}, Required: {value}")
              return
      distance_to_players,  amount_of_players = zappy.get_closest_of_item(last_surroundings, "player")
      #? Si des agents sont morts entre temps, on retourne collecter (et fork aussi)
      if distance_to_players == -1 or amount_of_players < minimum_players_for_upgrade:
        self.current_phase = "collecting"
      else:
        print("All required resources for upgrading are available.")
        self.current_phase = "upgrading"


  def send_command(self, command, timeout=2.0):
    if (self.performance_mode):
      command = " " + command
    return self.socketManager.send_command(command, timeout=timeout)


  def get_message(self, timeout=None):
    return self.socketManager.get_message(timeout=timeout)


  def has_messages(self):
    return self.socketManager.has_messages()


  def update_agent_info(self, agent_id, direction, inventory):
    if direction is None or inventory is None:
      if agent_id is None:
        return
      if agent_id in self.other_agents:
        del self.other_agents[agent_id]
      return

    self.other_agents[agent_id] = {
        "direction": direction,
        "inventory": inventory,
        "last_ping": self.tick
    }


  def fork(self):
    fork_res = self.send_command("Fork")
    if fork_res is not None and not "ko" in fork_res:
      self.fill_team()


  def fill_team(self):
    while True:
      slots_available = self.send_command("Connect_nbr")
      if slots_available is None or "ko" in slots_available:
        print(f"ForkAgentBehavior: Failed to get available slots. Response: {slots_available}")
        break
      try:
        slots_available = int(slots_available)
      except ValueError:
        print(f"ForkAgentBehavior: Invalid slots_available value: {slots_available}")
        return
      if slots_available > 0:
        from utils.mutliprocessing import fork_agent
        fork_agent(self.ip, self.port, self.team, self.performance_mode)
      else:
          break


  def update_last_known_enemy_direction(self, direction):
    if direction is not None:
      self.last_enemy_direction = direction
      if direction == 0:
        self.send_command("Eject")

