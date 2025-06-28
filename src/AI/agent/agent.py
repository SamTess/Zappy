from time import sleep, time
from agent.socketManager import SocketManager
from agent.decisionManager import DecisionManager
from agent.broadcastManager import BroadcastManager
from agent.agentStateManager import AgentStateManager
from logger.logger import Logger
from constants.upgrades import get_total_upgrade_resources, minimum_players_for_upgrade
from random import randint
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
      self.tick = 0
      encryption.secret_key = encryption.base_key + self.team

      self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.sock.connect((self.ip, self.port))

      self.logger = Logger("AI.log", message_prefix=f"(Agent n°{self.id}): ")

      self.decisionManager = DecisionManager(self)
      self.broadcastManager = BroadcastManager(self)
      self.stateManager = AgentStateManager(self)
      self.socketManager = SocketManager(self.sock)
      self.socketManager.start()

      self.performance_mode = performance_mode

      self.other_agents = {}                #? {"id": {"direction": "N", "inventory": {}, "last_ping": tick}}
      self.last_enemy_direction = None      #? 0 - 8

      self.current_role = "miner"           #? "fighter", "miner"
      self.current_phase = "fork"           #? "fork", "fill", "collect", "rally", "set", "upgrade", "reproduce"

      self.last_known_inventory = {}
      self.last_known_surroundings = {}


      self.first_message_processing = True
      self.is_original = True

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

    start_time = time()
    self.send_command("Look")
    end_time = time()
    self.server_response_time = end_time - start_time


    if welcome_msg is None or team_slots is None or map_size is None \
    or welcome_msg == "ko" or team_slots == "ko" or map_size == "ko":
      print("Failed to retrieve necessary information from server.")
      sys.exit(1)

    self.id = int(team_slots)
    self.map_size_x = int(map_size.split()[0])
    self.map_size_y = int(map_size.split()[1])

    print(f"Agent {self.id}: Joined team {self.team} successfully.")
    print(self.server_response_time, " seconds response time from server.")
    sleep(self.server_response_time * 10)

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
        if self.is_original:
          self.broadcastManager.send_broadcast("C", "Captain")
          print(f"Agent {self.id}: Broadcasting as captain.")
        self.broadcastManager.send_broadcast("I", f"{self.last_known_inventory}")
        self.process_server_message()
        self.stateManager.update()
        self.decisionManager.take_action()
        sleep(0.1)
        self.tick += 1
        if self.tick > 20:
          self.tick = 0

      except BrokenPipeError:
        print(f"Agent {self.id}: Connection closed by server.")
        break
      except Exception as e:
        print(f"Agent {self.id}: Error: {e}")


  def process_server_message(self):
    try:
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
      self.first_message_processing = False

    except Exception as e:
      print(f"Error processing server message: {e}")


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

    if agent_id in self.other_agents:
      self.other_agents[agent_id]['direction'] = direction
      self.other_agents[agent_id]['inventory'] = inventory
      self.other_agents[agent_id]['last_ping'] = self.tick
      return

    self.other_agents[agent_id] = {
        "direction": direction,
        "inventory": inventory,
        "last_ping": self.tick,
        "is_captain": False
    }


  def update_captain(self, agent_id, direction):
    if agent_id is None or direction is None:
      return

    for agent in self.other_agents:
      if self.other_agents[agent]['is_captain']:
        self.other_agents[agent]['is_captain'] = False

    if agent_id in self.other_agents:
      self.other_agents[agent_id]['is_captain'] = True
      self.other_agents[agent_id]['direction'] = direction
      print(f"Agent {self.id}: Updated captain to agent {agent_id} with direction {direction}.")
    else:
      self.other_agents[agent_id] = {
        "direction": direction,
        "inventory": {},
        "last_ping": self.tick,
        "is_captain": True
      }


  def update_agent_id(self, agent_id, direction, new_id):
    if agent_id is None or new_id is None:
      return

    if agent_id in self.other_agents:
      self.other_agents[new_id] = self.other_agents[agent_id]
      del self.other_agents[agent_id]
      self.other_agents[new_id]['direction'] = direction
      self.other_agents[new_id]['last_ping'] = self.tick
      print(f"Agent {self.id}: Updated agent ID from {agent_id} to {new_id}.")
    else:
      print(f"Agent {self.id}: Agent ID {agent_id} not found for update.")


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
