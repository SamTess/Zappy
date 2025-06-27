/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** TileRendererStrategy
*/

#pragma once
#include <memory>
#include "../../GUI/shared/IGraphicsLib.hpp"

namespace Zappy {
class ITileRenderStrategy {
    public:
        virtual ~ITileRenderStrategy() = default;
        virtual void renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
            int x, int y, float tileSize, float spacing) = 0;
};
} // namespace Zappy
