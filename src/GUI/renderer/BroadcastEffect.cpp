/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** BroadcastEffect
*/

#include <random>
#include <algorithm>
#include <cmath>
#include <utility>
#include <memory>
#include <string>
#include <iostream>
#include <vector>
#include "BroadcastEffect.hpp"
#include "../gameController/GameState.hpp"
#include "EjectionAnimationManager.hpp"

namespace Zappy {

BroadcastEffect::BroadcastEffect() : currentRadius(0.0f), maxRadius(12.0f), expansionSpeed(4.0f),
    pulseInterval(0.5f), lastPulseTime(0.0f), ringCount(2),
    followPlayerId(-1), followPlayer(false) {
    effectDuration = 2.5f;
    ringRadii.resize(ringCount, 0.0f);
    ringAlphas.resize(ringCount, 1.0f);
}

void BroadcastEffect::initialize(const ZappyTypes::Vector3& position, ParticleType particleType, int maxParticles) {
    origin = position;
    type = particleType;
    timeAlive = 0.0f;
    active = true;
    currentRadius = 0.0f;
    lastPulseTime = 0.0f;
    followPlayer = false;
    followPlayerId = -1;
    gameState = nullptr;
    for (int i = 0; i < ringCount; ++i) {
        ringRadii[i] = 0.0f;
        ringAlphas[i] = 1.0f;
    }
    particles.clear();
    particles.reserve(maxParticles);
}

void BroadcastEffect::initialize(int playerId, std::shared_ptr<const GameState> gameStatePtr, ParticleType particleType, int maxParticles) {
    followPlayerId = playerId;
    gameState = gameStatePtr;
    followPlayer = true;
    type = particleType;
    timeAlive = 0.0f;
    active = true;
    currentRadius = 0.0f;
    lastPulseTime = 0.0f;
    origin = getCurrentPlayerPosition();
    for (int i = 0; i < ringCount; ++i) {
        ringRadii[i] = 0.0f;
        ringAlphas[i] = 1.0f;
    }
    particles.clear();
    particles.reserve(maxParticles);
}

void BroadcastEffect::update(float deltaTime) {
    if (!active)
        return;
    timeAlive += deltaTime;
    if (timeAlive >= effectDuration) {
        active = false;
        return;
    }
    for (int i = 0; i < ringCount; ++i) {
        float startTime = i * pulseInterval;
        if (timeAlive >= startTime) {
            ringRadii[i] += expansionSpeed * deltaTime;
            float ringAge = timeAlive - startTime;
            ringAlphas[i] = std::max(0.0f, 1.0f - (ringAge / (effectDuration - startTime)));
        }
    }
}

void BroadcastEffect::render(const std::shared_ptr<IGraphicsLib>& graphicsLib) {
    if (!active || !graphicsLib)
        return;
    ZappyTypes::Vector3 currentPosition;
    if (followPlayer)
        currentPosition = getCurrentPlayerPosition();
    else
        currentPosition = origin;
    for (int i = 0; i < ringCount; ++i) {
        if (ringRadii[i] > 0.0f && ringAlphas[i] > 0.0f)
            renderRing(graphicsLib, currentPosition, ringRadii[i], ringAlphas[i]);
    }
}

bool BroadcastEffect::isActive() const {
    return active;
}

void BroadcastEffect::reset() {
    active = false;
    timeAlive = 0.0f;
    currentRadius = 0.0f;
    particles.clear();
}

void BroadcastEffect::renderRing(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    const ZappyTypes::Vector3& center, float radius, float alpha) {
    if (radius <= 0.0f || alpha <= 0.0f)
        return;
    int segments = 32;
    float angleStep = 2.0f * M_PI / segments;
    ZappyTypes::Color ringColor = {255, 100, 255, static_cast<unsigned char>(alpha * 255)};
    for (int i = 0; i < segments; ++i) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        ZappyTypes::Vector3 point1 = {center.x + static_cast<float>(cos(angle1)) * radius, center.y + 0.2f, center.z + static_cast<float>(sin(angle1)) * radius};
        ZappyTypes::Vector3 point2 = {center.x + static_cast<float>(cos(angle2)) * radius, center.y + 0.2f, center.z + static_cast<float>(sin(angle2)) * radius};
        graphicsLib->DrawLine3D(point1, point2, ringColor);
    }
}

ZappyTypes::Vector3 BroadcastEffect::getCurrentPlayerPosition() const {
    if (!gameState || !followPlayer || followPlayerId == -1)
        return origin;
    auto player = gameState->getPlayerInfo(followPlayerId);
    if (player) {
        ZappyTypes::Vector3 playerWorldPos = Zappy::EjectionAnimationManager::getInstance().convertTileToWorldPosition(
            player->getX(), player->getY(), gameState->getMapWidth(), gameState->getMapHeight());
        playerWorldPos.y = 1.0f;
        return playerWorldPos;
    }
    return origin;
}

}  // namespace Zappy
