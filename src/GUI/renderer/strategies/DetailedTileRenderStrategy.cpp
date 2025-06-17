/*
** EPITECH PROJECT, 2025
** B-YEP-400 Zappy
** File description:
** Detailed tile Renderer
*/

#include "DetailedTileRenderStrategy.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include "../../gameController/entities/Resource.hpp"

namespace Zappy {

DetailedTileRenderStrategy::DetailedTileRenderStrategy(const std::shared_ptr<const GameState>& gameState)
    : gameState(gameState) {}

void DetailedTileRenderStrategy::renderTile(const std::shared_ptr<IGraphicsLib>& graphicsLib,
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

    auto tile = gameState->getTile(x, y);

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
                playerInfo->renderPlayer(graphicsLib, position, tileSize, i, playerIds.size());
            } else {
                std::cout << "WARNING: Player " << playerId << " not found in gameState during rendering!" << std::endl;
            }
        }
        const auto& eggIds = tile->getEggIds();
        for (int eggId : eggIds) {
            auto eggInfo = gameState->getEggInfo(eggId);
            if (eggInfo) {
                eggInfo->renderEgg(graphicsLib, position, tileSize);
            }
        }
    } else {
    auto tile = gameState->getTile(x, y);
    if (tile) {
        for (int i = 0; i < static_cast<int>(ResourceType::COUNT); ++i) {
            int quantity = tile->getResourceQuantity(static_cast<ResourceType>(i));
            if (quantity > 0) {
                Resource tempResource(static_cast<ResourceType>(i), quantity);
                tempResource.renderResource(graphicsLib, position, tileSize);
            }
        }
        const auto& playerIds = tile->getPlayerIds();
        for (int i = 0; i < static_cast<int>(playerIds.size()); ++i) {
            int playerId = playerIds[i];
            auto player = gameState->getPlayerInfo(playerId);
            if (player) {
                player->renderPlayer(graphicsLib, position, tileSize, i, playerIds.size());
            }
        }
        const auto& eggIds = tile->getEggIds();
        for (int eggId : eggIds) {
            auto egg = gameState->getEggInfo(eggId);
            if (egg) {
                egg->renderEgg(graphicsLib, position, tileSize);
            }
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

void DetailedTileRenderStrategy::renderPlayerIndicator(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    ZappyTypes::Vector3 position, int playerId, float tileSize, int playerIndex, int totalPlayers) {
    auto playerInfo = gameState->getPlayerInfo(playerId);
    if (!playerInfo) {
        std::cout << "WARNING: Player " << playerId << " not found in gameState during rendering!" << std::endl;
        return;
    }
    int orientation = playerInfo->getOrientation();
    std::string teamName = playerInfo->getTeamName();
    if (orientation < 1 || orientation > 4)
        orientation = 1;
    ZappyTypes::Vector3 playerPos = position;
    if (totalPlayers > 1) {
        float offsetRadius = tileSize * 0.3f;
        float angle = (2.0f * M_PI * playerIndex) / totalPlayers;
        playerPos.x += offsetRadius * std::cos(angle);
        playerPos.z += offsetRadius * std::sin(angle);
    }
    ZappyTypes::Color playerColor = {255, 0, 0, 255};
    ZappyTypes::Color borderColor = {80, 80, 80, 255};
    ZappyTypes::Color directionColor = {255, 255, 0, 255};
    float playerSize = tileSize * 0.25f;
    float playerHeight = tileSize * 0.5f;
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

    float arrowSize = playerSize * 1.5f;
    float arrowHeight = topY + 0.5f;
    ZappyTypes::Vector3 arrowStart = playerPos;
    arrowStart.y = arrowHeight;
    ZappyTypes::Vector3 arrowEnd = arrowStart;
    switch (orientation) {
        case 1: // North
            arrowEnd.z += arrowSize;
            break;
        case 4: // WEST
            arrowEnd.x += arrowSize;
            break;
        case 3: // South
            arrowEnd.z -= arrowSize;
            break;
        case 2: // EAST
            arrowEnd.x -= arrowSize;
            break;
    }
    graphicsLib->DrawLine3D(arrowStart, arrowEnd, directionColor);
    float arrowHeadSize = arrowSize * 0.3f;
    ZappyTypes::Vector3 arrowHeadLeft = arrowEnd;
    ZappyTypes::Vector3 arrowHeadRight = arrowEnd;

    switch (orientation) {
        case 1: // North
            arrowHeadLeft.x -= arrowHeadSize * 0.5f;
            arrowHeadLeft.z -= arrowHeadSize;
            arrowHeadRight.x += arrowHeadSize * 0.5f;
            arrowHeadRight.z -= arrowHeadSize;
            break;
        case 4: // WEST
            arrowHeadLeft.x -= arrowHeadSize;
            arrowHeadLeft.z -= arrowHeadSize * 0.5f;
            arrowHeadRight.x -= arrowHeadSize;
            arrowHeadRight.z += arrowHeadSize * 0.5f;
            break;
        case 3: // South
            arrowHeadLeft.x -= arrowHeadSize * 0.5f;
            arrowHeadLeft.z += arrowHeadSize;
            arrowHeadRight.x += arrowHeadSize * 0.5f;
            arrowHeadRight.z += arrowHeadSize;
            break;
        case 2: // EAST
            arrowHeadLeft.x += arrowHeadSize;
            arrowHeadLeft.z -= arrowHeadSize * 0.5f;
            arrowHeadRight.x += arrowHeadSize;
            arrowHeadRight.z += arrowHeadSize * 0.5f;
            break;
    }
    graphicsLib->DrawLine3D(arrowEnd, arrowHeadLeft, directionColor);
    graphicsLib->DrawLine3D(arrowEnd, arrowHeadRight, directionColor);
    ZappyTypes::Vector3 textPosition = playerPos;
    textPosition.y = position.y + playerHeight + 0.5f + (playerIndex * 0.3f);
    std::string displayText = teamName.empty() ? ("P" + std::to_string(playerId)) : teamName;
    if (displayText.length() > 10)
        displayText = displayText.substr(0, 8) + "...";
    float textSize = 0.4f;
    float textWidth = displayText.length() * textSize * 0.6f;
    textPosition.x -= textWidth / 2.0f;
    textPosition.y += 0.1f;
    ZappyTypes::Color textColor = ZappyTypes::Color{0, 0, 0, 255};
    renderText3D(graphicsLib, displayText, textPosition, textSize, textColor);
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
