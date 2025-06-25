/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Model tile Renderer
*/

#include "ModelTileRenderStrategy.hpp"
#include <algorithm>
#include <cmath>
#include <memory>


namespace Zappy {

ModelTileRenderStrategy::ModelTileRenderStrategy(const std::shared_ptr<ModelManager>& manager, int id, const std::shared_ptr<const GameState>& gameState)
    : modelManager(manager), modelId(id), gameState(gameState) {}

void ModelTileRenderStrategy::renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    int x, int y,
    const ZappyTypes::Color& color,
    float tileSize,
    float spacing) {
    float mapOffset = gameState->getMapWidth() / 2.0f;
    ZappyTypes::Vector3 position = {
        (x - mapOffset + 0.5f) * (tileSize + spacing),
        0.0f,
        (y - gameState->getMapHeight() / 2.0f + 0.5f) * (tileSize + spacing)
    };
    modelManager->drawModel(modelId, position, color);
    ZappyTypes::Color baseColor = {50, 50, 50, 200};
    graphicsLib->DrawCube({position.x, position.y - 0.05f, position.z},
                         tileSize, 0.01f, tileSize, baseColor);
    ZappyTypes::Color borderColor = {100, 100, 100, 255};
    float halfSize = tileSize / 2;
    graphicsLib->DrawLine3D({position.x - halfSize, position.y - 0.05f, position.z - halfSize},
                           {position.x + halfSize, position.y - 0.05f, position.z - halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({position.x + halfSize, position.y - 0.05f, position.z - halfSize},
                           {position.x + halfSize, position.y - 0.05f, position.z + halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({position.x + halfSize, position.y - 0.05f, position.z + halfSize},
                           {position.x - halfSize, position.y - 0.05f, position.z + halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({position.x - halfSize, position.y - 0.05f, position.z + halfSize},
                           {position.x - halfSize, position.y - 0.05f, position.z - halfSize},
                           borderColor);
}

} // namespace Zappy
