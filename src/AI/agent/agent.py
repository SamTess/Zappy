import sys
import socket
from time import sleep
import utils.input as inputs
import utils.zappy as utils

class ClientAI:
    def __init__(self, ip, port, team, agent_id=0):
        self.ip = ip
        self.port = port
        self.team = team
        self.agent_id = agent_id
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.sock.connect((self.ip, self.port))
        print(f"Connected to {self.ip}:{self.port}")

        welcome_msg = inputs.read_line_blocking(self.sock)
        print(f"Welcome message: {welcome_msg}")
        self.sock.send(f"{self.team}\n".encode('utf-8'))
        print("Joining team:", self.team)
        server_response = inputs.read_line_blocking(self.sock)
        if (server_response == "ko"):
            print("Failed to join team. Please check the team name and try again.")
            sys.exit(1)
        print(f"Player's id: {server_response}")

    def process_server_messages(self):
        while True:
            server_message = inputs.read_line(self.sock)
            if server_message == "":
                break
            print(f"Agent {self.agent_id}: Server message: {server_message}")

    def send_command(self, command):
      self.sock.send(f"{command}\n".encode('utf-8'))
      return inputs.read_line_blocking(self.sock)

    def take_items_on_ground(self, surroundings):
      cleaned = surroundings.strip("[ ]")
      tiles = cleaned.split(", ")
      items_on_ground = tiles[0].strip().split()
      has_item = False

      for item in items_on_ground:
        if item != "player":
          has_item = True
          self.send_command(f"Take {item}")
      return has_item

    def take_next_decision(self, inventory, surroundings):
      # print(f"Agent {self.agent_id}: Surroundings: {surroundings}")
      # print(f"Agent {self.agent_id}: Inventory: {inventory}")

      result = None
      nb_food = utils.parse_inventory(inventory).get("food", 0)
      if nb_food < 10:
        result = self.send_command(utils.go_get_item(surroundings, "food"))
      else:
        self.take_items_on_ground(surroundings)
        result = self.send_command(utils.go_get_item(surroundings, utils.get_best_available_resource(surroundings)))

      if result.startswith("ko"):
          print(f"Agent {self.agent_id}: Command failed: {result}")
      # print(f"Agent {self.agent_id}: Command sent: {result}")

    def run(self):
        while True:
          try:
            self.process_server_messages()
            surroundings = self.send_command("Look")
            inventory = self.send_command("Inventory")
            self.take_next_decision(inventory, surroundings)

          except BrokenPipeError:
              print(f"Agent {self.agent_id}: Connection closed by server.")
              break
          except Exception as e:
              print(f"Agent {self.agent_id}: Error: {e}")
