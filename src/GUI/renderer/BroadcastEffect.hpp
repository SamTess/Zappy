/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** BroadcastEffect
*/

#pragma once

#include "ParticleEffect.hpp"
#include <memory>

class GameState;

namespace Zappy {

class BroadcastEffect : public ParticleEffect {
    public:
        BroadcastEffect();
        ~BroadcastEffect() override = default;

        void initialize(const ZappyTypes::Vector3& position, ParticleType type, int maxParticles = 30) override;
        void initialize(int playerId, std::shared_ptr<const GameState> gameState, ParticleType type, int maxParticles = 30);
        void update(float deltaTime) override;
        void render(const std::shared_ptr<IGraphicsLib>& graphicsLib) override;
        bool isActive() const override;
        void reset() override;

    private:
        void renderRing(const std::shared_ptr<IGraphicsLib>& graphicsLib, const ZappyTypes::Vector3& center, float radius, float alpha);
        ZappyTypes::Vector3 getCurrentPlayerPosition() const;

        float currentRadius;
        float maxRadius;
        float expansionSpeed;
        float pulseInterval;
        float lastPulseTime;
        int ringCount;
        std::vector<float> ringRadii;
        std::vector<float> ringAlphas;
        int followPlayerId;
        std::shared_ptr<const GameState> gameState;
        bool followPlayer;
};

}  // namespace Zappy
