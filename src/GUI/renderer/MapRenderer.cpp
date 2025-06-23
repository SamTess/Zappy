/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Map Renderer
*/

#include <algorithm>
#include <cmath>
#include <memory>
#include <string>
#include <vector>
#include "MapRenderer.hpp"
#include "strategies/TileRenderStrategyFactory.hpp"
#include "../gameController/GameState.hpp"

namespace Zappy {

MapRenderer::MapRenderer(const std::shared_ptr<IGraphicsLib>& graphics,
    const std::shared_ptr<const GameState>& state,
    const std::shared_ptr<ModelManagerAdapter>& modelManagerAdapter)
    : graphicsLib(graphics),
      gameState(state),
      strategyFactory(modelManagerAdapter),
      tileSize(1.0f),
      tileSpacing(1.5f),
      zoomLevel(1.0f),
      detailThreshold(2.0f) {
    tileRenderStrategy = strategyFactory.createSimpleTileStrategy(gameState);
    detailedTileStrategy = strategyFactory.createDetailedTileStrategy(gameState);
}

void MapRenderer::initialize() {
    resourceColors[static_cast<int>(ResourceType::FOOD)] = {255, 255, 150, 255};
    resourceColors[static_cast<int>(ResourceType::LINEMATE)] = {100, 255, 100, 255};
    resourceColors[static_cast<int>(ResourceType::DERAUMERE)] = {100, 100, 255, 255};
    resourceColors[static_cast<int>(ResourceType::SIBUR)] = {255, 100, 100, 255};
    resourceColors[static_cast<int>(ResourceType::MENDIANE)] = {200, 100, 200, 255};
    resourceColors[static_cast<int>(ResourceType::PHIRAS)] = {255, 200, 100, 255};
    resourceColors[static_cast<int>(ResourceType::THYSTAME)] = {100, 200, 255, 255};
    resourceColors[-1] = {150, 150, 150, 255};
}

void MapRenderer::render() {
    renderWithSelection(-1, -1, -1);
}

void MapRenderer::renderWithSelection(int selectedTileX, int selectedTileY, int selectedPlayerId) {
    if (!gameState)
        return;
    int mapWidth = gameState->getMapWidth();
    int mapHeight = gameState->getMapHeight();

    if (mapWidth <= 0 || mapHeight <= 0)
        return;
    static bool firstRender = true;
    if (firstRender)
        firstRender = false;
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            ResourceType dominantResource = gameState->getDominantResourceType(x, y);
            int resourceIndex = static_cast<int>(dominantResource);
            renderTile(x, y, resourceIndex);
        }
    }
    if (selectedTileX >= 0 && selectedTileY >= 0 &&
        selectedTileX < mapWidth && selectedTileY < mapHeight) {
        renderTileSelectionEffect(selectedTileX, selectedTileY);
    }
    if (selectedPlayerId >= 0)
        renderPlayerSelectionEffect(selectedPlayerId);
    renderVictoryScreen();
}

void MapRenderer::renderVictoryScreen() {
    if (!gameState->isGameEnded())
        return;
    auto winningTeam = gameState->getWinningTeam();
    if (winningTeam.empty())
        return;
    std::string text = "Team " + winningTeam + " wins!";
    int mapWidth = gameState->getMapWidth();
    int mapHeight = gameState->getMapHeight();
    float fontSize = 2.0f;
    float fontSpacing = 1.0f;
    float lineSpacing = 1.0f;
    ZappyTypes::Vector3 textSize = graphicsLib->MeasureText3D(text, fontSize, fontSpacing, lineSpacing);
    float centerX = (mapWidth - 1) / 2.0f - textSize.x / 2.0f;
    float centerZ = (mapHeight - 1) / 2.0f - textSize.z / 2.0f;
    std::vector<ZappyTypes::Color> colors = {
        {255, 0, 0, 255},
        {255, 165, 0, 255},
        {255, 255, 0, 255},
        {0, 255, 0, 255},
        {0, 0, 255, 255},
        {75, 0, 130, 255},
        {148, 0, 211, 255},
        {255, 20, 147, 255},
        {0, 255, 255, 255},
        {255, 105, 180, 255}
    };
    float currentX = centerX;
    for (size_t i = 0; i < text.length(); ++i) {
        char c = text[i];
        ZappyTypes::Color color = colors[i % colors.size()];
        std::string singleChar(1, c);
        ZappyTypes::Vector3 charPosition = {currentX, 5.0f, centerZ};
        graphicsLib->DrawText3D(singleChar, charPosition, fontSize, fontSpacing, lineSpacing, true, color);
        ZappyTypes::Vector3 charSize = graphicsLib->MeasureText3D(singleChar, fontSize, fontSpacing, lineSpacing);
        currentX += charSize.x + fontSpacing;
    }
}

void MapRenderer::setTileRenderStrategy(std::shared_ptr<ITileRenderStrategy> strategy) {
    if (strategy)
        tileRenderStrategy = strategy;
}

void MapRenderer::setResourceRenderStrategy(int resourceType, std::shared_ptr<ITileRenderStrategy> strategy) {
    if (strategy)
        resourceRenderStrategies[resourceType] = strategy;
}

void MapRenderer::setTileSize(float size) {
    tileSize = std::max(0.1f, size);
}

void MapRenderer::setTileSpacing(float spacing) {
    tileSpacing = std::max(0.0f, spacing);
}

void MapRenderer::setResourceColor(int resourceType, const ZappyTypes::Color& color) {
    resourceColors[resourceType] = color;
}

void MapRenderer::setZoomLevel(float zoom) {
    zoomLevel = std::max(0.1f, zoom);
}

void MapRenderer::setDetailThreshold(float threshold) {
    detailThreshold = threshold;
}

void MapRenderer::renderTile(int x, int y, int /*resourceType*/) {
    ZappyTypes::Color tileColor = calculateTileColor(x, y);
    detailedTileStrategy->renderTile(graphicsLib, x, y, tileColor, tileSize, tileSpacing);
}

ZappyTypes::Color MapRenderer::calculateTileColor(int x, int y) {
    if (resourceColors.find(-1) == resourceColors.end())
        return {150, 150, 150, 255};
    auto tile = gameState->getTile(x, y);
    if (tile && tile->isIncantating()) {
        return {50, 50, 255, 200};
    }
    ResourceType dominantType = gameState->getDominantResourceType(x, y);
    int resourceIndex = static_cast<int>(dominantType);
    if (dominantType == ResourceType::COUNT || resourceColors.find(resourceIndex) == resourceColors.end()) {
        return resourceColors[-1];
    }
    return resourceColors[resourceIndex];
}

void MapRenderer::renderTileSelectionEffect(int x, int y) {
    if (!gameState)
        return;
    float mapCenterX = gameState->getMapWidth() / 2.0f;
    float mapCenterY = gameState->getMapHeight() / 2.0f;
    ZappyTypes::Vector3 position = {
        (x - mapCenterX + 0.5f) * (tileSize + tileSpacing),
        0.8f,
        (y - mapCenterY + 0.5f) * (tileSize + tileSpacing)
    };
    float time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    float borderWidth = 0.08f;
    float cornerSize = 0.12f;
    float cornerHeight = 0.45f + 0.15f * sin(time * 5.0f);
    ZappyTypes::Color cornerColor = {255, 255, 255, static_cast<unsigned char>(150 + 105 * sin(time * 6.0f))};
    float cornerOffset = tileSize / 2.0f + borderWidth / 2.0f;
    graphicsLib->DrawCube({position.x - cornerOffset, position.y + cornerHeight, position.z - cornerOffset},
                         cornerSize, cornerSize, cornerSize, cornerColor);
    graphicsLib->DrawCube({position.x + cornerOffset, position.y + cornerHeight, position.z - cornerOffset},
                         cornerSize, cornerSize, cornerSize, cornerColor);
    graphicsLib->DrawCube({position.x - cornerOffset, position.y + cornerHeight, position.z + cornerOffset},
                         cornerSize, cornerSize, cornerSize, cornerColor);
    graphicsLib->DrawCube({position.x + cornerOffset, position.y + cornerHeight, position.z + cornerOffset},
                         cornerSize, cornerSize, cornerSize, cornerColor);
}

void MapRenderer::renderPlayerSelectionEffect(int playerId) {
    if (!gameState)
        return;
    auto player = gameState->getPlayerInfo(playerId);
    if (!player)
        return;
    int playerX = player->getX();
    int playerY = player->getY();
    float mapCenterX = gameState->getMapWidth() / 2.0f;
    float mapCenterY = gameState->getMapHeight() / 2.0f;
    ZappyTypes::Vector3 basePosition = {
        (playerX - mapCenterX + 0.5f) * (tileSize + tileSpacing),
        0.0f,
        (playerY - mapCenterY + 0.5f) * (tileSize + tileSpacing)
    };
    auto playersOnTile = gameState->getPlayersOnTile(playerX, playerY);
    int playerIndex = 0;
    for (size_t i = 0; i < playersOnTile.size(); ++i) {
        if (playersOnTile[i] == playerId) {
            playerIndex = i;
            break;
        }
    }
    ZappyTypes::Vector3 playerPosition = basePosition;
    if (playersOnTile.size() > 1) {
        float stackHeight = 1.1f;
        playerPosition.y += playerIndex * stackHeight;
    }
    playerPosition.y += 0.55f;
    float time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
    float pulseIntensity = 0.8f + 0.2f * sin(time * 4.0f);
    ZappyTypes::Color wireframeColor = {255, 255, 255, static_cast<unsigned char>(255 * pulseIntensity)};
    float wireframeSize = 0.5f * (1.0f + 0.1f * sin(time * 6.0f));
    graphicsLib->DrawCubeWires(playerPosition, wireframeSize, wireframeSize, wireframeSize, wireframeColor);
    ZappyTypes::Color selectionColor = {0, 255, 255, static_cast<unsigned char>(120 * pulseIntensity)};
    float radius = 0.6f * pulseIntensity;
    float height = 2.5f;
    graphicsLib->DrawCylinder({playerPosition.x, playerPosition.y - 0.3f, playerPosition.z},
                             radius, radius, height, 12, selectionColor);
    ZappyTypes::Color ringColor = {255, 255, 0, static_cast<unsigned char>(200 * pulseIntensity)};
    float ringRadius = 0.8f + 0.1f * sin(time * 2.0f);
    int segments = 20;
    float rotationOffset = time * 2.0f;
    for (int i = 0; i < segments; ++i) {
        float angle = (2.0f * M_PI * i) / segments + rotationOffset;
        ZappyTypes::Vector3 ringPos = {
            basePosition.x + static_cast<float>(ringRadius * cos(angle)),
            basePosition.y + 0.05f,
            basePosition.z + static_cast<float>(ringRadius * sin(angle))
        };
        if (i % 2 == 0) {
            graphicsLib->DrawCube(ringPos, 0.1f, 0.1f, 0.1f, ringColor);
        } else {
            ZappyTypes::Color wireRingColor = {255, 255, 0, static_cast<unsigned char>(150 * pulseIntensity)};
            graphicsLib->DrawCubeWires(ringPos, 0.12f, 0.12f, 0.12f, wireRingColor);
        }
    }
    int particleCount = 8;
    for (int i = 0; i < particleCount; ++i) {
        float particleTime = time + (i * 0.5f);
        float particleHeight = 0.3f + 0.2f * sin(particleTime * 3.0f);
        float particleAngle = (2.0f * M_PI * i) / particleCount + time * 1.5f;
        float particleRadius = 0.4f + 0.1f * sin(particleTime * 4.0f);
        ZappyTypes::Vector3 particlePos = {
            playerPosition.x + static_cast<float>(particleRadius * cos(particleAngle)),
            playerPosition.y + particleHeight,
            playerPosition.z + static_cast<float>(particleRadius * sin(particleAngle))
        };
        ZappyTypes::Color particleColor = {255, 255, 255, static_cast<unsigned char>(100 + 155 * sin(particleTime * 5.0f))};
        graphicsLib->DrawCube(particlePos, 0.05f, 0.05f, 0.05f, particleColor);
    }
}

} // namespace Zappy
