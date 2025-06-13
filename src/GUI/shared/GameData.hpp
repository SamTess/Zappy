/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Game data structures
*/

#pragma once

#include <string>
#include <vector>

namespace GUI {

// Structure pour représenter l'inventaire d'un joueur ou d'une case
struct Inventory {
    int food = 0;
    int linemate = 0;
    int deraumere = 0;
    int sibur = 0;
    int mendiane = 0;
    int phiras = 0;
    int thystame = 0;
};

// Structure pour représenter un joueur
struct Player {
    int id = 0;
    std::string team;
    int x = 0;
    int y = 0;
    int orientation = 0;  // 1: nord, 2: est, 3: sud, 4: ouest
    int level = 1;
    Inventory inventory;
    bool isIncantating = false;
    bool isEgg = false;
};

// Structure pour représenter une case de la map
struct Tile {
    int x = 0;
    int y = 0;
    int food = 0;
    int linemate = 0;
    int deraumere = 0;
    int sibur = 0;
    int mendiane = 0;
    int phiras = 0;
    int thystame = 0;
};

// Structure pour représenter une équipe
struct Team {
    std::string name;
    int remainingSlots = 0;
};

// Structure principale contenant toutes les données du jeu
struct GameData {
    // Informations sur la map
    int mapWidth = 10;
    int mapHeight = 10;
    
    // Informations temporelles
    float time = 0.0f;
    int frequency = 100;
    int tick = 0;
    
    // Données du jeu
    std::vector<Player> players;
    std::vector<Tile> tiles;
    std::vector<Team> teams;
};

} // namespace GUI
