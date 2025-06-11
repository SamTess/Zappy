import utils.encryption as encryption
import agent.behaviors as behaviors
import utils.agentActions as agentActions
import utils.zappy as zappy
from random import choices

class DecisionManager:
  def __init__(self, agent):
    self.agent = agent
    self.behaviors = {
      "GetFood": behaviors.GetFoodBehavior(agent),
      "GetMinerals": behaviors.GetMineralsBehavior(agent),
      "GetFoodAndMinerals": behaviors.GetFoodAndMineralsBehavior(agent),
      "Upgrade": behaviors.UpgradeBehavior(agent),
      "Dyson": behaviors.DysonBehavior(agent)
    }

  def take_action(self):
    inventory = self.agent.send_command("Inventory")
    surroundings = self.agent.send_command("Look")

    if inventory is None or surroundings is None:
      print("Failed to retrieve inventory or surroundings.")
      return

    print(inventory)

    self.behaviors[self.agent.current_behaviour].execute(surroundings, inventory)
    self.behaviors["Upgrade"].execute(surroundings, inventory)
