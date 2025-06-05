import utils.zappy as zappy

# translates the "cone" look position into an x y relative pos and gets to the position
def go_to_pos_with_distance(agent, distance):
  if (distance == 0):
    return

  xrelative, yrelative = zappy.get_item_relative_pos(distance)
  for _ in range(yrelative):
    agent.send_command("Forward")

  if xrelative > 0:
    agent.send_command("Right")
  elif xrelative < 0:
    agent.send_command("Left")

  for _ in range(abs(xrelative)):
    agent.send_command("Forward")

def take_all_of_item_here(agent, item):
  surroundings = agent.send_command("Look")
  while zappy.get_closest_of_item(surroundings, item) == 0:
    response = agent.send_command("Take " + item)
    if (response is None or "ko" in response):
      print(f"Failed to take {item}. Response: {response}")
      break
    surroundings = agent.send_command("Look")

def go_take_item(agent, item):
  surroundings = agent.send_command("Look")
  closetest_item_distance = zappy.get_closest_of_item(surroundings, item)

  nb_turns = 0
  i = 0
  while zappy.get_closest_of_item(surroundings, item) == -1:
    if nb_turns >= 3:
      for _ in range(agent.level * i):
        agent.send_command("Forward")
      i += 1
      nb_turns = 0
    nb_turns += 1
    agent.send_command("Left")
    surroundings = agent.send_command("Look")
    if i > 3:
      break

  if closetest_item_distance == 0:
    take_all_of_item_here(agent, item)
    return

  distance_to_item = zappy.get_closest_of_item(surroundings, item)

  go_to_pos_with_distance(agent, distance_to_item)
  take_all_of_item_here(agent, item)
