from abc import ABC, abstractmethod
from time import sleep
import utils.zappy as zappy
import utils.encryption as encryption
from agent.agentActionsService import AgentActionManager
import constants.upgrades as upgrades

class Behavior(ABC):
  def __init__(self, agent):
    self.agent = agent
  @abstractmethod
  def execute(self, surroundings=None, inventory=None):
    pass


class NoActionBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    sleep(0.1)
    return


class GetFoodBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    AgentActionManager(self.agent).go_take_item("food")


class UpgradeBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    if not surroundings or not inventory:
      print("UpgradeBehavior: Surroundings or inventory is None.")
      return

    inventory_dict = zappy.inventory_to_dict(inventory)
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
          return
        continue
      elif inventory_dict.get(resource, 0) < amount:
        return

    self.agent.send_command("Incantation")


class GetMineralsBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    if not surroundings:
      print("GetMineralsBehavior: Surroundings is None.")
      return
    AgentActionManager(self.agent).go_take_item(zappy.get_best_available_resource(surroundings))


class DysonBehavior(Behavior):
  def __init__(self, agent):
    super().__init__(agent)
    self.current_index = 0

  def execute(self, surroundings=None, inventory=None):
    self.current_index += 1
    self.agent.send_command("Forward")
    AgentActionManager(self.agent).take_everything_here()

    max_index = 10
    if self.agent.map_size_x is not None:
      max_index = self.agent.map_size_x

    if self.current_index >= max_index:
      self.agent.send_command("Right")
      self.agent.send_command("Forward")
      self.agent.send_command("Left")
      self.current_index = 0


class FoodDysonBehavior(Behavior):
  def __init__(self, agent):
    super().__init__(agent)
    self.current_index = 0

  def execute(self, surroundings=None, inventory=None):
    self.current_index += 1
    self.agent.send_command("Forward")
    AgentActionManager(self.agent).take_all_of_item_here("food")

    max_index = 10
    if self.agent.map_size_x is not None:
      max_index = self.agent.map_size_x

    if self.current_index >= max_index:
      self.agent.send_command("Right")
      self.agent.send_command("Forward")
      self.agent.send_command("Left")
      self.current_index = 0

class BigDysonBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    for _ in range(self.agent.map_size_x):
      AgentActionManager(self.agent).take_everything_here()
      self.agent.send_command("Forward")
    self.agent.send_command("Right")
    self.agent.send_command("Forward")
    self.agent.send_command("Left")

class GetFoodAndMineralsBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    if not surroundings or not inventory:
      print("GetFoodAndMineralsBehaviour: Surroundings or inventory is None.")
      return

    if zappy.inventory_to_dict(inventory).get("food", 0) < 10:
      GetFoodBehavior(self.agent).execute(surroundings, inventory)
    else:
      GetMineralsBehavior(self.agent).execute(surroundings, inventory)


class JoinTeamMatesBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    if not surroundings:
      print("JoinTeamMatesBehavior: Surroundings is None.")
      return

    if not self.agent.other_agents:
      print("JoinTeamMatesBehavior: No other agents known.")
      return

    lowest_id = min([int(agent_id) for agent_id in self.agent.other_agents.keys()])
    if self.agent.id <= lowest_id:
      return

    self.agent.other_agents[lowest_id]["direction"] = AgentActionManager(self.agent).got_to_dir(self.agent.other_agents[lowest_id]["direction"])


class TakeEverythingHereBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    if not surroundings:
      print("TakeEverythingHereBehavior: Surroundings is None.")
      return

    AgentActionManager(self.agent).take_everything_here()


class TakeAllFoodHereBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    if not surroundings:
      print("TakeAllFoodHereBehavior: Surroundings is None.")
      return

    AgentActionManager(self.agent).take_all_of_item_here("food")
