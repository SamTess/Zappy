/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ParticleSystem
*/

#include <random>
#include <algorithm>
#include <cmath>
#include <utility>
#include <memory>
#include <string>
#include <iostream>
#include "ParticleSystem.hpp"
#include "BroadcastEffect.hpp"
#include "../gameController/GameState.hpp"

namespace Zappy {

ParticleSystem& ParticleSystem::getInstance() {
    static ParticleSystem instance;
    return instance;
}

void ParticleSystem::update(float deltaTime) {
    for (auto& effect : activeEffects) {
        if (effect)
            effect->update(deltaTime);
    }
    removeInactiveEffects();
}

void ParticleSystem::render(const std::shared_ptr<IGraphicsLib>& graphicsLib) {
    if (!graphicsLib)
        return;
    for (const auto& effect : activeEffects) {
        if (effect && effect->isActive())
            effect->render(graphicsLib);
    }
}

void ParticleSystem::createEffect(ParticleType type, const ZappyTypes::Vector3& position, int intensity) {
    if (activeEffects.size() >= maxActiveEffects) {
        removeInactiveEffects();
        if (activeEffects.size() >= maxActiveEffects)
            return;
    }
    std::unique_ptr<ParticleEffect> effect;
    if (type == ParticleType::BROADCAST_RING)
        effect = std::make_unique<BroadcastEffect>();
    effect->initialize(position, type, intensity);
    activeEffects.push_back(std::move(effect));
}

void ParticleSystem::createPlayerBroadcastEffect(int playerId, std::shared_ptr<const GameState> gameState) {
    if (activeEffects.size() >= maxActiveEffects) {
        removeInactiveEffects();
        if (activeEffects.size() >= maxActiveEffects)
            return;
    }
    auto effect = std::make_unique<BroadcastEffect>();
    effect->initialize(playerId, gameState, ParticleType::BROADCAST_RING, 6);
    activeEffects.push_back(std::move(effect));
}

void ParticleSystem::cleanup() {
    activeEffects.clear();
}

void ParticleSystem::removeInactiveEffects() {
    activeEffects.erase(
        std::remove_if(activeEffects.begin(), activeEffects.end(),
            [](const std::unique_ptr<ParticleEffect>& effect) {
                return !effect || !effect->isActive();
        }),
        activeEffects.end()
    );
}

}  // namespace Zappy
