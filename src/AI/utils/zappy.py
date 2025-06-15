import random
from constants.resources import resources
from constants.upgrades import upgrades

# translates the "cone" look position into an x y relative pos
def get_item_relative_pos(item_position):
  i = item_position
  nb_rows = 0
  row_length = 1

  while True:
    i -= row_length
    if i < 0:
      break
    row_length += 2
    nb_rows += 1

  return i + nb_rows + 1, nb_rows

# transforms inventory string into a list of pair ressource - amount
def inventory_to_dict(inventory_str):
  if not inventory_str:
    print("parse_inventory: Inventory string is empty or None.")
    return {}

  cleaned = inventory_str.strip("[ ]")
  items = cleaned.split(", ")

  resource_dict = {}
  for item in items:
    parts = item.strip().split()
    if len(parts) == 2:
      resource_name = parts[0]
      try:
        amount = int(parts[1])
        resource_dict[resource_name] = amount
      except ValueError:
        print(f"Error parsing amount for resource '{resource_name}': {parts[1]} is not an integer.")
        continue
  return resource_dict

# tries to find the closest item in surroundings string
# returns distance to the item if found, otherwise -1
# also returns the amount of the item on that tile
def get_closest_of_item(surroundings_str, item):
  if not surroundings_str:
    print("get_closest_of_item: Surroundings data is empty or None.")
    return -1

  cleaned = surroundings_str.strip("[ ]")
  tiles = cleaned.split(", ")
  distance = None
  amount_found = 0

  for i, tile in enumerate(tiles):
    parts = tile.strip().split()
    for part in parts:
      part = part.strip(", .")
      if part == item:
        distance = int(i)
        amount_found += 1
    if distance is not None:
      return distance, amount_fou nd

  return -1, 0

def go_get_item(surroundings, item):
  distance_to_item = get_closest_of_item(surroundings, item)
  if distance_to_item == 0:
    return "Take " + item
  elif distance_to_item == -1:
    return random.choice(["Forward", "Right", "Left"])
  else:
    return "Forward"

def get_best_available_resource(surroundings):
  best_available_resource = None
  best_priority = 0

  for resource in resources:
    if get_closest_of_item(surroundings, resource) != -1:
      resource_priority = resources[resource]
      if resource_priority > best_priority:
        best_priority = resource_priority
        best_available_resource = resource

  return best_available_resource


def how_much_of_item_here(surroundings, item):
  if not surroundings:
    print("is_item_here: Surroundings data is empty or None.")
    return 0

  print(surroundings)

  cleaned = surroundings.strip("[ ]")
  tiles = cleaned.split(", ")
  here = tiles[0].strip().split()

  count = 0
  for item_here in here:
    if item_here.strip() == item:
      count += 1
  return count
