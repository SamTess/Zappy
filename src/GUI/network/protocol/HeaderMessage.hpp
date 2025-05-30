/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** headerMessage
*/

#ifndef HEADERMESSAGE_HPP_
#define HEADERMESSAGE_HPP_

// Map related headers
#define MSZ_HEADER "msz"  // Map size
#define BCT_HEADER "bct"  // Content of a tile
#define MCT_HEADER "mct"  // Content of the map (all tiles)

// Team related headers
#define TNA_HEADER "tna"  // Name of all the teams

// Player related headers
#define PNW_HEADER "pnw"  // Connection of a new player
#define PPO_HEADER "ppo"  // Player's position
#define PLV_HEADER "plv"  // Player's level
#define PIN_HEADER "pin"  // Player's inventory
#define PEX_HEADER "pex"  // Expulsion
#define PBC_HEADER "pbc"  // Broadcast

#define PDR_HEADER "pdr"  // Resource dropping
#define PGT_HEADER "pgt"  // Resource collecting
#define PDI_HEADER "pdi"  // Death of a player

// Incantation related headers
#define PIC_HEADER "pic"  // Start of an incantation
#define PIE_HEADER "pie"  // End of an incantation

// Egg related headers
#define ENW_HEADER "enw"  // An egg was laid by a player
#define EBO_HEADER "ebo"  // Player connection for an egg
#define EDI_HEADER "edi"  // Death of an egg
#define PFK_HEADER "pfk"  // Egg laying by the player

// Time related headers
#define SGT_HEADER "sgt"  // Time unit request
#define SST_HEADER "sst"  // Time unit modification

// Game related headers
#define SEG_HEADER "seg"  // End of game
#define SMG_HEADER "smg"  // Message from the server

// Error related headers
#define SUC_HEADER "suc"  // Unknown command
#define SBP_HEADER "sbp"  // Command parameter

// Enumération des types de messages
namespace HeaderMessage {
    enum MessageType {
        UNKNOWN = 0,
        MAP_SIZE,
        TILE_CONTENT,
        ALL_TILES_CONTENT,
        TEAM_NAMES,
        PLAYER_CONNECTION,
        PLAYER_POSITION,
        PLAYER_LEVEL,
        PLAYER_INVENTORY,
        PLAYER_EXPULSION,
        PLAYER_BROADCAST,
        PLAYER_DEATH,
        RESOURCE_DROP,
        RESOURCE_COLLECT,
        INCANTATION_START,
        INCANTATION_END,
        EGG_LAYING,
        EGG_DROP,
        EGG_CONNECTION,
        EGG_DEATH,
        TIME_UNIT,
        END_GAME,
        SERVER_MESSAGE,
        UNKNOWN_COMMAND
    };
}

#endif /* !HEADERMESSAGE_HPP_ */
