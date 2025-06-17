/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IRenderable
*/

#ifndef IRENDERABLE_HPP_
#define IRENDERABLE_HPP_

#include <memory>
#include "../../Shared/Common.hpp"

class IGraphicsLib;

enum class ResourceType {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    COUNT
};

class IRenderable {
public:
    virtual ~IRenderable() = default;
    virtual void render(const std::shared_ptr<IGraphicsLib>& graphicsLib,
            const ZappyTypes::Vector3& position,
            float tileSize) const = 0;
};

#endif /* !IRENDERABLE_HPP_ */
