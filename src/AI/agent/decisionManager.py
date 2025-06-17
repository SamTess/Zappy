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
      "DropEveryMinerals": behaviors.DropEveryMineralsBehavior(agent),
      "None": behaviors.NoActionBehavior(agent),
      "": behaviors.NoActionBehavior(agent),
    }


    self.decisions = {
      "collecting": {"miner": ["BigDyson"], "fighter": ["FoodDyson"]},
      "rallying": {"miner": ["JoinTeamMates", "TakeAllFoodHere"], "fighter": ["FoodDyson"]},
      "setting": {"miner": ["DropEveryMinerals"], "fighter": ["FoodDyson"]},
      "upgrading": {"miner": ["Upgrade"], "fighter": ["FoodDyson"]}
    }


  def take_action(self):
    inventory = self.agent.send_command("Inventory")
    surroundings = self.agent.send_command("Look")

    self.agent.last_known_inventory = inventory
    self.agent.last_known_surroundings = surroundings

    if inventory is None or surroundings is None:
      print("Failed to retrieve inventory or surroundings.")
      return

    for action in self.decisions[self.agent.current_phase][self.agent.current_role]:
      self.behaviors[action].execute(surroundings, inventory)
      print(f"Executed action: {action}")
    # print(inventory)
