import utils.encryption as encryption
import utils.agentActions as agentActions

####! BROADCAST SYSTEM:
#? message format: "message <emitter_direction>, <encrypted_message>"
#? encrypted_message format: "<message_type>-<sender_id>-<message>
#? message_type: "I" for inventory

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

    parts = full_server_message.split(", ", 1)
    if len(parts) < 2:
        print(f"Invalid broadcast message format (missing comma space separator): {full_server_message}")
        return

    header_part = parts[0]
    broadcast_message = parts[1]

    try:
      sender_agent_direction_str = header_part.split(" ")[1]
      sender_agent_direction = int(sender_agent_direction_str)
    except (IndexError, ValueError):
      print(f"Invalid emitter direction in broadcast message: {full_server_message}")
      return

    if not broadcast_message:
      print("Empty broadcast message content.")
      return

    print(f"Broadcast received from direction: {sender_agent_direction}")

    decrypted_message = encryption.decrypt_message(broadcast_message)

    if decrypted_message is not None:
      print(f"Decrypted broadcast message: {decrypted_message}")

      msg_parts = decrypted_message.split('-', 3)
      if len(msg_parts) != 3:
        print(f"Invalid decrypted message format (expected 3 parts): {decrypted_message}")
        return

      msg_type, sender_agent_id_str, payload = msg_parts

      try:
        sender_agent_id = int(sender_agent_id_str)
      except ValueError:
        print(f"Invalid channel_id or sender_agent_id in decrypted message: {decrypted_message}")
        return

      if msg_type == 'I':
        self._handle_inventory_message(sender_agent_id, sender_agent_direction, payload)
      else:
        print(f"Unknown message type: {msg_type} in decrypted message: {decrypted_message}")
    else:
      print(f"Enemy broadcast message (decryption failed): {broadcast_message}")
      return

  def _handle_inventory_message(self, sender_agent_id, sender_agent_direction, message):
    if not hasattr(self.agent, 'update_agent_info'):
        print("Agent is missing 'update_agent_info' method for handling inventory messages.")
        return
    try:
      self.agent.update_agent_info(sender_agent_id, sender_agent_direction, message)
    except Exception as e:
      print(f"Error updating agent info: {e}")
      return

  def send_broadcast(self, message_type, message):
    if not hasattr(self.agent, 'id'):
        print("Agent is missing 'id' attribute for sending broadcast.")
        return

    agent_id = self.agent.id
    message_content = f"{message_type}-{agent_id}-{message}"
    encrypted_message = encryption.encrypt_message(message_content)

    if encrypted_message:
      # print(f"Agent {agent_id} sending INFO broadcast on channel {channel_id}: {message_content}")
      self.agent.send_message("Broadcast " + encrypted_message)
    else:
      print(f"send_info_broadcast: Failed to encrypt message.")
