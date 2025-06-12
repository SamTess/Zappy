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
#include "MapRenderer.hpp"
#include "strategies/TileRenderStrategyFactory.hpp"
#include "MapRendererObserver.hpp"
#include "../graphicalContext/GraphicalContext.hpp"

namespace Zappy {

MapRenderer::MapRenderer(const std::shared_ptr<IGraphicsLib>& graphics,
    const std::shared_ptr<GraphicalContext>& ctx,
    const std::shared_ptr<ModelManagerAdapter>& modelManagerAdapter)
    : graphicsLib(graphics),
      context(ctx),
      strategyFactory(&modelManagerAdapter->getManager()),
      tileSize(1.0f),
      tileSpacing(0.1f),
      zoomLevel(1.0f),
      detailThreshold(2.0f) {
    tileRenderStrategy = strategyFactory.createSimpleTileStrategy(ctx);
    detailedTileStrategy = strategyFactory.createDetailedTileStrategy(ctx);
    std::shared_ptr<MapRenderer> selfPtr(this, [](MapRenderer*) {});
    context->addObserver(std::make_shared<MapRendererObserver>(selfPtr));
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
    if (!context || !graphicsLib)
        return;
    int mapWidth = context->getMapWidth();
    int mapHeight = context->getMapHeight();
    if (mapWidth <= 0 || mapHeight <= 0)
        return;
    for (int y = 0; y < mapHeight; ++y) {
        for (int x = 0; x < mapWidth; ++x) {
            ResourceType dominantResource = context->getDominantResourceType(x, y);
            int resourceIndex = static_cast<int>(dominantResource);
            renderTile(x, y, resourceIndex);
        }
    }
    graphicsLib->DrawGrid(std::max(mapWidth, mapHeight), tileSize + tileSpacing);
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
    const TileData& tileData = context->getTileData(x, y);
    if (tileData.isIncantating) {
        return {50, 50, 255, 200};
    }
    ResourceType dominantType = context->getDominantResourceType(x, y);
    int resourceIndex = static_cast<int>(dominantType);
    if (dominantType == ResourceType::COUNT || resourceColors.find(resourceIndex) == resourceColors.end()) {
        return resourceColors[-1];
    }
    return resourceColors[resourceIndex];
}

} // namespace Zappy
