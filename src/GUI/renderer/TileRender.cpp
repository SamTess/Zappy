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
#include "TileRender.hpp"
#include "../gameController/entities/Tile.hpp"
#include "../gameController/entities/Player.hpp"
#include "../gameController/entities/Resource.hpp"
#include "../gameController/entities/Egg.hpp"
#include "../gameController/GameState.hpp"
#include "EjectionAnimationManager.hpp"
#include "DeathAnimationManager.hpp"

namespace Zappy {

DetailedTileRenderStrategy::DetailedTileRenderStrategy(const std::shared_ptr<const GameState>& gameState)
    : gameState(gameState) {}


void DetailedTileRenderStrategy::renderAllResources(std::shared_ptr<const ITile> tile,
    const std::shared_ptr<IGraphicsLib>& graphicsLib, const ZappyTypes::Vector3& position, float tileSize) {
    const auto& resources = tile->getResources();

    for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
        int quantity = resources[i];
        if (quantity > 0) {
            Resource resource(static_cast<ResourceType>(i), quantity);
            resource.renderResource(graphicsLib, position, tileSize);
        }
    }
}

void DetailedTileRenderStrategy::renderAllPlayers(std::shared_ptr<const ITile> tile,
    const std::shared_ptr<IGraphicsLib>& graphicsLib, const ZappyTypes::Vector3& position, float tileSize) {
    const auto& playerIds = tile->getPlayerIds();

    for (size_t i = 0; i < playerIds.size(); ++i) {
        int playerId = playerIds[i];
        auto playerInfo = gameState->getPlayerInfo(playerId);
        if (playerInfo) {
            ZappyTypes::Vector3 renderPosition = position;
            if (EjectionAnimationManager::getInstance().isPlayerBeingEjected(playerId)) {
                ZappyTypes::Vector3 animPos = EjectionAnimationManager::getInstance().getPlayerAnimationPosition(playerId);
                if (animPos.x != 0 || animPos.y != 0 || animPos.z != 0)
                    renderPosition = animPos;
            } else if (!playerInfo->isMoving()) {
                renderPosition = position;
            } else {
                continue;
            }
            playerInfo->renderPlayer(graphicsLib, renderPosition, tileSize, i, playerIds.size());
        }
    }
}

void DetailedTileRenderStrategy::renderAllMovingPlayers(const std::shared_ptr<IGraphicsLib>& graphicsLib, float tileSize, float spacing) {
    const auto& players = gameState->getPlayers();

    for (const auto& [playerId, playerInfo] : players) {
        if (playerInfo && playerInfo->isMoving() &&
            !EjectionAnimationManager::getInstance().isPlayerBeingEjected(playerId)) {
            ZappyTypes::Vector3 interpolatedPos = playerInfo->getInterpolatedPosition();
            float mapCenterX = gameState->getMapWidth() / 2.0f;
            float mapCenterY = gameState->getMapHeight() / 2.0f;
            ZappyTypes::Vector3 renderPosition = {(interpolatedPos.x - mapCenterX + 0.5f) * (tileSize + spacing), interpolatedPos.y,
                (interpolatedPos.z - mapCenterY + 0.5f) * (tileSize + spacing)};
            playerInfo->renderPlayer(graphicsLib, renderPosition, tileSize, 0, 1);
        }
    }
}

void DetailedTileRenderStrategy::renderAllEggs(std::shared_ptr<const ITile> tile,
    const std::shared_ptr<IGraphicsLib>& graphicsLib, const ZappyTypes::Vector3& position, float tileSize) {
    const auto& eggIds = tile->getEggIds();

    for (size_t i = 0; i < eggIds.size(); ++i) {
        int eggId = eggIds[i];
        auto eggInfo = gameState->getEggInfo(eggId);
        if (eggInfo)
            eggInfo->renderEgg(graphicsLib, position, tileSize, i);
    }
}

void DetailedTileRenderStrategy::renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    int x, int y, float tileSize, float spacing) {
    ZappyTypes::Vector3 position = {
        (x - gameState->getMapWidth() / 2.0f + 0.5f) * (tileSize + spacing), 0.0f,
        (y - gameState->getMapHeight() / 2.0f + 0.5f) * (tileSize + spacing)};

    auto tile = gameState->getTile(x, y);
    tile->render(graphicsLib, position, tileSize);
    renderAllResources(tile, graphicsLib, position, tileSize);
    renderAllPlayers(tile, graphicsLib, position, tileSize);
    renderAllEggs(tile, graphicsLib, position, tileSize);
}

} // namespace Zappy
