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
      "Dyson": behaviors.DysonBehavior(agent),
      "BigDyson": behaviors.BigDysonBehavior(agent),
      "JoinTeamMates": behaviors.JoinTeamMatesBehavior(agent),
      "FoodDyson": behaviors.FoodDysonBehavior(agent),
      "TakeEverythingHere": behaviors.TakeEverythingHereBehavior(agent),
      "TakeAllFoodHere": behaviors.TakeAllFoodHereBehavior(agent),
      "None": behaviors.NoActionBehavior(agent),
      "": behaviors.NoActionBehavior(agent),
    }


  def upgradePhase(self, surroundings, inventory): # TODO(ms-tristan): implement
    if self.agent.current_role == "miner":
      print("Upgrading with da bros")
    elif self.agent.current_role == "fighter":
      print("Feeding da bros")


  def rallyPhase(self, surroundings, inventory):
    if (self.agent.current_role == "miner"): # TODO(ms-tristan): implement
      self.behaviors["JoinTeamMates"].execute(surroundings, inventory)
      self.behaviors["TakeAllFoodHere"].execute(surroundings, inventory)
    elif (self.agent.current_role == "fighter"):
      self.behaviors["FoodDyson"].execute(surroundings, inventory)


  def collectPhase(self, surroundings, inventory):
    if (self.agent.current_role == "miner"):
      self.behaviors["BigDyson"].execute(surroundings, inventory)
    elif (self.agent.current_role == "fighter"):
      self.behaviors["FoodDyson"].execute(surroundings, inventory)


  def take_action(self):
    inventory = self.agent.send_command("Inventory")
    surroundings = self.agent.send_command("Look")

    self.agent.last_known_inventory = inventory
    self.agent.last_known_surroundings = surroundings

    if inventory is None or surroundings is None:
      print("Failed to retrieve inventory or surroundings.")
      return

    if self.agent.current_phase == "collecting":
      self.collectPhase(surroundings, inventory)
    elif self.agent.current_phase == "rallying":
      self.rallyPhase(surroundings, inventory)
    elif self.agent.current_phase == "upgrading":
      self.upgradePhase(surroundings, inventory)

    # print(inventory)
