/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Detailed tile Renderer
*/
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include "DetailedTileRenderStrategy.hpp"
#include "../../gameController/entities/Tile.hpp"
#include "../../gameController/entities/Player.hpp"
#include "../../gameController/entities/Resource.hpp"
#include "../../gameController/entities/Egg.hpp"
#include "../../gameController/GameState.hpp"
#include "../EjectionAnimationManager.hpp"

namespace Zappy {

DetailedTileRenderStrategy::DetailedTileRenderStrategy(const std::shared_ptr<const GameState>& gameState)
    : gameState(gameState) {}

void DetailedTileRenderStrategy::renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    int x, int y,
    const ZappyTypes::Color& color,
    float tileSize,
    float spacing) {
    ZappyTypes::Vector3 position = {
        (x - gameState->getMapWidth() / 2.0f + 0.5f) * (tileSize + spacing),
        0.0f,
        (y - gameState->getMapHeight() / 2.0f + 0.5f) * (tileSize + spacing)
    };

    auto tile = gameState->getTile(x, y);
    (void)color;
    tile->render(graphicsLib, position, tileSize);
    if (tile) {
        const auto& resources = tile->getResources();
        for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
            int quantity = resources[i];
            if (quantity > 0) {
                Resource resource(static_cast<ResourceType>(i), quantity);
                resource.renderResource(graphicsLib, position, tileSize);
            }
        }

        const auto& playerIds = tile->getPlayerIds();
        for (size_t i = 0; i < playerIds.size(); ++i) {
            int playerId = playerIds[i];
            auto playerInfo = gameState->getPlayerInfo(playerId);
            if (playerInfo) {
                ZappyTypes::Vector3 renderPosition = position;
                if (Zappy::EjectionAnimationManager::getInstance().isPlayerBeingEjected(playerId)) {
                    ZappyTypes::Vector3 animPos = Zappy::EjectionAnimationManager::getInstance().getPlayerAnimationPosition(playerId);
                    if (animPos.x != 0 || animPos.y != 0 || animPos.z != 0)
                        renderPosition = animPos;
                }
                playerInfo->renderPlayer(graphicsLib, renderPosition, tileSize, i, playerIds.size());
            } else {
                std::cout << "WARNING: Player " << playerId << " not found in gameState during rendering!" << std::endl;
            }
        }
        const auto& eggIds = tile->getEggIds();
        for (size_t i = 0; i < eggIds.size(); ++i) {
            int eggId = eggIds[i];
            auto eggInfo = gameState->getEggInfo(eggId);
            if (eggInfo) {
                eggInfo->renderEgg(graphicsLib, position, tileSize, i);
            }
        }
    }
}

void DetailedTileRenderStrategy::renderResourceIndicator(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    ZappyTypes::Vector3 position,
    ResourceType resourceType,
    int quantity,
    float tileSize) {
    ZappyTypes::Color resourceColors[] = {
        {255, 255, 150, 255},
        {0, 255, 0, 255},
        {0, 0, 255, 255},
        {255, 0, 0, 255},
        {255, 0, 255, 255},
        {0, 255, 255, 255},
        {255, 215, 0, 255}
    };
    ZappyTypes::Color borderColor = {80, 80, 80, 255};
    float indicatorSize = tileSize * 0.2f;
    float spacing = tileSize * 0.4f;
    int resourceIndex = static_cast<int>(resourceType);
    ZappyTypes::Vector3 indicatorPos = position;
    indicatorPos.y = position.y + 0.15f;
    int row = resourceIndex / 3;
    int col = resourceIndex % 3;
    indicatorPos.x += (col - 1) * spacing;
    indicatorPos.z += (row - 1) * spacing;
    graphicsLib->DrawCube(indicatorPos, indicatorSize, indicatorSize, indicatorSize, resourceColors[resourceIndex]);
    float halfSize = indicatorSize / 2;
    graphicsLib->DrawLine3D({indicatorPos.x - halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z - halfSize},
        {indicatorPos.x + halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z - halfSize},
        borderColor);
    graphicsLib->DrawLine3D({indicatorPos.x + halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z - halfSize},
        {indicatorPos.x + halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z + halfSize},
        borderColor);
    graphicsLib->DrawLine3D({indicatorPos.x + halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z + halfSize},
        {indicatorPos.x - halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z + halfSize},
        borderColor);
    graphicsLib->DrawLine3D({indicatorPos.x - halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z + halfSize},
        {indicatorPos.x - halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z - halfSize},
        borderColor);
    for (int i = 1; i < std::min(quantity, 5); ++i) {
        indicatorPos.y += indicatorSize * 0.8f;
        graphicsLib->DrawCube(indicatorPos, indicatorSize, indicatorSize, indicatorSize, resourceColors[resourceIndex]);
        graphicsLib->DrawLine3D({indicatorPos.x - halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z - halfSize},
            {indicatorPos.x + halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z - halfSize},
            borderColor);
        graphicsLib->DrawLine3D({indicatorPos.x + halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z - halfSize},
            {indicatorPos.x + halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z + halfSize},
            borderColor);
        graphicsLib->DrawLine3D({indicatorPos.x + halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z + halfSize},
            {indicatorPos.x - halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z + halfSize},
            borderColor);
        graphicsLib->DrawLine3D({indicatorPos.x - halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z + halfSize},
            {indicatorPos.x - halfSize, indicatorPos.y + halfSize + 0.001f, indicatorPos.z - halfSize},
            borderColor);
    }
}

void DetailedTileRenderStrategy::renderText3D(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    const std::string& text,
    ZappyTypes::Vector3 position,
    float fontSize,
    ZappyTypes::Color color) {
    if (text.empty() || fontSize <= 0.05f)
        return;
    float fontSpacing = 0.1f;
    float lineSpacing = -0.1f;
    bool backface = true;
    graphicsLib->DrawText3D(text, position, fontSize, fontSpacing, lineSpacing, backface, color);
}

void DetailedTileRenderStrategy::renderEggIndicator(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    ZappyTypes::Vector3 position,
    int /*eggId*/,
    float tileSize) {
    ZappyTypes::Color eggColor = {255, 255, 255, 255};
    ZappyTypes::Color borderColor = {80, 80, 80, 255};
    float eggSize = tileSize * 0.15f;
    ZappyTypes::Vector3 eggPos = position;
    eggPos.y = position.y + 0.2f;
    eggPos.x -= tileSize * 0.25f;
    eggPos.z -= tileSize * 0.25f;
    graphicsLib->DrawSphere(eggPos, eggSize, eggColor);
    float offset = eggSize * 0.7f;
    graphicsLib->DrawLine3D({eggPos.x - offset, eggPos.y, eggPos.z},
                           {eggPos.x + offset, eggPos.y, eggPos.z},
                           borderColor);
    graphicsLib->DrawLine3D({eggPos.x, eggPos.y - offset, eggPos.z},
                           {eggPos.x, eggPos.y + offset, eggPos.z},
                           borderColor);
    graphicsLib->DrawLine3D({eggPos.x, eggPos.y, eggPos.z - offset},
                           {eggPos.x, eggPos.y, eggPos.z + offset},
                           borderColor);
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

} // namespace Zappy
