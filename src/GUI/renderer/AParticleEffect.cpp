/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** AParticleEffect
*/

#include "AParticleEffect.hpp"

namespace Zappy {

AParticleEffect::AParticleEffect() : effectDuration(2.0f), timeAlive(0.0f), active(false), type(ParticleType::EJECTION_BURST) {
}

}  // namespace Zappy
