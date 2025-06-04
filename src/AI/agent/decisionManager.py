import utils.encryption as encryption
import utils.zappy as zappy
from random import choices
from abc import ABC, abstractmethod

class Action(ABC):
  @abstractmethod
  def execute(self, agent):
    pass

class GetFoodAction(Action):
  def take_all_food_here(self, agent):
    surroundings = agent.send_command("Look")
    while zappy.get_closest_of_item(surroundings, "food") == 0:
      agent.send_command("Take food")

  def execute(self, agent):
    nb_turns = 0
    surroundings = agent.send_command("Look")

    if zappy.get_closest_of_item(surroundings, "food") == 0:
      self.take_all_food_here(agent)
      return

    while zappy.get_closest_of_item(surroundings, "food") == -1:
      if nb_turns >= 3:
        for _ in range(5):
          agent.send_command("Forward")
      nb_turns += 1
      agent.send_command("Left")
      surroundings = agent.send_command("Look")

    i = 0
    row = 0
    row_length = 1
    distance_to_food = zappy.get_closest_of_item(surroundings, "food")



class DecisionManager:
  def __init__(self, agent):
    self.agent = agent

  def process_server_message(self, message):
    if not self.agent.has_messages():
      return

    message = self.agent.get_message()
    if message.startswith("message: "):
      print(f"Received message: {encryption.decrypt_message(message[9:])}")

    elif message.startswith("dead"):
      print("Agent has died.")
      self.agent.stop()

    else:
      print(f"Unknown server message: {message}")


  def take_next_decision(self):

    surroundings = self.agent.send_command("Look")
    inventory = self.agent.send_command("Inventory")

    if not inventory or not surroundings:
      print("Inventory or surroundings data is missing.")
      return

    if zappy.parse_inventory(inventory).get("food", 0) < 10:
      distance_to_food = zappy.get_closest_of_item(surroundings, "food")


    if zappy.get_closest_of_item(surroundings, "food") == 0:
      self.agent.send_command("Take food")

    else:
      commands = ["Forward", "Right", "Left"]
      weights = [3, 1, 1]
      self.agent.send_command(choices(commands, weights=weights, k=1)[0])
