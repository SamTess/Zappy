/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerInventory
*/

#include "PlayerInventory.hpp"

PlayerInventory::PlayerInventory() : _playerId(0), _x(0), _y(0), _resources{0} {}

PlayerInventory::PlayerInventory(const PlayerInventoryData& data)
    : _playerId(data.getId()), _x(data.getX()), _y(data.getY()),
    _resources{data.getFood(), data.getLinemate(), data.getDeraumere(),
    data.getSibur(), data.getMendiane(), data.getPhiras(), data.getThystame()} {}

int PlayerInventory::getPlayerId() const {
    return _playerId;
}

int PlayerInventory::getX() const {
    return _x;
}

int PlayerInventory::getY() const {
    return _y;
}

const std::array<int, 7>& PlayerInventory::getResources() const {
    return _resources;
}

int PlayerInventory::getFood() const {
    return _resources[0];
}

int PlayerInventory::getLinemate() const {
    return _resources[1];
}

int PlayerInventory::getDeraumere() const {
    return _resources[2];
}

int PlayerInventory::getSibur() const {
    return _resources[3];
}

int PlayerInventory::getMendiane() const {
    return _resources[4];
}

int PlayerInventory::getPhiras() const {
    return _resources[5];
}

int PlayerInventory::getThystame() const {
    return _resources[6];
}

void PlayerInventory::updateFromProtocol(const PlayerInventoryData& data) {
    _playerId = data.getId();
    _x = data.getX();
    _y = data.getY();
    _resources[0] = data.getFood();
    _resources[1] = data.getLinemate();
    _resources[2] = data.getDeraumere();
    _resources[3] = data.getSibur();
    _resources[4] = data.getMendiane();
    _resources[5] = data.getPhiras();
    _resources[6] = data.getThystame();
}

void PlayerInventory::setPosition(int x, int y) {
    _x = x;
    _y = y;
}

int PlayerInventory::getResourceCount(int resourceType) const {
    if (resourceType < 0 || resourceType >= static_cast<int>(_resources.size())) {
        return 0;
    }
    return _resources[resourceType];
}
