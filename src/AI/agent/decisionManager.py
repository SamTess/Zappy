import utils.encryption as encryption
import agent.behaviors as behaviors
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


  def upgradePhase(self): # TODO(ms-tristan): implement
    if self.agent.role == "miner":
      print("Upgrading with da bros")
    elif self.agent.role == "fighter":
      print("Feeding da bros")


  def rallyPhase(self):
    if (self.agent.role == "miner"): # TODO(ms-tristan): implement
      print("Going to the agent with the lowest id")
    elif (self.agent.role == "fighter"):
      print("Going onto the ennemies")


  def collectPhase(self):
    if (self.agent.role == "miner"): # TODO(ms-tristan): implement
      print("Collecting everything")
    elif (self.agent.role == "fighter"):
      print("Collecting food only")


  def take_action(self):
    self.agent.last_known_inventory = self.agent.send_command("Inventory")
    self.agent.last_known_surroundings = self.agent.send_command("Look")

    inventory = self.agent.last_known_inventory
    surroundings = self.agent.last_known_surroundings

    if inventory is None or surroundings is None:
      print("Failed to retrieve inventory or surroundings.")
      return

    print(inventory)

    self.behaviors[self.agent.current_behaviour].execute(surroundings, inventory)
    self.behaviors["Upgrade"].execute(surroundings, inventory)
