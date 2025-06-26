/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** AnimationManager
*/

#ifndef ANIMATION_MANAGER_HPP_
#define ANIMATION_MANAGER_HPP_

#include <memory>
#include <string>
#include "../../renderer/EjectionAnimationManager.hpp"
#include "../../renderer/DeathAnimationManager.hpp"
#include "../../renderer/ParticleSystem.hpp"
#include "../GameState.hpp"

class AnimationManager {
public:
    AnimationManager();
    ~AnimationManager() = default;

    void updateAnimations(float deltaTime);
    void startDeathAnimation(int playerId, const ZappyTypes::Vector3& position, const std::string& teamName);
    void startEjectionAnimation(int playerId, const ZappyTypes::Vector3& from, const ZappyTypes::Vector3& to);
    void createBroadcastEffect(int playerId, std::shared_ptr<GameState> gameState);
};

#endif /* !ANIMATION_MANAGER_HPP_ */
