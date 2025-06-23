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
    // toyute les couleurs pour chaque lettre
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

} // namespace Zappy
