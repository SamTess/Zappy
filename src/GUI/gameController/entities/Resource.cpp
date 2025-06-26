/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resource
*/

#include "Resource.hpp"
#include <algorithm>
#include <memory>
#include <string>
#include "../../textureManager/ModelManager.hpp"

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

    const ModelType colorRessources[] = {
        LA_VACA_SATURNO,
        TRULIMERO_TRULICINA,
        GARAMARAN,
        FRIGO_CAMELO,
        ESPRESSONA_SIGNORA,
        TRALALERO_TRALALA,
        TUNG_TUNG_TUNG_SAHUR
    };
    const float refScale[] = {
        0.70f, // LA_VACA_SATURNO
        0.30f, // TRULIMERO_TRULICINA
        0.50f, // GARAMARAN
        0.25f, // FRIGO_CAMELO
        0.25f, // ESPRESSONA_SIGNORA
        0.25f, // TRALALERO_TRALALA
        0.30f  // TUNG_TUNG_TUNG_SAHUR
    };
    int resourceIndex = static_cast<int>(_type);
    if (resourceIndex < 0 || resourceIndex >= static_cast<int>(ResourceType::COUNT))
        return;
    ModelType resourceColor = colorRessources[resourceIndex];
    float indicatorSize = tileSize * 0.2f;
    // float spacing = tileSize * 0.4f;
    ZappyTypes::Vector3 indicatorPos = position;
    indicatorPos.y = position.y + 0.5f;

    float resourceSpacing = tileSize * 0.3f;
    float totalWidth = 6 * resourceSpacing;
    float startOffsetX = -totalWidth / 2.0f;
    indicatorPos.x = position.x + startOffsetX + resourceIndex * resourceSpacing;
    indicatorPos.z = position.z - 0.7f;

    for (int i = 0; i < _quantity; ++i) {
        ZappyTypes::Vector3 cubePos = indicatorPos;
        cubePos.y += i * indicatorSize * 1.2f;
        ModelManager::getInstance().drawModel(resourceColor, cubePos, refScale[resourceIndex]);
    }
}
