from abc import ABC, abstractmethod
from time import sleep
import utils.zappy as zappy
import utils.encryption as encryption
import agent.actions as actions
import constants.upgrades as upgrades

class Behavior(ABC):
  def __init__(self, agent):
    self.agent = agent
  @abstractmethod
  def execute(self, surroundings=None, inventory=None):
    pass


class GetFoodBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    actions.go_take_item(self.agent, "food")


class UpgradeBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):

    if not surroundings or not inventory:
      print("UpgradeBehavior: Surroundings or inventory is None.")
      return

    inventory_dict = zappy.parse_inventory(inventory)
    upgrade_info = upgrades.upgrades.get(self.agent.level, {})

    if not upgrade_info:
      print(f"UpgradeBehavior: No upgrade defined for level {self.agent.level}.")
      return
    if not inventory_dict:
      print("UpgradeBehavior: Inventory is empty or not properly parsed.")
      return

    upgrade_cost = upgrade_info.get("cost", {})

    for resource, amount in upgrade_cost.items():
      if resource == "players":
        if zappy.how_much_of_item_here(surroundings, "player") < amount:
          self.agent.send_command("Broadcast " + encryption.encrypt_message("HELP! Upgrade: 2"))
          return
        continue
      elif inventory_dict.get(resource, 0) < amount:
        return

    self.agent.send_command("Incantation")
      # res = self.agent.send_command("Broadcast a.")
      # print(f"Broadcasting upgrade message: {res}")


class GetMineralsBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    surroundings = self.agent.send_command("Look")
    actions.go_take_item(self.agent, zappy.get_best_available_resource(surroundings))


class DysonBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    for _ in range(self.agent.map_size_y):
      actions.take_everything_here(self.agent)
      self.agent.send_command("Forward")
    self.agent.send_command("Right")
    self.agent.send_command("Forward")
    self.agent.send_command("Left")

class GetFoodAndMineralsBehaviour(Behavior):
  def execute(self, surroundings=None, inventory=None):
    if not surroundings or not inventory:
      print("GetFoodAndMineralsBehaviour: Surroundings or inventory is None.")
      return

    if zappy.parse_inventory(inventory).get("food", 0) < 10:
      GetFoodBehavior(self.agent).execute(surroundings, inventory)
    else:
      GetMineralsBehavior(self.agent).execute(surroundings, inventory)
