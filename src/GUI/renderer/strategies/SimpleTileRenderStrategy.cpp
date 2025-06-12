/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Simple tile Renderer
*/

#include <algorithm>
#include <cmath>
#include <memory>
#include "SimpleTileRenderStrategy.hpp"
#include "../../graphicalContext/GraphicalContext.hpp"

namespace Zappy {

SimpleTileRenderStrategy::SimpleTileRenderStrategy(const std::shared_ptr<GraphicalContext>& ctx)
    : context(ctx) {}

void SimpleTileRenderStrategy::renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    int x, int y,
    const ZappyTypes::Color& color,
    float tileSize,
    float spacing) {
    float mapOffset = context->getMapWidth() / 2.0f;
    ZappyTypes::Vector3 position = {
        (x - mapOffset + 0.5f) * (tileSize + spacing),
        0.0f,
        (y - mapOffset + 0.5f) * (tileSize + spacing)
    };
    graphicsLib->DrawCube(position, tileSize, tileSize * 0.1f, tileSize, color);
    ZappyTypes::Color borderColor = {100, 100, 100, 255};
    graphicsLib->DrawLine3D({position.x - tileSize/2, position.y, position.z - tileSize/2},
                           {position.x + tileSize/2, position.y, position.z - tileSize/2},
                           borderColor);
    graphicsLib->DrawLine3D({position.x + tileSize/2, position.y, position.z - tileSize/2},
                           {position.x + tileSize/2, position.y, position.z + tileSize/2},
                           borderColor);
    graphicsLib->DrawLine3D({position.x + tileSize/2, position.y, position.z + tileSize/2},
                           {position.x - tileSize/2, position.y, position.z + tileSize/2},
                           borderColor);
    graphicsLib->DrawLine3D({position.x - tileSize/2, position.y, position.z + tileSize/2},
                           {position.x - tileSize/2, position.y, position.z - tileSize/2},
                           borderColor);
}

} // namespace Zappy
