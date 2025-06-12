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
#include "../graphicalContext/GraphicalContext.hpp"

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

ModelTileRenderStrategy::ModelTileRenderStrategy(const std::shared_ptr<ModelManager>& manager, int id, const std::shared_ptr<GraphicalContext>& ctx)
    : modelManager(manager), modelId(id), context(ctx) {}

void ModelTileRenderStrategy::renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
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

DetailedTileRenderStrategy::DetailedTileRenderStrategy(const std::shared_ptr<GraphicalContext>& ctx)
    : context(ctx) {}

void DetailedTileRenderStrategy::renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
                                      int x, int y,
                                      const ZappyTypes::Color& color,
                                      float tileSize,
                                      [[maybe_unused]] float spacing) {
    float mapOffset = context->getMapWidth() / 2.0f;
    ZappyTypes::Vector3 position = {
        (x - mapOffset + 0.5f) * (tileSize + spacing),
        0.0f,
        (y - mapOffset + 0.5f) * (tileSize + spacing)
    };
    graphicsLib->DrawCube(position, tileSize, 0.1f, tileSize, color);
    ZappyTypes::Color borderColor = {100, 100, 100, 255};
    float offset = tileSize/2;
    graphicsLib->DrawLine3D({position.x - offset, position.y + 0.05f, position.z - offset},
                           {position.x + offset, position.y + 0.05f, position.z - offset},
                           borderColor);
    graphicsLib->DrawLine3D({position.x + offset, position.y + 0.05f, position.z - offset},
                           {position.x + offset, position.y + 0.05f, position.z + offset},
                           borderColor);
    graphicsLib->DrawLine3D({position.x + offset, position.y + 0.05f, position.z + offset},
                           {position.x - offset, position.y + 0.05f, position.z + offset},
                           borderColor);
    graphicsLib->DrawLine3D({position.x - offset, position.y + 0.05f, position.z + offset},
                           {position.x - offset, position.y + 0.05f, position.z - offset},
                           borderColor);
    const TileData& tileData = context->getTileData(x, y);
    for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
        int quantity = tileData.resources[i];
        if (quantity > 0) {
            renderResourceIndicator(graphicsLib, position, static_cast<ResourceType>(i), quantity, tileSize);
        }
    }
    for (int playerId : tileData.playerIds) {
        renderPlayerIndicator(graphicsLib, position, playerId, tileSize);
    }
    for (int eggId : tileData.eggIds) {
        renderEggIndicator(graphicsLib, position, eggId, tileSize);
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

void DetailedTileRenderStrategy::renderPlayerIndicator(const std::shared_ptr<IGraphicsLib>& graphicsLib,
                                                       ZappyTypes::Vector3 position,
                                                       int playerId,
                                                       float tileSize) {
    const std::shared_ptr<PlayerInfoData> playerInfo = context->getPlayerInfo(playerId);
    if (!playerInfo) return;
    ZappyTypes::Color playerColor = {255, 0, 0, 255};
    ZappyTypes::Color borderColor = {80, 80, 80, 255};
    ZappyTypes::Color directionColor = {255, 255, 0, 255};
    float playerSize = tileSize * 0.25f;
    float playerHeight = tileSize * 0.5f;
    ZappyTypes::Vector3 playerPos = position;
    playerPos.y = position.y + 0.4f;
    graphicsLib->DrawCylinder(playerPos, playerSize, playerSize, playerHeight, 8, playerColor);
    float halfSize = playerSize;
    float topY = playerPos.y + playerHeight/2.0f;
    graphicsLib->DrawLine3D({playerPos.x - halfSize, topY, playerPos.z - halfSize},
                           {playerPos.x + halfSize, topY, playerPos.z - halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, topY, playerPos.z - halfSize},
                           {playerPos.x + halfSize, topY, playerPos.z + halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, topY, playerPos.z + halfSize},
                           {playerPos.x - halfSize, topY, playerPos.z + halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x - halfSize, topY, playerPos.z + halfSize},
                           {playerPos.x - halfSize, topY, playerPos.z - halfSize},
                           borderColor);
    float bottomY = playerPos.y - playerHeight/2.0f;
    graphicsLib->DrawLine3D({playerPos.x - halfSize, bottomY, playerPos.z - halfSize},
                           {playerPos.x + halfSize, bottomY, playerPos.z - halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, bottomY, playerPos.z - halfSize},
                           {playerPos.x + halfSize, bottomY, playerPos.z + halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, bottomY, playerPos.z + halfSize},
                           {playerPos.x - halfSize, bottomY, playerPos.z + halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x - halfSize, bottomY, playerPos.z + halfSize},
                           {playerPos.x - halfSize, bottomY, playerPos.z - halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x - halfSize, bottomY, playerPos.z - halfSize},
                           {playerPos.x - halfSize, topY, playerPos.z - halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, bottomY, playerPos.z - halfSize},
                           {playerPos.x + halfSize, topY, playerPos.z - halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, bottomY, playerPos.z + halfSize},
                           {playerPos.x + halfSize, topY, playerPos.z + halfSize},
                           borderColor);
    graphicsLib->DrawLine3D({playerPos.x - halfSize, bottomY, playerPos.z + halfSize},
                           {playerPos.x - halfSize, topY, playerPos.z + halfSize},
                           borderColor);
    if (playerInfo) {
        float arrowSize = playerSize * 1.5f;
        float arrowHeight = topY + 0.5f;
        ZappyTypes::Vector3 arrowStart = playerPos;
        arrowStart.y = arrowHeight;
        ZappyTypes::Vector3 arrowEnd = arrowStart;
        int orientation = playerInfo->getOrientation();
        switch (orientation) {
            case 1: // North
                arrowEnd.z -= arrowSize;
                break;
            case 2: // East
                arrowEnd.x += arrowSize;
                break;
            case 3: // South
                arrowEnd.z += arrowSize;
                break;
            case 4: // West
                arrowEnd.x -= arrowSize;
                break;
            default:
                break;
        }
        graphicsLib->DrawLine3D(arrowStart, arrowEnd, directionColor);
        float arrowHeadSize = arrowSize * 0.3f;
        ZappyTypes::Vector3 arrowHeadLeft = arrowEnd;
        ZappyTypes::Vector3 arrowHeadRight = arrowEnd;

        switch (orientation) {
            case 1: // North
                arrowHeadLeft.x -= arrowHeadSize * 0.5f;
                arrowHeadLeft.z += arrowHeadSize;
                arrowHeadRight.x += arrowHeadSize * 0.5f;
                arrowHeadRight.z += arrowHeadSize;
                break;
            case 2: // East
                arrowHeadLeft.x -= arrowHeadSize;
                arrowHeadLeft.z -= arrowHeadSize * 0.5f;
                arrowHeadRight.x -= arrowHeadSize;
                arrowHeadRight.z += arrowHeadSize * 0.5f;
                break;
            case 3: // South
                arrowHeadLeft.x -= arrowHeadSize * 0.5f;
                arrowHeadLeft.z -= arrowHeadSize;
                arrowHeadRight.x += arrowHeadSize * 0.5f;
                arrowHeadRight.z -= arrowHeadSize;
                break;
            case 4: // West
                arrowHeadLeft.x += arrowHeadSize;
                arrowHeadLeft.z -= arrowHeadSize * 0.5f;
                arrowHeadRight.x += arrowHeadSize;
                arrowHeadRight.z += arrowHeadSize * 0.5f;
                break;
            default:
                break;
        }
        graphicsLib->DrawLine3D(arrowEnd, arrowHeadLeft, directionColor);
        graphicsLib->DrawLine3D(arrowEnd, arrowHeadRight, directionColor);
        if (!playerInfo->getTeamName().empty()) {
            ZappyTypes::Vector3 textPosition = playerPos;
            textPosition.y = position.y + playerHeight + 1.0f;
            std::string teamName = playerInfo->getTeamName();
            if (teamName.length() > 10) {
                teamName = teamName.substr(0, 8) + "...";
            }
            ZappyTypes::Color textColor = {255, 255, 255, 255};
            unsigned int hash = 0;
            for (char c : playerInfo->getTeamName()) {
                hash = hash * 31 + c;
            }
            textColor.r = 128 + (hash % 128);
            textColor.g = 128 + ((hash / 256) % 128);
            textColor.b = 128 + ((hash / 65536) % 128);
            float textSize = 0.1f * playerSize;
            renderText3D(graphicsLib, teamName, textPosition, textSize, textColor);
        }
    }
}

void DetailedTileRenderStrategy::renderText3D(const std::shared_ptr<IGraphicsLib>& graphicsLib,
                                             const std::string& text,
                                             ZappyTypes::Vector3 position,
                                             float fontSize,
                                             ZappyTypes::Color color) {
    if (text.empty()) {
        return;
    }
    float fontSpacing = 0.05f;
    float lineSpacing = -0.1f;
    bool backface = true;
    graphicsLib->DrawText3D(text, position, fontSize, fontSpacing, lineSpacing, backface, color);
}

void DetailedTileRenderStrategy::renderEggIndicator(const std::shared_ptr<IGraphicsLib>& graphicsLib,
                                                  ZappyTypes::Vector3 position,
                                                  [[maybe_unused]] int eggId,
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

TileRenderStrategyFactory::TileRenderStrategyFactory(const std::shared_ptr<ModelManager>& manager)
    : modelManager(manager) {}

TileRenderStrategyFactory::TileRenderStrategyFactory(ModelManager* manager)
    : modelManager(std::shared_ptr<ModelManager>(manager, [](ModelManager*) {})) {}

std::shared_ptr<ITileRenderStrategy> TileRenderStrategyFactory::createSimpleTileStrategy(const std::shared_ptr<GraphicalContext>& ctx) {
    return std::make_shared<SimpleTileRenderStrategy>(ctx);
}

std::shared_ptr<ITileRenderStrategy> TileRenderStrategyFactory::createModelTileStrategy(int modelId, const std::shared_ptr<GraphicalContext>& ctx) {
    return std::make_shared<ModelTileRenderStrategy>(modelManager, modelId, ctx);
}

std::shared_ptr<ITileRenderStrategy> TileRenderStrategyFactory::createDetailedTileStrategy(
    const std::shared_ptr<GraphicalContext>& ctx) {
    return std::make_shared<DetailedTileRenderStrategy>(ctx);
}

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

void MapRenderer::renderTile(int x, int y, int resourceType) {
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

MapRendererObserver::MapRendererObserver(std::shared_ptr<MapRenderer> renderer)
    : renderer(renderer) {}

void MapRendererObserver::onMapSizeChanged(int width, int height) {
    if (width > 20 || height > 20) {
        float newSize = 10.0f / std::max(width, height);
        renderer->setTileSize(newSize);
        renderer->setTileSpacing(newSize * 0.1f);
    }
}

void MapRendererObserver::onTileChanged(int /*x*/, int /*y*/, const TileData& /*tileData*/) {
    //TODO(Sam) : maj avec les données du network
}

} // namespace Zappy
