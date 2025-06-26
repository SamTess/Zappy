/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Egg
*/

#include "Egg.hpp"
#include <cmath>
#include <memory>
#include "../../textureManager/ModelManager.hpp"

Egg::Egg() : _eggId(0), _x(0), _y(0) {}

Egg::Egg(const EggData& data)
    : _eggId(data.getEggId()), _x(data.getX()), _y(data.getY()) {}

int Egg::getId() const {
    return _eggId;
}

int Egg::getX() const {
    return _x;
}

int Egg::getY() const {
    return _y;
}

void Egg::setPosition(int x, int y) {
    _x = x;
    _y = y;
}

void Egg::updateFromProtocol(const EggData& data) {
    _eggId = data.getEggId();
    _x = data.getX();
    _y = data.getY();
}

void Egg::render(const std::shared_ptr<IGraphicsLib>& graphicsLib,
                const ZappyTypes::Vector3& position,
                float tileSize) const {
    renderEgg(graphicsLib, position, tileSize);
}

void Egg::renderEgg(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    const ZappyTypes::Vector3& position,
    float tileSize,
    int stackIndex) const {
    if (!graphicsLib)
        return;
    ZappyTypes::Vector3 eggPos = position;
    float eggHeight = 0.3f;
    eggPos.y = position.y + 0.5f + (stackIndex * eggHeight * 0.8f);
    eggPos.x -= tileSize * 0.18f;
    eggPos.z -= tileSize * -0.8f;

    ModelManager::getInstance().drawModel(TRIPY_TROPHY, eggPos, 0.25f);
}
