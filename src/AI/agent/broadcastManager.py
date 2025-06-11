import utils.encryption as encryption
import utils.agentActions as agentActions

####! BROADCAST SYSTEM:
##? Broadcast messages contain a header added by the server:
##  - The header takes the form: "message <direction>, message_content"
##  - "message " : indicates to the client that it is a broadcast message
##  - <direction> : indicates the direction of the message sender
##  - message_content : the content of the broadcast message
##* {the broadcast message is encrypted and decrypted by the team using the helpers in utils.encryption}

##? Broadcast message encryption:
##  - Broadcast contents are encrypted with a secret key specific to each team
##  - A message that fails to decrypt is considered an enemy message and therefore ignored

##? Broadcast content:
## Exemple: I-42-1-hello
##  - I : Indicates the type of message: I=Information, O=Open, C=Close
##  - 42 : Indicates the ID of the relevant channel
##  - 1 : Indicates the ID of the sending agent
##  - hello : The message to be transmitted
##* {theses 4 parts are always included (and dash-separated)}

##? Broadcast channel management:
##* Opening a channel:
##  - The sender creates a unique ID based on the known channels (a list containing the IDs and topics of open channels)
##  - It sends O-channel_id-agent_id-channel_topic
##* Upon receiving a channel opening, the agent:
##  - Checks if the channel is already open (if yes, ignores the message > allows redistributing the info to new agents)
##  - Otherwise, adds the channel to its list of known channels
##  - If the channel topic is of interest, subscribes to the channel
##* Upon receiving a channel closure, the agent:
##  - Unsubscribes and forgets the channel

##? Channel management in the gameloop:
##* Subscription
##  - If an agent’s situation evolves over time, it may decide to subscribe to a channel
##* Tracking the channel topic:
##  - If an agent subscribes to a channel, it must track the topic based on a list of predefined behaviors
##  - If the topic of the channel is resolved, the channel is closed and the agent unsubscribes

class BroadcastManager:
  def __init__(self, agent):
    self.agent = agent
    self.known_channels = {}  # channel_id: {'topic': str, 'original_sender_id': int}
    self.subscribed_channels = set()  # Set of channel_ids


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
      emitter_direction_str = header_part.split(" ")[1]
      emitter_direction = int(emitter_direction_str)
    except (IndexError, ValueError):
      print(f"Invalid emitter direction in broadcast message: {full_server_message}")
      return

    if not broadcast_message:
      print("Empty broadcast message content.")
      return

    print(f"Broadcast received from direction: {emitter_direction}")

    decrypted_message = encryption.decrypt_message(broadcast_message)

    if decrypted_message is not None:
      print(f"Decrypted broadcast message: {decrypted_message}")

      msg_parts = decrypted_message.split('-', 3)
      if len(msg_parts) != 4:
        print(f"Invalid decrypted message format (expected 4 parts): {decrypted_message}")
        return

      msg_type, channel_id_str, sender_agent_id_str, payload = msg_parts

      try:
        channel_id = int(channel_id_str)
        sender_agent_id = int(sender_agent_id_str)
      except ValueError:
        print(f"Invalid channel_id or sender_agent_id in decrypted message: {decrypted_message}")
        return

      if msg_type == 'O':
        self._handle_open_channel(channel_id, sender_agent_id, payload)
      elif msg_type == 'C':
        self._handle_close_channel(channel_id, sender_agent_id, payload)
      elif msg_type == 'I':
        self._handle_info_message(channel_id, sender_agent_id, payload)
      else:
        print(f"Unknown message type: {msg_type} in decrypted message: {decrypted_message}")
    else:
      print(f"Enemy broadcast message (decryption failed): {broadcast_message}")
      return


  def _handle_open_channel(self, channel_id, sender_agent_id, topic):
    if channel_id in self.known_channels:
      # print(f"Channel {channel_id} already known. Ignoring open request from agent {sender_agent_id}.")
      return

    self.known_channels[channel_id] = {'topic': topic, 'original_sender_id': sender_agent_id}
    print(f"Channel {channel_id} opened by agent {sender_agent_id} with topic: '{topic}'.")

    # Agent decides if the channel topic is of interest
    if hasattr(self.agent, 'is_topic_of_interest') and self.agent.is_topic_of_interest(channel_id, topic):
      self.subscribe_to_channel(channel_id)
    else:
      print(f"Topic '{topic}' for channel {channel_id} is not of interest or agent cannot evaluate.")


  def _handle_close_channel(self, channel_id, sender_agent_id, message):
    if channel_id in self.known_channels:
      print(f"Closing channel {channel_id} (received from agent {sender_agent_id}). Message: '{message}'")
      self.unsubscribe_from_channel(channel_id)
      del self.known_channels[channel_id]
      print(f"Channel {channel_id} closed and forgotten.")
    else:
      # print(f"Received close for unknown channel {channel_id} from agent {sender_agent_id}.")
      pass


  def _handle_info_message(self, channel_id, sender_agent_id, message):
    if channel_id not in self.known_channels:
      print(f"Received info for unknown channel {channel_id} from agent {sender_agent_id}. Ignoring.")
      return

    if channel_id not in self.subscribed_channels:
      # print(f"Received info for channel {channel_id} (from agent {sender_agent_id}) but not subscribed. Ignoring.")
      return

    print(f"Received info for subscribed channel {channel_id} from agent {sender_agent_id}: '{message}'")
    if hasattr(self.agent, 'process_channel_message'):
      self.agent.process_channel_message(channel_id, sender_agent_id, message)
    else:
      print(f"Agent has no method to process channel message for channel {channel_id}.")


  def subscribe_to_channel(self, channel_id):
    if channel_id not in self.known_channels:
      print(f"Cannot subscribe to unknown channel {channel_id}.")
      return
    if channel_id in self.subscribed_channels:
      # print(f"Already subscribed to channel {channel_id}.")
      return

    self.subscribed_channels.add(channel_id)
    topic = self.known_channels[channel_id]['topic']
    print(f"Subscribed to channel {channel_id} (Topic: {topic}).")
    if hasattr(self.agent, 'on_subscribe_to_channel'):
      self.agent.on_subscribe_to_channel(channel_id, topic)


  def unsubscribe_from_channel(self, channel_id):
    if channel_id not in self.subscribed_channels:
      return

    self.subscribed_channels.remove(channel_id)
    topic = self.known_channels.get(channel_id, {}).get('topic', 'N/A')
    print(f"Unsubscribed from channel {channel_id} (Topic: {topic}).")
    if hasattr(self.agent, 'on_unsubscribe_from_channel'):
      self.agent.on_unsubscribe_from_channel(channel_id, topic)


  def _generate_unique_channel_id(self):
    if not self.known_channels:
      return 1
    return max(self.known_channels.keys()) + 1


  def send_open_channel_broadcast(self, topic):
    if not hasattr(self.agent, 'id'):
        print("Agent is missing 'id' attribute for sending broadcast.")
        return None

    channel_id = self._generate_unique_channel_id()
    agent_id = self.agent.id
    message_content = f"O-{channel_id}-{agent_id}-{topic}"
    encrypted_message = encryption.encrypt_message(message_content)

    if encrypted_message:
      # print(f"Agent {agent_id} sending OPEN channel broadcast: {message_content}")
      self.agent.send_message("Broadcast " + encrypted_message)

      # subscribes to its own channel
      self.known_channels[channel_id] = {'topic': topic, 'original_sender_id': agent_id}
      self.subscribe_to_channel(channel_id) # Opener should track its own channel.
      # print(f"Agent {agent_id} opened and auto-subscribed to new channel {channel_id} with topic '{topic}'.")
      return channel_id
    else:
      print(f"Failed to encrypt message for opening channel with topic: {topic}")
      return None


  def send_close_channel_broadcast(self, channel_id, close_message="Channel closed by sender"):
    if not hasattr(self.agent, 'id'):
        print("Agent is missing 'id' attribute for sending broadcast.")
        return

    if channel_id not in self.known_channels:
      print(f"Cannot send close for unknown or already closed channel {channel_id}.")
      return

    agent_id = self.agent.id
    message_content = f"C-{channel_id}-{agent_id}-{close_message}"
    encrypted_message = encryption.encrypt_message(message_content)

    if encrypted_message:
      # print(f"Agent {agent_id} sending CLOSE channel broadcast: {message_content}")
      self.agent.send_message("Broadcast " + encrypted_message)

      self.unsubscribe_from_channel(channel_id)
      if channel_id in self.known_channels:
          del self.known_channels[channel_id]
      print(f"Agent {agent_id} sent close for channel {channel_id} and updated local state.")
    else:
      print(f"Failed to encrypt message for closing channel {channel_id}.")


  def send_info_broadcast(self, channel_id, message):
    if not hasattr(self.agent, 'id'):
        print("Agent is missing 'id' attribute for sending broadcast.")
        return

    if channel_id not in self.subscribed_channels:
      print(f"Cannot send info on channel {channel_id} as not subscribed.")
      return
    if channel_id not in self.known_channels:
      print(f"Cannot send info on unknown channel {channel_id}.")
      return

    agent_id = self.agent.id
    message_content = f"I-{channel_id}-{agent_id}-{message}"
    encrypted_message = encryption.encrypt_message(message_content)

    if encrypted_message:
      # print(f"Agent {agent_id} sending INFO broadcast on channel {channel_id}: {message_content}")
      self.agent.send_message("Broadcast " + encrypted_message)
    else:
      print(f"Failed to encrypt message for info on channel {channel_id}.")
