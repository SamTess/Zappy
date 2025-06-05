
from abc import ABC, abstractmethod
import utils.zappy as zappy
import utils.encryption as encryption
import agent.actions as actions


class Behavior(ABC):
  def __init__(self, agent):
    self.agent = agent
  @abstractmethod
  def execute(self):
    pass

class GetFoodBehavior(Behavior):
  def execute(self):
    actions.go_take_item(self.agent, "food")

class UpgradeBehavior(Behavior):
  def execute(self):
    # inventory = self.agent.send_command("Inventory")
    result = self.agent.send_command("Incantation")
    if result is None or "ko" in result:
      print("Upgrade failed: " + str(result))
    else:
      self.agent.level += 1
      print("Upgrade done: " + str(result))
      res = self.agent.send_command("Broadcast a.")
      print(f"Broadcasting upgrade message: {res}")

class GetMineralsBehavior(Behavior):
  def execute(self):
    surroundings = self.agent.send_command("Look")
    actions.go_take_item(self.agent, zappy.get_best_available_resource(surroundings))

class DysonBehavior(Behavior):
  def execute(self):
    for _ in range(self.agent.map_size_y):
      actions.take_everything_here(self.agent)
      self.agent.send_command("Forward")
    self.agent.send_command("Right")
    self.agent.send_command("Forward")
    self.agent.send_command("Left")
