/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** DeathAnimationManager
*/

#pragma once

#include <memory>
#include <map>
#include <chrono>
#include <string>
#include "../../Shared/IGraphicsLib.hpp"
#include "../../Shared/Common.hpp"

namespace Zappy {

struct DeathAnimation {
    int playerId;
    std::string teamName;
    ZappyTypes::Vector3 startPosition;
    ZappyTypes::Vector3 currentPosition;
    ZappyTypes::Vector3 velocity;
    ZappyTypes::Vector3 rotation;
    ZappyTypes::Vector3 rotationVelocity;
    float scale;
    float timeAlive;
    float duration;
    bool active;
    bool hasHitGround;

    DeathAnimation() : playerId(-1), teamName(""), startPosition{0, 0, 0}, currentPosition{0, 0, 0},
        velocity{0, 0, 0}, rotation{0, 0, 0}, rotationVelocity{0, 0, 0},
        scale(0.4f), timeAlive(0.0f), duration(6.0f), active(false), hasHitGround(false) {}
};

class DeathAnimationManager {
    public:
        static DeathAnimationManager& getInstance();

        void startDeathAnimation(int playerId, const ZappyTypes::Vector3& position);
        void startDeathAnimation(int playerId, const ZappyTypes::Vector3& position, const std::string& teamName);
        void update(float deltaTime);
        void render(const std::shared_ptr<IGraphicsLib>& graphicsLib);
        bool isPlayerInDeathAnimation(int playerId) const;
        void cleanup();
        ZappyTypes::Vector3 getPlayerDeathPosition(int playerId) const;
        ZappyTypes::Vector3 getPlayerDeathRotation(int playerId) const;
        ZappyTypes::Vector3 convertTileToWorldPosition(int tileX, int tileY, int mapWidth, int mapHeight) const;

    private:
        DeathAnimationManager() = default;
        ~DeathAnimationManager() = default;
        void updateDeathAnimation(std::shared_ptr<DeathAnimation> animation, float deltaTime);
        void removeFinishedAnimations();

        const float gravity = -15.0f;
        const float bounceReduction = 0.3f;
        const float groundFriction = 0.9f;
        const float rotationDamping = 0.95f;
        std::map<int, std::shared_ptr<DeathAnimation>> activeAnimations;
};

}  // namespace Zappy
