
from abc import ABC, abstractmethod
import utils.zappy as zappy
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
    print("UPGRADING")
    result = self.agent.send_command("Incantation")
    if result is None or "ko" in result:
      print("Upgrade failed: " + str(result))
    else:
      self.agent.level += 1
      print("Upgrade done: " + str(result))

class GetMineralsBehavior(Behavior):
  def execute(self):
    surroundings = self.agent.send_command("Look")
    actions.go_take_item(self.agent, zappy.get_best_available_resource(surroundings))
