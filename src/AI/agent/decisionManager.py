import utils.encryption as encryption
import utils.zappy as zappy
from random import choices

class DecisionManager:
  def __init__(self, agent):
    self.decision_tree = {}
    self.agent = agent

  def send_command(self, command):
    return self.agent.socketManager.send_command(command)

  def process_server_message(self, message):
    if message.startswith("message: "):
      print(f"Received message: {encryption.decrypt_message(message[9:])}")

    elif message.startswith("dead"):
      print("Agent has died.")
      self.agent.stop()

    else:
      print(f"Unknown server message: {message}")


  def take_next_decision(self, inventory, surroundings):
    if not inventory or not surroundings:
      print("Inventory or surroundings data is missing.")
      return

    if zappy.get_closest_of_item(surroundings, "food") == 0:
      self.send_command("Take food")
    else:
      commands = ["Forward", "Right", "Left"]
      weights = [3, 1, 1]
      self.send_command(choices(commands, weights=weights, k=1)[0])
