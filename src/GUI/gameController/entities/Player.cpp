/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player - Implémentation des méthodes pour les joueurs
*/

#include "Player.hpp"
#include <iostream>
#include <cmath>
#include <memory>
#include <string>

Player::Player() : _id(0), _x(0), _y(0), _orientation(0), _level(1), _teamName(""), _isAlive(true) {}

Player::Player(const PlayerInfoData& data)
    : _id(data.getId()), _x(data.getX()), _y(data.getY()),
    _orientation(data.getOrientation()), _level(data.getLevel()),
    _teamName(data.getTeamName()), _isAlive(data.isAlive()) {}

int Player::getId() const {
    return _id;
}

int Player::getX() const {
    return _x;
}

int Player::getY() const {
    return _y;
}

void Player::setPosition(int x, int y) {
    _x = x;
    _y = y;
}

int Player::getLevel() const {
    return _level;
}

int Player::getOrientation() const {
    return _orientation;
}

const std::string& Player::getTeamName() const {
    return _teamName;
}

bool Player::isAlive() const {
    return _isAlive;
}

void Player::updateFromProtocol(const PlayerInfoData& data) {
    _id = data.getId();
    _x = data.getX();
    _y = data.getY();
    _orientation = data.getOrientation();
    if (data.getLevel() >= _level)
        _level = data.getLevel();
    if (!data.getTeamName().empty())
        _teamName = data.getTeamName();
    _isAlive = data.isAlive();
}

void Player::setLevel(int level) {
    _level = level;
}

void Player::setOrientation(int orientation) {
    _orientation = orientation;
}

void Player::setTeamName(const std::string& teamName) {
    _teamName = teamName;
}

void Player::setIsAlive(bool alive) {
    _isAlive = alive;
}

void Player::render(const std::shared_ptr<IGraphicsLib>& graphicsLib,
            const ZappyTypes::Vector3& position,
            float tileSize) const {
    renderPlayer(graphicsLib, position, tileSize, 0, 1);
}

void Player::renderPlayer(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    const ZappyTypes::Vector3& position,
    float tileSize,
    int playerIndex,
    int totalPlayers) const {
    if (!graphicsLib)
        return;
    ZappyTypes::Color borderColor = {80, 80, 80, 255};
    ZappyTypes::Color playerBorder = {255, 0, 0, 255};
    ZappyTypes::Color textColor = {0, 0, 0, 255};

    ZappyTypes::Vector3 playerPos = position;
    if (totalPlayers > 1) {
        float offsetRadius = tileSize * 0.3f;
        float angle = (2.0f * M_PI * playerIndex) / totalPlayers;
        playerPos.x += offsetRadius * std::cos(angle);
        playerPos.z += offsetRadius * std::sin(angle);
    }

    float playerSize = tileSize * 0.25f;
    float playerHeight = tileSize * 0.5f;
    playerPos.y = position.y + 0.4f;

    graphicsLib->DrawCylinder(playerPos, playerSize, playerSize, playerHeight, 8, playerBorder);

    float halfSize = playerSize;
    float topY = playerPos.y + playerHeight/2.0f;
    float bottomY = playerPos.y - playerHeight/2.0f;

    graphicsLib->DrawLine3D({playerPos.x - halfSize, topY, playerPos.z - halfSize},
        {playerPos.x + halfSize, topY, playerPos.z - halfSize}, borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, topY, playerPos.z - halfSize},
        {playerPos.x + halfSize, topY, playerPos.z + halfSize}, borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, topY, playerPos.z + halfSize},
        {playerPos.x - halfSize, topY, playerPos.z + halfSize}, borderColor);
    graphicsLib->DrawLine3D({playerPos.x - halfSize, topY, playerPos.z + halfSize},
        {playerPos.x - halfSize, topY, playerPos.z - halfSize}, borderColor);

    graphicsLib->DrawLine3D({playerPos.x - halfSize, bottomY, playerPos.z - halfSize},
        {playerPos.x + halfSize, bottomY, playerPos.z - halfSize}, borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, bottomY, playerPos.z - halfSize},
        {playerPos.x + halfSize, bottomY, playerPos.z + halfSize}, borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, bottomY, playerPos.z + halfSize},
        {playerPos.x - halfSize, bottomY, playerPos.z + halfSize}, borderColor);
    graphicsLib->DrawLine3D({playerPos.x - halfSize, bottomY, playerPos.z + halfSize},
        {playerPos.x - halfSize, bottomY, playerPos.z - halfSize}, borderColor);

    graphicsLib->DrawLine3D({playerPos.x - halfSize, bottomY, playerPos.z - halfSize},
        {playerPos.x - halfSize, topY, playerPos.z - halfSize}, borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, bottomY, playerPos.z - halfSize},
        {playerPos.x + halfSize, topY, playerPos.z - halfSize}, borderColor);
    graphicsLib->DrawLine3D({playerPos.x + halfSize, bottomY, playerPos.z + halfSize},
        {playerPos.x + halfSize, topY, playerPos.z + halfSize}, borderColor);
    graphicsLib->DrawLine3D({playerPos.x - halfSize, bottomY, playerPos.z + halfSize},
        {playerPos.x - halfSize, topY, playerPos.z + halfSize}, borderColor);

    renderDirectionArrow(graphicsLib, playerPos, tileSize);

    ZappyTypes::Vector3 textPosition = playerPos;
    textPosition.y = position.y + playerHeight + 0.5f + (playerIndex * 0.3f);
    std::string displayText = _teamName.empty() ? ("P" + std::to_string(_id)) : _teamName;

    float textSize = 0.4f;
    float textWidth = displayText.length() * textSize * 0.6f;
    textPosition.x -= textWidth / 2.0f;
    textPosition.y += 0.1f;
    renderText3D(graphicsLib, displayText, textPosition, textSize, textColor);
}

void Player::renderDirectionArrow(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    const ZappyTypes::Vector3& playerPos,
    float tileSize) const {
    float arrowSize = tileSize * 0.25f * 1.5f;
    float arrowHeight = playerPos.y + tileSize * 0.5f / 2.0f + 0.5f;
    ZappyTypes::Color directionColor = {255, 255, 0, 255};
    ZappyTypes::Vector3 arrowStart = playerPos;
    arrowStart.y = arrowHeight;
    ZappyTypes::Vector3 arrowEnd = arrowStart;

    int orientation = (_orientation < 1 || _orientation > 4) ? 1 : _orientation;
    switch (orientation) {
        case 1:
            arrowEnd.z += arrowSize;
            break;
        case 4:
            arrowEnd.x += arrowSize;
            break;
        case 3:
            arrowEnd.z -= arrowSize;
            break;
        case 2:
            arrowEnd.x -= arrowSize;
            break;
    }

    graphicsLib->DrawLine3D(arrowStart, arrowEnd, directionColor);
    float arrowHeadSize = arrowSize * 0.3f;
    ZappyTypes::Vector3 arrowHeadLeft = arrowEnd;
    ZappyTypes::Vector3 arrowHeadRight = arrowEnd;

    switch (orientation) {
        case 1:
            arrowHeadLeft.x -= arrowHeadSize * 0.5f;
            arrowHeadLeft.z -= arrowHeadSize;
            arrowHeadRight.x += arrowHeadSize * 0.5f;
            arrowHeadRight.z -= arrowHeadSize;
            break;
        case 4:
            arrowHeadLeft.x -= arrowHeadSize;
            arrowHeadLeft.z -= arrowHeadSize * 0.5f;
            arrowHeadRight.x -= arrowHeadSize;
            arrowHeadRight.z += arrowHeadSize * 0.5f;
            break;
        case 3:
            arrowHeadLeft.x -= arrowHeadSize * 0.5f;
            arrowHeadLeft.z += arrowHeadSize;
            arrowHeadRight.x += arrowHeadSize * 0.5f;
            arrowHeadRight.z += arrowHeadSize;
            break;
        case 2:
            arrowHeadLeft.x += arrowHeadSize;
            arrowHeadLeft.z -= arrowHeadSize * 0.5f;
            arrowHeadRight.x += arrowHeadSize;
            arrowHeadRight.z += arrowHeadSize * 0.5f;
            break;
    }
    graphicsLib->DrawLine3D(arrowEnd, arrowHeadLeft, directionColor);
    graphicsLib->DrawLine3D(arrowEnd, arrowHeadRight, directionColor);
}

void Player::renderText3D(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    const std::string& text,
    ZappyTypes::Vector3 position,
    float fontSize,
    ZappyTypes::Color color) const {
    if (text.empty() || fontSize <= 0.05f)
        return;
    float fontSpacing = 0.1f;
    float lineSpacing = -0.1f;
    bool backface = true;
    graphicsLib->DrawText3D(text, position, fontSize, fontSpacing, lineSpacing, backface, color);
}
