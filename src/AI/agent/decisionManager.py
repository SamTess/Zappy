import agent.behaviors as behaviors

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
      "FoodBigDyson": behaviors.FoodBigDysonBehavior(agent),
      "FoodDyson": behaviors.FoodDysonBehavior(agent),
      "JoinTeamMates": behaviors.JoinTeamMatesBehavior(agent),
      "TakeEverythingHere": behaviors.TakeEverythingHereBehavior(agent),
      "TakeAllFoodHere": behaviors.TakeAllFoodHereBehavior(agent),
      "TakeOneFoodHere": behaviors.TakeOneFoodHereBehavior(agent),
      "DropEveryMinerals": behaviors.DropEveryMineralsBehavior(agent),
      "DropAllFood": behaviors.DropAllFoodBehavior(agent),
      "StartCollecting": behaviors.StartCollectingBehavior(agent),
      "FillTeam": behaviors.FillTeamBehavior(agent),
      "Fork": behaviors.ForkBehavior(agent),
      "None": behaviors.NoActionBehavior(agent),
      "": behaviors.NoActionBehavior(agent),
    }


    self.decisions = {
      "start": {"miner": ["FillTeam", "StartCollecting"], "fighter": ["FillTeam", "StartCollecting"]},
      "reproducing": {"miner": ["FoodBigDyson", "Fork", "FillTeam"], "fighter": ["FoodBigDyson"]},
      "collecting": {"miner": ["FillTeam", "BigDyson"], "fighter": ["FoodBigDyson", "FillTeam"]},
      "rallying": {"miner": ["JoinTeamMates", "TakeAllFoodHere"], "fighter": ["JoinTeamMates"]},
      "setting": {"miner": ["DropEveryMinerals"], "fighter": ["DropAllFood"]},
      "upgrading": {"miner": ["Upgrade", "TakeOneFoodHere", "TakeOneFoodHere"], "fighter": ["FoodBigDyson"]}
    }


  def take_action(self):
    try:
      inventory = self.agent.send_command("Inventory")
      surroundings = self.agent.send_command("Look")

      self.agent.last_known_inventory = inventory
      self.agent.last_known_surroundings = surroundings

      if inventory is None or surroundings is None:
        print("Failed to retrieve inventory or surroundings.")
        return

      for action in self.decisions[self.agent.current_phase][self.agent.current_role]:
        self.behaviors[action].execute(surroundings, inventory)

    except Exception as e:
      print(f"DecisionManager: Error in take_action: {e}")
      return
    # print(inventory)
