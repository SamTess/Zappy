/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** AnimationManager
*/

#include <memory>
#include <string>
#include "AnimationManager.hpp"

AnimationManager::AnimationManager() {}

void AnimationManager::updateAnimations(float deltaTime) {
    Zappy::EjectionAnimationManager::getInstance().update(deltaTime);
    Zappy::ParticleSystem::getInstance().update(deltaTime);
    Zappy::DeathAnimationManager::getInstance().update(deltaTime);
}

void AnimationManager::startDeathAnimation(int playerId, const ZappyTypes::Vector3& position, const std::string& teamName) {
    Zappy::DeathAnimationManager::getInstance().startDeathAnimation(playerId, position, teamName);
}

void AnimationManager::startEjectionAnimation(int playerId, const ZappyTypes::Vector3& from, const ZappyTypes::Vector3& to) {
    Zappy::EjectionAnimationManager::getInstance().startEjectionAnimation(playerId, from, to);
}

void AnimationManager::createBroadcastEffect(int playerId, std::shared_ptr<GameState> gameState) {
    Zappy::ParticleSystem::getInstance().createPlayerBroadcastEffect(playerId, gameState);
}
