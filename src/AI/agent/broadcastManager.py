import utils.encryption as encryption
import agent.actions as actions


class BroadcastManager:
  def __init__(self, agent):
    self.agent = agent

  def manage_broadcast(self, full_server_message):
    if not full_server_message or len(full_server_message) < 2:
      print("Empty broadcast message received.")
      return
    if not full_server_message.startswith("message "):
      print(f"Invalid broadcast message format: {full_server_message}")
      return

    try:
      emitter_direction = int(full_server_message.split(" ")[1].split(",")[0])
    except ValueError:
      print(f"Invalid emitter direction in broadcast message: {full_server_message}")
      return

    broadcast_message = full_server_message.split(", ")[1]

    if not broadcast_message:
      print("Empty broadcast message content.")
      return

    print(f"Broadcast received from direction: {emitter_direction}")

    decrypted_message = encryption.decrypt_message(full_server_message.split(", ")[1])

    if decrypted_message is not None:
      print(f"Decrypted broadcast message: {decrypted_message}")
    else:
      print(f"Enemy broadcast message: {broadcast_message}")
      return

    if decrypted_message.startswith("HELP! Upgrade: "):
      try:
        level = int(decrypted_message.split(": ")[1])
        if level == self.agent.level + 1:
          actions.got_to_dir(self.agent, broadcast_emitter_direction)
          print(f"Received help request for level {level}. Moving towards the emitter.")
        else:
          print(f"Received help request for level {level}, but current level is {self.agent.level}. Ignoring.")
      except ValueError:
        print(f"Failed to parse level from message: {decrypted_message}")
