/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** message
*/

#ifndef MESSAGE_HPP_
#define MESSAGE_HPP_

#include <string>
#include <vector>
#include "HeaderMessage.hpp"

class Message {
    public:
        enum MessageType {
            MAP_SIZE,
            TILE_CONTENT,
            ALL_TILES_CONTENT,
            TEAM_NAMES,
            PLAYER_CONNECTED,
            PLAYER_POSITION,
            PLAYER_LEVEL,
            PLAYER_INVENTORY,
            PLAYER_EXPELLED,
            PLAYER_BROADCAST,
            PLAYER_DIED,
            RESOURCE_DROPPED,
            RESOURCE_COLLECTED,
            INCANTATION_STARTED,
            INCANTATION_ENDED,
            EGG_LAID,
            EGG_HATCHED,
            EGG_DIED,
            EGG_LAYING,
            TIME_UNIT,
            GAME_ENDED,
            SERVER_MESSAGE,
            UNKNOWN_COMMAND
        };
        Message();
        explicit Message(std::string header, std::string data);
        explicit Message(MessageType type);
        ~Message() = default;

        void setMessage(const std::string &data);
        void setHeader(const std::string &header);
        void setData(const std::string &data);

        const std::string &getMessage() const;
        const std::string &getHeader() const;
        const std::string &getData() const;

        HeaderMessage::MessageType getType() const;
        void setType(HeaderMessage::MessageType type);
        // Additional methods for parameter management
        void addIntParam(int value);
        void addStringParam(const std::string &value);
        const std::vector<std::string> &getParameters() const;
        void setParameters(const std::vector<std::string> &params);
        int getIntParam(size_t index) const;

    private:
        std::string _messageString;
        std::string _messageHeader;
        std::string _messageData;
        HeaderMessage::MessageType _messageType;
        std::vector<std::string> _parameters;
};

#endif /* !MESSAGE_HPP_ */
