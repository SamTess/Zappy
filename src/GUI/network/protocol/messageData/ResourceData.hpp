/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ResourceData
*/

#ifndef RESOURCE_DATA_HPP_
#define RESOURCE_DATA_HPP_

#include "IMessageData.hpp"

class ResourceData : public IMessageData {
    public:
        ResourceData(int playerId, int resourceType)
            : _playerId(playerId), _resourceType(resourceType) {}

        MessageType getType() const override { return MessageType::Resource; }

        int getPlayerId() const { return _playerId; }
        int getResourceType() const { return _resourceType; }

        void setPlayerId(int value) { _playerId = value; }
        void setResourceType(int value) { _resourceType = value; }

    private:
        int _playerId;
        int _resourceType;
};

#endif /* !RESOURCE_DATA_HPP_ */
