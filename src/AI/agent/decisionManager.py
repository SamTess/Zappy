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


  def process_server_message(self):
    if not self.agent.has_messages():
      return

    message = self.agent.get_message()
    if message.startswith("message: "):
      print(f"Received message: {encryption.decrypt_message(message[9:])}")

    elif message.startswith("dead"):
      print("Agent has died.")
      self.agent.stop()

    else:
      print(f"Unknown server message: {message}")


  def take_action(self):
    inventory = self.agent.send_command("Inventory")
    print(inventory)

    self.behaviors["Dyson"].execute()

    if zappy.can_upgrade(inventory, self.agent.level):
      self.behaviors["Upgrade"].execute()
