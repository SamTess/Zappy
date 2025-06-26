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
#include <algorithm>
#include "../../textureManager/ModelManager.hpp"

Player::Player() : _id(0), _x(0), _y(0), _orientation(0), _level(1), _teamName(""), _isAlive(true),
_lastRotation(180.0f), _currentRotation(0.0f), _startX(0.0f), _startY(0.0f), 
    _targetX(0.0f), _targetY(0.0f), _movementProgress(1.0f), _movementDuration(0.5f), _isMoving(false) {}

Player::Player(const PlayerInfoData& data)
    : _id(data.getId()), _x(data.getX()), _y(data.getY()),
    _orientation(data.getOrientation()), _level(data.getLevel()),
    _teamName(data.getTeamName()), _isAlive(data.isAlive()),
    _startX(static_cast<float>(data.getX())), _startY(static_cast<float>(data.getY())),
    _targetX(static_cast<float>(data.getX())), _targetY(static_cast<float>(data.getY())),
    _movementProgress(1.0f), _movementDuration(0.5f), _isMoving(false) {
    _currentRotation = orientationToRotationDegrees(_orientation);
    _lastRotation = _currentRotation;
}

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
    int newX = data.getX();
    int newY = data.getY();

    if (newX != _x || newY != _y)
        startMovement(newX, newY);
    _x = newX;
    _y = newY;
    int newOrientation = data.getOrientation();
    if (newOrientation != _orientation) {
        _lastRotation = _currentRotation;
        _orientation = newOrientation;
        _currentRotation = orientationToRotationDegrees(_orientation);
    }
    if (data.getLevel() >= _level)
        _level = data.getLevel();
    if (!data.getTeamName().empty()) {
        _teamName = data.getTeamName();
    }
    _isAlive = data.isAlive();
}

void Player::setLevel(int level) {
    _level = level;
}

void Player::setOrientation(int orientation) {
    if (orientation != _orientation) {
        _lastRotation = _currentRotation;
        _orientation = orientation;
        _currentRotation = orientationToRotationDegrees(_orientation);
    }
}

void Player::setTeamName(const std::string& teamName) {
    _teamName = teamName;
}

void Player::setIsAlive(bool alive) {
    _isAlive = alive;
}

float Player::getCurrentRotation() const {
    return _currentRotation;
}

float Player::getLastRotation() const {
    return _lastRotation;
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
    ZappyTypes::Color teamColor = generateTeamColor();

    (void)tileSize;
    ZappyTypes::Vector3 playerPos = position;
    if (totalPlayers > 1) {
        float stackHeight = 1.1f;
        playerPos.y += playerIndex * stackHeight;
    }

    playerPos.y += 0.55f;
    float rotationAngle = _currentRotation;
    ZappyTypes::Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
    ModelManager::getInstance().drawModelEx(LABUBU, playerPos, rotationAxis, rotationAngle, 0.4f, teamColor);
}

float Player::orientationToRotationDegrees(int orientation) const {
    switch (orientation) {
        case 1: // RIGHT
            return 180.0f;
        case 2: // DOWN
            return 90.0f;
        case 3: // LEFT
            return 0.0f;
        case 4: // UP
            return 270.0f;
        default:
            return 0.0f;
    }
}

ZappyTypes::Color Player::generateTeamColor() const {
    if (_teamName.empty()) {
        return {255, 255, 255, 255};
    }
    int asciiSum = 0;
    for (char c : _teamName) {
        asciiSum += static_cast<int>(c);
    }
    int red = (asciiSum * 7) % 256;
    int green = (asciiSum * 13) % 256;
    int blue = (asciiSum * 17) % 256;
    return {static_cast<unsigned char>(red),
            static_cast<unsigned char>(green),
            static_cast<unsigned char>(blue),
            255};
}

void Player::updateAnimation(float deltaTime) {
    if (_isMoving) {
        _movementProgress += deltaTime / _movementDuration;
        if (_movementProgress >= 1.0f) {
            _movementProgress = 1.0f;
            _isMoving = false;
        }
    }
}

ZappyTypes::Vector3 Player::getInterpolatedPosition() const {
    if (!_isMoving) {
        return {static_cast<float>(_x), 0.0f, static_cast<float>(_y)};
    }
    float t = _movementProgress;
    t = 1.0f - (1.0f - t) * (1.0f - t);
    float interpolatedX = _startX + ((_targetX - _startX) * t);
    float interpolatedY = _startY + ((_targetY - _startY) * t);
    return {interpolatedX, 0.0f, interpolatedY};
}

bool Player::isMoving() const {
    return _isMoving;
}

void Player::startMovement(int newX, int newY) {
    if (!_isMoving) {
        _startX = static_cast<float>(_x);
        _startY = static_cast<float>(_y);
    } else {
        ZappyTypes::Vector3 currentPos = getInterpolatedPosition();
        _startX = currentPos.x;
        _startY = currentPos.z;
    }
    _targetX = static_cast<float>(newX);
    _targetY = static_cast<float>(newY);
    _movementProgress = 0.0f;
    _isMoving = true;
}
