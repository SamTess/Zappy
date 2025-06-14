upgrades = {
    1 : {
        "name": "lvl 1 -> 2",
        "cost": {
            "players": 1,
            "linemate": 1,
            "deraumere": 0,
            "sibur": 0,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }
    },
    2 : {
        "name": "lvl 2 -> 3",
        "cost": {
            "players": 2,
            "linemate": 1,
            "deraumere": 1,
            "sibur": 1,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }
    },
    3 : {
        "name": "lvl 3 -> 4",
        "cost": {
            "players": 2,
            "linemate": 2,
            "deraumere": 0,
            "sibur": 1,
            "mendiane": 0,
            "phiras": 2,
            "thystame": 0
        }
    },
    4 : {
        "name": "lvl 4 -> 5",
        "cost": {
            "players": 4,
            "linemate": 1,
            "deraumere": 1,
            "sibur": 2,
            "mendiane": 0,
            "phiras": 1,
            "thystame": 0
        }
    },
    5 : {
        "name": "lvl 5 -> 6",
        "cost": {
            "players": 4,
            "linemate": 1,
            "deraumere": 2,
            "sibur": 1,
            "mendiane": 3,
            "phiras": 0,
            "thystame": 0
        }
    },
    6 : {
        "name": "lvl 6 -> 7",
        "cost": {
            "players": 6,
            "linemate": 1,
            "deraumere": 2,
            "sibur": 3,
            "mendiane": 0,
            "phiras": 1,
            "thystame": 0
        }
    },
    7 : {
        "name": "lvl 7 -> 8",
        "cost": {
            "players": 6,
            "linemate": 2,
            "deraumere": 2,
            "sibur": 2,
            "mendiane": 2,
            "phiras": 2,
            "thystame": 1
        }
    },
}

def get_total_upgrade_resources(start_level=1, target_level=8):
    if start_level >= target_level or start_level < 1 or target_level > 8:
        return {
            "linemate": 0,
            "deraumere": 0,
            "sibur": 0,
            "mendiane": 0,
            "phiras": 0,
            "thystame": 0
        }

    total_resources = {
        "linemate": 0,
        "deraumere": 0,
        "sibur": 0,
        "mendiane": 0,
        "phiras": 0,
        "thystame": 0
    }

    for level in range(start_level, target_level):
        if level in upgrades:
            for resource, amount in upgrades[level]["cost"].items():
                if resource != "players":
                    total_resources[resource] += amount

    return total_resources
