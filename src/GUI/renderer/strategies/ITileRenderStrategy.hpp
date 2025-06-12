/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** TileRendererStrategy interface
*/

#pragma once
#include <memory>
#include "../../graphicalContext/GraphicalContext.hpp"
#include "../../../Shared/IGraphicsLib.hpp"

namespace Zappy {
class ITileRenderStrategy {
public:
    virtual ~ITileRenderStrategy() = default;
    virtual void renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        int x, int y,
        const ZappyTypes::Color& color,
        float tileSize,
        float spacing) = 0;
};
} // namespace Zappy
