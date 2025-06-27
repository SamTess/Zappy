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
      "JoinCaptain": behaviors.JoinCaptainBehavior(agent),
      "TakeEverythingHere": behaviors.TakeEverythingHereBehavior(agent),
      "TakeAllFoodHere": behaviors.TakeAllFoodHereBehavior(agent),
      "TakeOneFoodHere": behaviors.TakeOneFoodHereBehavior(agent),
      "DropEveryMinerals": behaviors.DropEveryMineralsBehavior(agent),
      "DropAllFood": behaviors.DropAllFoodBehavior(agent),
      "FillTeam": behaviors.FillTeamBehavior(agent),
      "Fork": behaviors.ForkBehavior(agent),
      "None": behaviors.NoActionBehavior(agent),
      "": behaviors.NoActionBehavior(agent),
    }


    self.decisions = {
      "fork": {"all": ["Fork", "FoodBigDyson"]},
      "fill": {"all": ["FillTeam"]},
      "collect": {"miner": ["BigDyson"], "fighter": ["BigDyson"]},
      "rally": { "all": ["TakeOneFoodHere", "JoinCaptain"]},
      "set": {"all": ["DropEveryMinerals"]},
      "upgrade": {"all": ["Upgrade"]},
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

      if self.agent.current_phase not in self.decisions:
        # print(f"DecisionManager: No decisions defined for phase '{self.agent.current_phase}'.")
        return

      if self.agent.current_role in self.decisions[self.agent.current_phase]:
        for action in self.decisions[self.agent.current_phase][self.agent.current_role]:
          self.behaviors[action].execute(surroundings, inventory)
      if "all" in self.decisions[self.agent.current_phase]:
        for action in self.decisions[self.agent.current_phase]["all"]:
          self.behaviors[action].execute(surroundings, inventory)

    except Exception as e:
      print(f"DecisionManager: Error in take_action: {e}")
      return
    # print(inventory)
