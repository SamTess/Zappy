/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resource - Implémentation des méthodes pour les ressources
*/

#include "Resource.hpp"
#include <algorithm>
#include <memory>
#include <string>

Resource::Resource(ResourceType type, int quantity)
    : _type(type), _quantity(quantity) {
    updateName();
}

ResourceType Resource::getType() const {
    return _type;
}

int Resource::getQuantity() const {
    return _quantity;
}

void Resource::setQuantity(int quantity) {
    _quantity = std::max(0, quantity);
}

void Resource::addQuantity(int amount) {
    _quantity = std::max(0, _quantity + amount);
}

bool Resource::isEmpty() const {
    return _quantity <= 0;
}

const std::string& Resource::getName() const {
    return _name;
}

void Resource::render(const std::shared_ptr<IGraphicsLib>& graphicsLib,
            const ZappyTypes::Vector3& position,
            float tileSize) const {
    renderResource(graphicsLib, position, tileSize);
}

void Resource::updateName() {
    switch (_type) {
        case ResourceType::FOOD: _name = "food"; break;
        case ResourceType::LINEMATE: _name = "linemate"; break;
        case ResourceType::DERAUMERE: _name = "deraumere"; break;
        case ResourceType::SIBUR: _name = "sibur"; break;
        case ResourceType::MENDIANE: _name = "mendiane"; break;
        case ResourceType::PHIRAS: _name = "phiras"; break;
        case ResourceType::THYSTAME: _name = "thystame"; break;
        default: _name = "unknown"; break;
    }
}

void Resource::renderResource(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    const ZappyTypes::Vector3& position,
    float tileSize) const {
    if (!graphicsLib || _quantity <= 0)
        return;

    const ZappyTypes::Color colorRessources[] = {
        {255, 255, 150, 255},
        {0, 255, 0, 255},
        {0, 0, 255, 255},
        {255, 0, 0, 255},
        {255, 0, 255, 255},
        {0, 255, 255, 255},
        {255, 215, 0, 255}
    };
    const ZappyTypes::Color borderColor = {80, 80, 80, 255};
    int resourceIndex = static_cast<int>(_type);
    if (resourceIndex < 0 || resourceIndex >= static_cast<int>(ResourceType::COUNT))
        return;
    ZappyTypes::Color resourceColor = colorRessources[resourceIndex];
    float indicatorSize = tileSize * 0.2f;
    float spacing = tileSize * 0.4f;
    ZappyTypes::Vector3 indicatorPos = position;
    indicatorPos.y = position.y + 0.15f;

    int row = resourceIndex / 3;
    int col = resourceIndex % 3;
    indicatorPos.x += (col - 1) * spacing;
    indicatorPos.z += (row - 1) * spacing;

    for (int i = 0; i < _quantity; ++i) {
        ZappyTypes::Vector3 cubePos = indicatorPos;
        cubePos.y += i * indicatorSize * 0.8f;
        graphicsLib->DrawCube(cubePos, indicatorSize, indicatorSize, indicatorSize, resourceColor);
        float halfSize = indicatorSize / 2;
        float topY = cubePos.y + halfSize + 0.001f;
        graphicsLib->DrawLine3D({cubePos.x - halfSize, topY, cubePos.z - halfSize},
            {cubePos.x + halfSize, topY, cubePos.z - halfSize}, borderColor);
        graphicsLib->DrawLine3D({cubePos.x + halfSize, topY, cubePos.z - halfSize},
            {cubePos.x + halfSize, topY, cubePos.z + halfSize}, borderColor);
        graphicsLib->DrawLine3D({cubePos.x + halfSize, topY, cubePos.z + halfSize},
            {cubePos.x - halfSize, topY, cubePos.z + halfSize}, borderColor);
        graphicsLib->DrawLine3D({cubePos.x - halfSize, topY, cubePos.z + halfSize},
            {cubePos.x - halfSize, topY, cubePos.z - halfSize}, borderColor);
    }
}
