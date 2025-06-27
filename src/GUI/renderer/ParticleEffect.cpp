/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ParticleEffect
*/

#include "ParticleEffect.hpp"

namespace Zappy {

ParticleEffect::ParticleEffect() : effectDuration(2.0f), timeAlive(0.0f), active(false), type(ParticleType::EJECTION_BURST) {
}

}  // namespace Zappy
