/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Egg - Implémentation des méthodes pour les œufs
*/

#include "Egg.hpp"
#include <cmath>
#include <memory>

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
    float tileSize) const {
    if (!graphicsLib)
        return;
    ZappyTypes::Color borderColor = {80, 80, 80, 255};
    ZappyTypes::Color eggColor = {255, 255, 255, 255};
    float eggSize = tileSize * 0.15f;
    ZappyTypes::Vector3 eggPos = position;
    eggPos.y = position.y + 0.2f;
    eggPos.x -= tileSize * 0.25f;
    eggPos.z -= tileSize * 0.25f;

    graphicsLib->DrawSphere(eggPos, eggSize, eggColor);

    float offset = eggSize * 0.7f;
    graphicsLib->DrawLine3D({eggPos.x - offset, eggPos.y, eggPos.z},
        {eggPos.x + offset, eggPos.y, eggPos.z}, borderColor);
    graphicsLib->DrawLine3D({eggPos.x, eggPos.y - offset, eggPos.z},
        {eggPos.x, eggPos.y + offset, eggPos.z}, borderColor);
    graphicsLib->DrawLine3D({eggPos.x, eggPos.y, eggPos.z - offset},
        {eggPos.x, eggPos.y, eggPos.z + offset}, borderColor);

    const int numSegments = 8;
    for (int i = 0; i < numSegments; ++i) {
        float angle1 = 2 * M_PI * i / numSegments;
        float angle2 = 2 * M_PI * ((i + 1) % numSegments) / numSegments;
        float x1 = eggPos.x + offset * std::cos(angle1);
        float z1 = eggPos.z + offset * std::sin(angle1);
        float x2 = eggPos.x + offset * std::cos(angle2);
        float z2 = eggPos.z + offset * std::sin(angle2);
        graphicsLib->DrawLine3D({x1, eggPos.y, z1}, {x2, eggPos.y, z2}, borderColor);
    }
}
