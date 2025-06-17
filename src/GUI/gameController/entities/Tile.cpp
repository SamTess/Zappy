/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile - Implémentation des méthodes pour les tuiles
*/

#include "Tile.hpp"
#include <algorithm>
#include <memory>
#include <vector>

Tile::Tile(int x, int y) : _x(x), _y(y), _resources{0}, _isIncantating(false) {}

Tile::Tile(const TileContentData& data) : _x(data.getX()), _y(data.getY()), _isIncantating(false) {
    updateFromProtocol(data);
}

int Tile::getX() const {
    return _x;
}

int Tile::getY() const {
    return _y;
}

const std::array<int, 7>& Tile::getResources() const {
    return _resources;
}

int Tile::getResourceQuantity(ResourceType type) const {
    int index = static_cast<int>(type);
    return (index >= 0 && index < 7) ? _resources[index] : 0;
}

void Tile::setResourceQuantity(ResourceType type, int quantity) {
    int index = static_cast<int>(type);
    if (index >= 0 && index < 7) {
        _resources[index] = std::max(0, quantity);
    }
}

ResourceType Tile::getDominantResourceType() const {
    int maxQuantity = 0;
    ResourceType dominantType = ResourceType::FOOD;
    for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
        if (_resources[i] > maxQuantity) {
            maxQuantity = _resources[i];
            dominantType = static_cast<ResourceType>(i);
        }
    }
    return dominantType;
}

const std::vector<int>& Tile::getPlayerIds() const {
    return _playerIds;
}

const std::vector<int>& Tile::getEggIds() const {
    return _eggIds;
}

bool Tile::isIncantating() const {
    return _isIncantating;
}

void Tile::setIncantating(bool incantating) {
    _isIncantating = incantating;
}

void Tile::addPlayer(int playerId) {
    if (std::find(_playerIds.begin(), _playerIds.end(), playerId) == _playerIds.end()) {
        _playerIds.push_back(playerId);
    }
}

void Tile::removePlayer(int playerId) {
    _playerIds.erase(
        std::remove(_playerIds.begin(), _playerIds.end(), playerId),
        _playerIds.end()
    );
}

void Tile::addEgg(int eggId) {
    if (std::find(_eggIds.begin(), _eggIds.end(), eggId) == _eggIds.end()) {
        _eggIds.push_back(eggId);
    }
}

void Tile::removeEgg(int eggId) {
    _eggIds.erase(
        std::remove(_eggIds.begin(), _eggIds.end(), eggId),
        _eggIds.end()
    );
}

void Tile::updateFromProtocol(const TileContentData& data) {
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

// modifier render et pourquoi pas faire un prendre de tout ce qu'il a à l'intérieur de lui
void Tile::render(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    const ZappyTypes::Vector3& position,
    float tileSize) const {
    if (!graphicsLib)
        return;
    ZappyTypes::Color tileColor = {100, 100, 100, 255};
    graphicsLib->DrawCube(position, tileSize, 0.1f, tileSize, tileColor);
}
