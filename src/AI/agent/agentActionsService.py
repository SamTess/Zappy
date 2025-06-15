import utils.zappy as zappy


class AgentActionManager:
  def __init__(self, agent):
    self.agent = agent


  # translates the "cone" look position into an x y relative pos and gets to the position
  def go_to_pos_with_distance(self, distance):
    if (distance == 0):
      return

    xrelative, yrelative = zappy.get_item_relative_pos(distance)
    for _ in range(yrelative):
      self.agent.send_command("Forward")

    if xrelative > 0:
      self.agent.send_command("Right")
    elif xrelative < 0:
      self.agent.send_command("Left")

    for _ in range(abs(xrelative)):
      self.agent.send_command("Forward")


  def take_all_of_item_here(self, item):
    surroundings = self.agent.send_command("Look")
    distance_to_item, amount_found = zappy.get_closest_of_item(surroundings, item)

    if distance_to_item == 0:
      for _ in range(amount_found):
        response = self.agent.send_command("Take " + item)
        if response is None or "ko" in response:
          print(f"Failed to take {item}. Response: {response}")
          break
    else:
      return


  def take_everything_here(self):
    surroundings = self.agent.send_command("Look")
    if surroundings is None or "ko" in surroundings:
      print(f"take_everything_here: Failed to look around. Response: {surroundings}")
      return

    items_on_current_tile = surroundings.strip("[ ]").split(",")[0].split(" ")
    for item in items_on_current_tile:
      if item:
        item = item.strip(", .")
      if item != "player":
        self.agent.send_command("Take " + item)


  def go_take_item(self, item):
    nb_turns = 0
    i = 0

    while True:
      surroundings = self.agent.send_command("Look")
      if surroundings is None or "ko" in surroundings:
        print(f"go_take_item: Failed to look around. Response: {surroundings}")
        return
      closest_item_distance = zappy.get_closest_of_item(surroundings, item)
      if closest_item_distance != -1 or i > 4:
        break
      self.agent.send_command("Right")
      nb_turns += 1
      if nb_turns > 3:
        i += 1
        for _ in range(i * self.agent.level):
          self.agent.send_command("Forward")
        nb_turns = 0

    if closest_item_distance == 0:
      self.take_all_of_item_here(item)
      return

    distance_to_item = zappy.get_closest_of_item(surroundings, item)

    self.go_to_pos_with_distance(distance_to_item)
    self.take_all_of_item_here(item)

# retourne la nouvelle direction de la cible après les actions
  def got_to_dir(self, direction):
    if direction == 1:
      self.agent.send_command("Forward")
      return 1
    elif direction == 2:
      self.agent.send_command("Forward")
      self.agent.send_command("Left")
      self.agent.send_command("Forward")
      self.agent.send_command("Right")
      return 2
    elif direction == 3:
      self.agent.send_command("Left")
      self.agent.send_command("Forward")
      return 1
    elif direction == 4:
      self.agent.send_command("Left")
      self.agent.send_command("Forward")
      return 2
    elif direction == 5:
      self.agent.send_command("Left")
      self.agent.send_command("Left")
      self.agent.send_command("Forward")
      return 1
    elif direction == 6:
      self.agent.send_command("Left")
      self.agent.send_command("Left")
      self.agent.send_command("Forward")
      return 2
    elif direction == 7:
      self.agent.send_command("Right")
      self.agent.send_command("Forward")
      return 1
    elif direction == 8:
      self.agent.send_command("Right")
      self.agent.send_command("Forward")
      return 2
