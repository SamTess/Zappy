/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EjectionAnimationManager - Manages player ejection animations
*/

#pragma once

#include <memory>
#include <map>
#include <chrono>
#include "ParticleSystem.hpp"
#include "../../Shared/Common.hpp"

namespace Zappy {

struct EjectionAnimation {
    int playerId;
    ZappyTypes::Vector3 startPosition;
    ZappyTypes::Vector3 endPosition;
    ZappyTypes::Vector3 direction;
    float duration;
    float timeElapsed;
    bool active;
    bool particlesCreated;

    EjectionAnimation() : playerId(-1), startPosition{0, 0, 0}, endPosition{0, 0, 0},
                         direction{0, 0, 0}, duration(1.0f), timeElapsed(0.0f),
                         active(false), particlesCreated(false) {}
};

class EjectionAnimationManager {
public:
    static EjectionAnimationManager& getInstance();

    void startEjectionAnimation(int playerId, const ZappyTypes::Vector3& fromPosition,
        const ZappyTypes::Vector3& toPosition);

    void update(float deltaTime);

    void render(const std::shared_ptr<IGraphicsLib>& graphicsLib);

    bool isPlayerBeingEjected(int playerId) const;

    ZappyTypes::Vector3 getPlayerAnimationPosition(int playerId) const;

    ZappyTypes::Vector3 convertTileToWorldPosition(int tileX, int tileY, int mapWidth, int mapHeight) const;

    void cleanup();

    void setAnimationDuration(float duration) { defaultAnimationDuration = duration; }
    void setParticleIntensity(int intensity) { particleIntensity = intensity; }

private:
    EjectionAnimationManager() : defaultAnimationDuration(1.2f), particleIntensity(20) {}
    ~EjectionAnimationManager() = default;
    EjectionAnimationManager(const EjectionAnimationManager&) = delete;
    EjectionAnimationManager& operator=(const EjectionAnimationManager&) = delete;

    void removeCompletedAnimations();
    ZappyTypes::Vector3 calculateDirection(const ZappyTypes::Vector3& from, const ZappyTypes::Vector3& to);
    ZappyTypes::Vector3 interpolatePosition(const ZappyTypes::Vector3& start, const ZappyTypes::Vector3& end, float t) const;
    float easeOutQuad(float t) const;

    std::map<int, EjectionAnimation> activeAnimations;
    float defaultAnimationDuration;
    int particleIntensity;
};

}  // namespace Zappy
