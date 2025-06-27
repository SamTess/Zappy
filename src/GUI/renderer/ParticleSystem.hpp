/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ParticleSystem
*/

#pragma once

#include <vector>
#include <memory>
#include "AParticleEffect.hpp"
#include "BroadcastEffect.hpp"

class GameState;

namespace Zappy {

class ParticleSystem {
    public:
        static ParticleSystem& getInstance();

        void update(float deltaTime);
        void render(const std::shared_ptr<IGraphicsLib>& graphicsLib);
        void createPlayerBroadcastEffect(int playerId, std::shared_ptr<const GameState> gameState);
        void createEffect(ParticleType type, const ZappyTypes::Vector3& position, int intensity = 20);
        void cleanup();
        void setMaxEffects(size_t max) { maxActiveEffects = max; }

    private:
        ParticleSystem() : maxActiveEffects(50) {}
        ~ParticleSystem() = default;
        void removeInactiveEffects();

        std::vector<std::unique_ptr<AParticleEffect>> activeEffects;
        size_t maxActiveEffects;
};

}  // namespace Zappy
