from time import sleep
from agent.socketManager import SocketManager
from agent.decisionManager import DecisionManager
from agent.broadcastManager import BroadcastManager
from logger.logger import Logger
import utils.encryption as encryption
import agent.actions as actions
import socket
import sys

class Agent:
  def __init__(self, ip, port, team, agent_id=0):
    try:
      self.ip = ip
      self.port = port

      self.level = 1
      self.team = team
      self.id = agent_id
      self.map_size_x = None
      self.map_size_y = None
      self.current_behaviour = "Dyson"
      encryption.secret_key = encryption.secret_key + self.team

      self.decisionManager = DecisionManager(self)
      self.broadcastManager = BroadcastManager(self)
      self.logger = Logger("AI.log", message_prefix=f"(Agent n°{self.id}): ")

      self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
      self.sock.connect((self.ip, self.port))
      print(f"Connected to {self.ip}:{self.port}")

      self.socketManager = SocketManager(self.sock)
      self.socketManager.start()

    except socket.error as e:
      print(f"Error connecting to server: {e}")
      sys.exit(1)

    except Exception as e:
      print(f"Unexpected error: {e}")
      sys.exit(1)


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

    self.run()


  def stop(self):
    self.logger.info(f"Stopping agent {self.id}...")
    self.socketManager.stop()
    self.sock.close()
    print(f"Agent {self.id} stopped.")


  def run(self):
    while self.socketManager.running:
      try:

        self.process_server_message()
        self.decisionManager.take_action()
        sleep(0.1)

      except BrokenPipeError:
        print(f"Agent {self.id}: Connection closed by server.")
        self.stop()
        break
      except Exception as e:
        print(f"Agent {self.id}: Error: {e}")
        self.stop()
        break

  def send_command(self, command, timeout=2.0):
    return self.socketManager.send_command(command, timeout=timeout)

  def get_message(self, timeout=None):
    return self.socketManager.get_message(timeout=timeout)

  def has_messages(self):
    return self.socketManager.has_messages()

  def process_server_message(self):
    if not self.has_messages():
      return
    message = self.get_message()
    if message.startswith("message "):
      self.broadcastManager.manage_broadcast(message)
    elif message.startswith("dead"):
      print("Agent has died.")
      self.stop()
    elif message.startswith("Current level: "):
      try:
        self.level = int(message.split(": ")[1])
        if self.level >= 2:
          self.current_behaviour = "GetFoodAndMinerals"
        print(f"Current level set to: {self.level}")
      except ValueError:
        print(f"Failed to parse level from message: {message}")
    else:
      print(f"Unknown server message: {message}")
