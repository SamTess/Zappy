/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EjectionAnimationManager
*/

#include "EjectionAnimationManager.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <iostream>

namespace Zappy {

EjectionAnimationManager& EjectionAnimationManager::getInstance() {
    static EjectionAnimationManager instance;
    return instance;
}

void EjectionAnimationManager::startEjectionAnimation(int playerId, const ZappyTypes::Vector3& fromPosition,
    const ZappyTypes::Vector3& toPosition) {
    EjectionAnimation animation;
    animation.playerId = playerId;
    animation.startPosition = fromPosition;
    animation.endPosition = toPosition;
    animation.direction = calculateDirection(fromPosition, toPosition);
    animation.duration = defaultAnimationDuration;
    animation.timeElapsed = 0.0f;
    animation.active = true;
    animation.particlesCreated = false;

    activeAnimations[playerId] = animation;
}

void EjectionAnimationManager::update(float deltaTime) {
    for (auto& pair : activeAnimations) {
        EjectionAnimation& animation = pair.second;

        if (!animation.active) continue;

        animation.timeElapsed += deltaTime;

        if (!animation.particlesCreated) {
            ParticleSystem::getInstance().createPlayerEjectionEffect(
                animation.startPosition, animation.direction);
            animation.particlesCreated = true;
        }

        if (animation.timeElapsed >= animation.duration) {
            animation.active = false;
        }
    }

    removeCompletedAnimations();
}

void EjectionAnimationManager::render(const std::shared_ptr<IGraphicsLib>& graphicsLib) {
    if (!graphicsLib) return;

    for (const auto& pair : activeAnimations) {
        const EjectionAnimation& animation = pair.second;

        if (!animation.active) continue;

        float progress = animation.timeElapsed / animation.duration;
        if (progress < 0.5f) {
            ZappyTypes::Color lineColor = {255, 100, 100, static_cast<unsigned char>(255 * (1.0f - progress * 2.0f))};
            ZappyTypes::Vector3 lineEnd = animation.startPosition;
            lineEnd.x += animation.direction.x * 2.0f;
            lineEnd.z += animation.direction.z * 2.0f;
            lineEnd.y += 0.1f;

            graphicsLib->DrawLine3D(animation.startPosition, lineEnd, lineColor);
        }
    }
}

bool EjectionAnimationManager::isPlayerBeingEjected(int playerId) const {
    auto it = activeAnimations.find(playerId);
    return it != activeAnimations.end() && it->second.active;
}

ZappyTypes::Vector3 EjectionAnimationManager::getPlayerAnimationPosition(int playerId) const {
    auto it = activeAnimations.find(playerId);
    if (it == activeAnimations.end() || !it->second.active)
        return {0, 0, 0};

    const EjectionAnimation& animation = it->second;
    float progress = animation.timeElapsed / animation.duration;
    progress = std::min(1.0f, progress);

    float easedProgress = easeOutQuad(progress);

    return interpolatePosition(animation.startPosition, animation.endPosition, easedProgress);
}

ZappyTypes::Vector3 EjectionAnimationManager::convertTileToWorldPosition(int tileX, int tileY, int mapWidth, int mapHeight) const {
    float tileSize = 1.0f;
    float spacing = 1.5f;

    ZappyTypes::Vector3 result = {
        (static_cast<float>(tileX) - mapWidth / 2.0f + 0.5f) * (tileSize + spacing),
        0.5f,
        (static_cast<float>(tileY) - mapHeight / 2.0f + 0.5f) * (tileSize + spacing)
    };

    std::cout << "[DEBUG] convertTileToWorldPosition: tile(" << tileX << "," << tileY
              << ") -> world(" << result.x << "," << result.y << "," << result.z << ")"
              << " [map:" << mapWidth << "x" << mapHeight << "]" << std::endl;

    return result;
}

void EjectionAnimationManager::cleanup() {
    activeAnimations.clear();
}

void EjectionAnimationManager::removeCompletedAnimations() {
    for (auto it = activeAnimations.begin(); it != activeAnimations.end();) {
        if (!it->second.active) {
            it = activeAnimations.erase(it);
        } else {
            ++it;
        }
    }
}

ZappyTypes::Vector3 EjectionAnimationManager::calculateDirection(const ZappyTypes::Vector3& from, const ZappyTypes::Vector3& to) {
    ZappyTypes::Vector3 direction;
    direction.x = to.x - from.x;
    direction.y = 0.0f;
    direction.z = to.z - from.z;

    float length = std::sqrt(direction.x * direction.x + direction.z * direction.z);
    if (length > 0.0f) {
        direction.x /= length;
        direction.z /= length;
    }

    return direction;
}

ZappyTypes::Vector3 EjectionAnimationManager::interpolatePosition(const ZappyTypes::Vector3& start, const ZappyTypes::Vector3& end, float t) const {
    ZappyTypes::Vector3 result;
    result.x = start.x + (end.x - start.x) * t;
    result.y = start.y + (end.y - start.y) * t;
    result.z = start.z + (end.z - start.z) * t;
    return result;
}

float EjectionAnimationManager::easeOutQuad(float t) const {
    return 1.0f - (1.0f - t) * (1.0f - t);
}

}  // namespace Zappy
