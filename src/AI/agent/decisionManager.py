from agent.encryptionService import EncryptionService

class DecisionManager:
  def __init__(self):
    self.decision_tree = {}

  def make_decision(self, message):
    if message.startswith("message: "):
      print(f"Received message: {EncryptionService.decrypt_message(message[9:])}")
