/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IMessageData
*/

#ifndef IMESSAGE_DATA_HPP_
#define IMESSAGE_DATA_HPP_

enum class MessageType {
    MapSize,
    TileContent,
    PlayerInfo,
    PlayerInventory,
    Broadcast,
    Incantation,
    IncantationEnd,
    Egg,
    TimeUnit,
    EndGame,
    ServerMessage,
    TeamName,
    Resource,
};

class IMessageData {
public:
    virtual ~IMessageData() = default;
    virtual MessageType getType() const = 0;
};

#endif /* !IMESSAGE_DATA_HPP_ */
