from abc import ABC, abstractmethod
from time import sleep
from agent.agentActionsService import AgentActionManager
from constants.upgrades import get_total_upgrade_resources, minimum_players_for_upgrade
import utils.zappy as zappy

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

from random import choice
class BigDysonBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    for _ in range(self.agent.map_size_x):
      AgentActionManager(self.agent).take_everything_here()
      self.agent.send_command("Forward")
    self.agent.send_command(choice(["Right", "Left", "Forward"]))
    self.agent.send_command("Forward")
    self.agent.send_command(choice(["Right", "Left", "Forward"]))


class FoodBigDysonBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    for _ in range(self.agent.map_size_x):
      self.agent.send_command("Forward")
      AgentActionManager(self.agent).take_all_of_item_here("food")
    self.agent.send_command(choice(["Right", "Left", "Forward"]))
    self.agent.send_command("Forward")
    self.agent.send_command(choice(["Right", "Left", "Forward"]))


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

    if not self.agent.other_agents:
      print("JoinTeamMatesBehavior: No other agents known.")
      return

    try:
      #? On définit le chef en fonction de celui qui a l'inventaire le plus fourni
      better_inv_id = None

      for agent_id, agent_info in self.agent.other_agents.items():
        inv_dict = zappy.inventory_to_dict(agent_info["inventory"])
        if inv_dict is None:
          print(f"JoinTeamMatesBehavior: Inventory for agent {agent_id} is None.")
          continue
        inv_value = zappy.get_inventory_value(inv_dict)
        if better_inv_id is None or inv_value > zappy.get_inventory_value(zappy.inventory_to_dict(self.agent.other_agents[better_inv_id]["inventory"])):
          better_inv_id = agent_id


      if better_inv_id is None:
        print("JoinTeamMatesBehavior: No suitable agent found with a valid inventory.")
        return

      self.agent.other_agents[better_inv_id]["direction"] = AgentActionManager(self.agent).got_to_dir(self.agent.other_agents[better_inv_id]["direction"])

    except (ValueError, TypeError) as e:
      print(f"JoinTeamMatesBehavior: Error processing agent IDs: {e}")
      return


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


class TakeOneFoodHereBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    self.agent.send_command("Take food")


class DropEveryMineralsBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    if not inventory:
      print("DropEveryMineralsBehavior: Inventory is None.")
      return

    inventory_dict = zappy.inventory_to_dict(inventory)
    minerals = [item for item in inventory_dict if item != "food"]

    for mineral in minerals:
      amount = inventory_dict[mineral]
      for _ in range(amount):
        self.agent.send_command(f"Set {mineral}")


class DropAllFoodBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    if not inventory:
      print("DropAllFoodBehavior: Inventory is None.")
      return

    inventory_dict = zappy.inventory_to_dict(inventory)
    if "food" in inventory_dict:
      amount = inventory_dict["food"]
      for _ in range(amount):
        self.agent.send_command("Set food")


class FillTeamBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    while True:
      slots_available = self.agent.send_command("Connect_nbr")
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
        fork_agent(self.agent.ip, self.agent.port, self.agent.team, self.agent.performance_mode)
      else:
        break


class ForkBehavior(Behavior):
  def execute(self, surroundings=None, inventory=None):
    self.agent.process_server_message()

    self.agent.send_command("Fork")
    print("Fork command sent.")
