import socket
import sys
from time import sleep
from agent.socketManager import SocketManager
from agent.decisionManager import DecisionManager
from logger.logger import Logger

class Agent:
  def __init__(self, ip, port, team, agent_id=0):
    try:
      self.ip = ip
      self.port = port
      self.team = team
      self.agent_id = agent_id

      self.decisionManager = DecisionManager(self)
      self.logger = Logger("AI.log", message_prefix=f"(Agent n°{self.agent_id}): ")

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
    welcome_msg = self.get_message(timeout=2)
    team_slots = self.send_command(self.team)
    map_size = self.get_message(timeout=2)

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
    self.logger.info(f"Stopping agent {self.agent_id}...")
    self.socketManager.stop()
    self.sock.close()
    print(f"Agent {self.agent_id} stopped.")


  def run(self):
    while self.socketManager.running:
      try:

        self.decisionManager.process_server_message()
        self.decisionManager.take_next_decision()

        sleep(0.1)

      except Exception as e:
        print(f"Agent {self.agent_id}: Error: {e}")
        self.stop()
        break

  def send_command(self, command):
    return self.socketManager.send_command(command)

  def get_message(self, timeout=None):
    return self.socketManager.get_message(timeout=timeout)

  def has_messages(self):
    return self.socketManager.has_messages()
    # def process_server_messages(self):
    #     while True:
    #         server_message = inputs.read_line(self.sock)
    #         if server_message is None or server_message == "":
    #             break
    #         else:
    #             print("uknown server message received:" + server_message)

    # def take_items_on_ground(self, surroundings):
    #   cleaned = surroundings.strip("[ ]")
    #   tiles = cleaned.split(", ")
    #   items_on_ground = tiles[0].strip().split()
    #   has_item = False

    #   for item in items_on_ground:
    #     if item != "player":
    #       has_item = True
    #       self.send_command(f"Take {item}")
    #   return has_item

    # def take_next_decision(self, inventory, surroundings):
    #   # print(f"Agent {self.agent_id}: Surroundings: {surroundings}")
    #   # print(f"Agent {self.agent_id}: Inventory: {inventory}")

    #   result = None
    #   nb_food = utils.parse_inventory(inventory).get("food", 0)
    #   if nb_food < 10:
    #     result = self.send_command(utils.go_get_item(surroundings, "food"))
    #   elif nb_food >= 15:
    #     result = self.send_command("Broadcast " + encryptionManager._encrypt_message(f"Agent {self.agent_id} is alive and running."))
    #     print(f"Agent {self.agent_id}: Broadcasting alive message.")
    #   else:
    #     self.take_items_on_ground(surroundings)
    #     result = self.send_command(utils.go_get_item(surroundings, utils.get_best_available_resource(surroundings)))

    #   if result and result.startswith("ko"):
    #       print(f"Agent {self.agent_id}: Command failed: {result}")

    # def run(self):
    #     while True:
    #       try:
    #         self.process_server_messages()
    #         surroundings = self.send_command("Look")
    #         inventory = self.send_command("Inventory")
    #         self.take_next_decision(inventory, surroundings)

    #       except BrokenPipeError:
    #           print(f"Agent {self.agent_id}: Connection closed by server.")
    #           break
    #       except Exception as e:
    #           print(f"Agent {self.agent_id}: Error: {e}")
