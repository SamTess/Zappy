from agent.agentActionsService import AgentActionManager
from constants.upgrades import get_total_upgrade_resources, minimum_players_for_upgrade
from utils.zappy import inventory_to_dict, get_closest_of_item
from random import randint


class AgentStateManager:
  def __init__(self, agent):
      self.agent = agent


  def set_role(self, role):
      self.agent.current_role = role


  def set_phase(self, phase):
      self.agent.current_phase = phase


  def get_role(self):
      return self.agent.current_role


  def get_phase(self):
      return self.agent.current_phase


  def _manage_fork_phase(self):
    if not self.agent.is_original:
      self.agent.current_phase = "collect"
      return
    available_slots = AgentActionManager(self.agent).get_available_slots()
    print(f"Agent {self.agent.id}: fork phase: {available_slots} slots are available.")
    if available_slots > minimum_players_for_upgrade:
      self.agent.current_phase = "fill"
      return


  def _manage_fill_phase(self):
    available_slots = AgentActionManager(self.agent).get_available_slots()
    print(f"Agent {self.agent.id}: fill phase: {available_slots} slots are available.")
    if available_slots <= 0:
      self.agent.current_phase = "collect"
      return


  def _manage_collect_phase(self):
    required_total_amount_of_resources = get_total_upgrade_resources()
    team_total_amount_of_resources = inventory_to_dict(self.agent.last_known_inventory)

    for agent_id, agent_info in self.agent.other_agents.items():
      agent_inventory = inventory_to_dict(agent_info['inventory'])
      for key, value in agent_inventory.items():
        if key in team_total_amount_of_resources:
          team_total_amount_of_resources[key] += value
        else:
          team_total_amount_of_resources[key] = value

    have_enough_resources = True
    for key, required_value in required_total_amount_of_resources.items():
      available_value = team_total_amount_of_resources.get(key, 0)
      if available_value < required_value:
        have_enough_resources = False

    if have_enough_resources:
      print(f"Agent {self.agent.id}: required resources for upgrade are available. Enough players: {len(self.agent.other_agents)}")
      self.agent.current_phase = "rally"


  def _manage_rally_phase(self):
    required_total_amount_of_resources = get_total_upgrade_resources()
    tile_agents_total_amount_of_resources = inventory_to_dict(self.agent.last_known_inventory)
    i = 0
    for agent_id, agent_info in self.agent.other_agents.items():
      if agent_info['direction'] is None or agent_info['direction'] != 0:
        continue
      agent_inventory = inventory_to_dict(agent_info['inventory'])
      for key, value in agent_inventory.items():
          if key in tile_agents_total_amount_of_resources:
              tile_agents_total_amount_of_resources[key] += value
          else:
              tile_agents_total_amount_of_resources[key] = value
      i += 1

    if i < minimum_players_for_upgrade:
      return

    have_enough_resources = True
    for key, required_value in required_total_amount_of_resources.items():
      available_value = tile_agents_total_amount_of_resources.get(key, 0)
      if available_value < required_value:
        have_enough_resources = False

    if have_enough_resources:
      print(f"Agent {self.agent.id}: All agents are ready for setting.")
      self.agent.current_phase = "set"


  def _manage_set_phase(self):
    last_surroundings = self.agent.last_known_surroundings
    if last_surroundings is None or "ko" in last_surroundings:
      print(f"Agent {self.agent.id}: Failed to retrieve surroundings for setting phase.")
      return

    required_total_amount_of_resources = get_total_upgrade_resources()
    for key, value in required_total_amount_of_resources.items():
        distance_to_item, amount_found = get_closest_of_item(last_surroundings, key)
        if distance_to_item == -1 or amount_found < value:
          print(f"Agent {self.agent.id}: Not enough {key} for upgrade. Found: {amount_found}, Required: {value}")
          return
    print(f"Agent {self.agent.id}: All required resources for upgrading are available.")
    self.agent.current_phase = "upgrade"


  def _manage_upgrade_phase(self):
    print(f"Agent {self.agent.id}: Managing upgrade phase...")
    # Implement upgrade phase logic here


  def _update_self_id(self):
    #? On change l'id pour éviter les conflits
    new_id = self.agent.id
    while new_id in self.agent.other_agents:
      #? J'ajoute l'offset de minimum_players_for_upgrade car ça diminue les chances de retomber sur un id déjà utilisé
      new_id = randint(minimum_players_for_upgrade * 2, 10000 + minimum_players_for_upgrade * 2)
    if new_id != self.agent.id:
      print(f"Agent {self.agent.id}: Changing ID from {self.agent.id} to {new_id}.")
      self.agent.broadcastManager.send_broadcast("U", str(new_id))
      self.agent.id = new_id


  def update(self):
    print(f"Agent {self.agent.id}: Updating self state at tick {self.agent.tick}...")
    try:
      self._update_self_id()
      if self.agent.current_phase == "fork":
        self._manage_fork_phase()
      elif self.agent.current_phase == "fill":
        self._manage_fill_phase()
      elif self.agent.current_phase == "collect":
        self._manage_collect_phase()
      elif self.agent.current_phase == "rally":
        self._manage_rally_phase()
      elif self.agent.current_phase == "set":
        self._manage_set_phase()
      elif self.agent.current_phase == "upgrade":
        self._manage_upgrade_phase()
      else:
        print(f"Agent {self.agent.id}: Unknown phase '{self.agent.current_phase}'. No action taken.")
        return
    except Exception as e:
      print(f"Agent {self.agent.id}: Error updating self state: {e}")
