import utils.encryption as encryption
import agent.behaviors as behaviors
import utils.zappy as zappy
from random import choices
from abc import ABC, abstractmethod

class DecisionManager:
  def __init__(self, agent):
    self.agent = agent
    self.behaviors = {
      "GetFood": behaviors.GetFoodBehavior(agent),
      "GetMinerals": behaviors.GetMineralsBehavior(agent),
      "Upgrade": behaviors.UpgradeBehavior(agent),
      "Dyson": behaviors.DysonBehavior(agent)
    }

  def manage_broadcast(self, full_message):

    if not full_message or len(full_message) < 2:
      print("Empty broadcast message received.")
      return
    splitted_message = full_message.split(", ")
    broadcast_emitter_direction = int(splitted_message[0])
    broadcast_message = splitted_message[1]

    print(f"Broadcast received from direction: {broadcast_emitter_direction}")

    decrypted_broadcast_message = encryption.decrypt_message(full_message.split(", ")[1])
    if decrypted_broadcast_message is not None:
      print(f"Decrypted broadcast message: {decrypted_broadcast_message}")
    else:
      print(f"Enemy broadcast message: {broadcast_message}")
      return

    if decrypted_broadcast_message.startswith("HELP! Upgrade: "):
      try:
        level = int(decrypted_broadcast_message.split(": ")[1])
        if level == self.agent.level + 1:
          print(f"Received help request for current level {self.agent.level}.")
          self.agent.send_command("Incantation")
        else:
          print(f"Received help request for level {level}, but current level is {self.agent.level}. Ignoring.")
      except ValueError:
        print(f"Failed to parse level from message: {decrypted_broadcast_message}")


  def process_server_message(self):
    if not self.agent.has_messages():
      return

    message = self.agent.get_message()
    if message.startswith("message "):
      self.manage_broadcast(message[8:])
    elif message.startswith("dead"):
      print("Agent has died.")
      self.agent.stop()
    elif message.startswith("Current level: "):
      try:
        self.agent.level = int(message.split(": ")[1])
        print(f"Current level set to: {self.agent.level}")
      except ValueError:
        print(f"Failed to parse level from message: {message}")
    else:
      print(f"Unknown server message: {message}")


  def take_action(self):
    inventory = self.agent.send_command("Inventory")
    surroundings = self.agent.send_command("Look")

    if inventory is None or surroundings is None:
      print("Failed to retrieve inventory or surroundings.")
      return

    print(inventory)

    if (self.agent.level < 2):
      self.behaviors["Dyson"].execute()
    else:
      if zappy.parse_inventory(inventory).get("food", 0) < 10:
        self.behaviors["GetFood"].execute(surroundings, inventory)
      else:
        self.behaviors["GetMinerals"].execute(surroundings, inventory)

    self.behaviors["Upgrade"].execute(surroundings, inventory)
