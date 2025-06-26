/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** DeathAnimationManager
*/

#include <random>
#include <algorithm>
#include <memory>
#include <string>
#include <cmath>
#include <iostream>
#include "DeathAnimationManager.hpp"
#include "../textureManager/ModelManager.hpp"

namespace Zappy {

DeathAnimationManager& DeathAnimationManager::getInstance() {
    static DeathAnimationManager instance;
    return instance;
}

void DeathAnimationManager::startDeathAnimation(int playerId, const ZappyTypes::Vector3& position) {
    startDeathAnimation(playerId, position, "Unknown");
}

void DeathAnimationManager::startDeathAnimation(int playerId, const ZappyTypes::Vector3& position, const std::string& teamName) {
    activeAnimations.erase(playerId);
    auto animation = std::make_shared<DeathAnimation>();
    animation->playerId = playerId;
    animation->teamName = teamName;
    animation->startPosition = position;
    animation->currentPosition = position;
    animation->currentPosition.y += 1.5f;
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> velocityDist(-3.0f, 3.0f);
    std::uniform_real_distribution<float> upwardDist(6.0f, 9.0f);
    std::uniform_real_distribution<float> rotationDist(-360.0f, 360.0f);
    animation->velocity.x = velocityDist(gen);
    animation->velocity.y = upwardDist(gen);
    animation->velocity.z = velocityDist(gen);
    animation->rotationVelocity.x = rotationDist(gen);
    animation->rotationVelocity.y = rotationDist(gen) * 0.5f;
    animation->rotationVelocity.z = rotationDist(gen) * 0.3f;
    animation->rotation = {0.0f, 0.0f, 0.0f};
    animation->timeAlive = 0.0f;
    animation->duration = 3.0f;
    animation->active = true;
    animation->hasHitGround = false;
    activeAnimations[playerId] = animation;
}

void DeathAnimationManager::update(float deltaTime) {
    for (auto& pair : activeAnimations) {
        if (pair.second && pair.second->active)
            updateDeathAnimation(pair.second, deltaTime);
    }
    removeFinishedAnimations();
}

void DeathAnimationManager::updateDeathAnimation(std::shared_ptr<DeathAnimation> animation, float deltaTime) {
    if (!animation || !animation->active)
        return;
    animation->timeAlive += deltaTime;
    if (animation->timeAlive >= animation->duration) {
        animation->active = false;
        return;
    }
    animation->velocity.y += gravity * deltaTime;
    animation->currentPosition.x += animation->velocity.x * deltaTime;
    animation->currentPosition.y += animation->velocity.y * deltaTime;
    animation->currentPosition.z += animation->velocity.z * deltaTime;
    if (animation->currentPosition.y <= 0.0f && !animation->hasHitGround) {
        animation->currentPosition.y = 0.0f;
        animation->hasHitGround = true;
        animation->velocity.y = -animation->velocity.y * bounceReduction;
        animation->velocity.x *= groundFriction;
        animation->velocity.z *= groundFriction;
        animation->rotationVelocity.x *= bounceReduction;
        animation->rotationVelocity.y *= bounceReduction;
        animation->rotationVelocity.z *= bounceReduction;
    }
    if (animation->hasHitGround && animation->currentPosition.y <= 0.0f && animation->velocity.y < 0.0f) {
        animation->currentPosition.y = 0.0f;
        animation->velocity.y = -animation->velocity.y * bounceReduction;
        if (std::abs(animation->velocity.y) < 0.5f)
            animation->velocity.y = 0.0f;
    }
    animation->rotation.x += animation->rotationVelocity.x * deltaTime;
    animation->rotation.y += animation->rotationVelocity.y * deltaTime;
    animation->rotation.z += animation->rotationVelocity.z * deltaTime;
    animation->rotationVelocity.x *= rotationDamping;
    animation->rotationVelocity.y *= rotationDamping;
    animation->rotationVelocity.z *= rotationDamping;
    if (animation->hasHitGround && animation->currentPosition.y <= 0.1f) {
        animation->velocity.x *= groundFriction;
        animation->velocity.z *= groundFriction;
    }
}

void DeathAnimationManager::render(const std::shared_ptr<IGraphicsLib>& graphicsLib) {
    if (!graphicsLib) return;

    for (const auto& pair : activeAnimations) {
        const auto& animation = pair.second;
        if (!animation || !animation->active)
            continue;

        ZappyTypes::Color teamColor = {255, 255, 255, 255};
        if (!animation->teamName.empty()) {
            std::hash<std::string> hasher;
            size_t hash = hasher(animation->teamName);
            teamColor.r = static_cast<unsigned char>((hash & 0xFF0000) >> 16);
            teamColor.g = static_cast<unsigned char>((hash & 0x00FF00) >> 8);
            teamColor.b = static_cast<unsigned char>(hash & 0x0000FF);
            teamColor.a = 255;
        }
        float fadeRatio = std::max(0.3f, 1.0f - (animation->timeAlive / animation->duration));
        teamColor.a = static_cast<unsigned char>(255 * fadeRatio);
        ZappyTypes::Vector3 rotationAxis = {1.0f, 0.0f, 0.0f};
        float rotationAngle = animation->rotation.x;
        ModelManager::getInstance().drawModelEx(LABUBU, animation->currentPosition, rotationAxis, rotationAngle, animation->scale, teamColor);
    }
}

bool DeathAnimationManager::isPlayerInDeathAnimation(int playerId) const {
    auto it = activeAnimations.find(playerId);

    return it != activeAnimations.end() && it->second && it->second->active;
}

ZappyTypes::Vector3 DeathAnimationManager::getPlayerDeathPosition(int playerId) const {
    auto it = activeAnimations.find(playerId);

    if (it != activeAnimations.end() && it->second && it->second->active)
        return it->second->currentPosition;
    return {0.0f, 0.0f, 0.0f};
}

ZappyTypes::Vector3 DeathAnimationManager::getPlayerDeathRotation(int playerId) const {
    auto it = activeAnimations.find(playerId);

    if (it != activeAnimations.end() && it->second && it->second->active)
        return it->second->rotation;
    return {0.0f, 0.0f, 0.0f};
}

void DeathAnimationManager::cleanup() {
    activeAnimations.clear();
}

void DeathAnimationManager::removeFinishedAnimations() {
    auto it = activeAnimations.begin();
    while (it != activeAnimations.end()) {
        if (!it->second || !it->second->active) {
            it = activeAnimations.erase(it);
        } else {
            ++it;
        }
    }
}

ZappyTypes::Vector3 DeathAnimationManager::convertTileToWorldPosition(int tileX, int tileY, int mapWidth, int mapHeight) const {
    float tileSize = 1.0f;
    float spacing = 1.5f;
    float worldX = (static_cast<float>(tileX) - static_cast<float>(mapWidth) / 2.0f + 0.5f) * (tileSize + spacing);
    float worldZ = (static_cast<float>(mapHeight) / 2.0f - static_cast<float>(tileY) - 0.5f) * (tileSize + spacing);
    float worldY = 0.5f;

    return {worldX, worldY, worldZ};
}

}  // namespace Zappy
