/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ParticleSystem
*/

#include "ParticleSystem.hpp"
#include <random>
#include <algorithm>
#include <cmath>
#include <utility>
#include <memory>
#include <string>
#include <iostream>
#include "../gameController/GameState.hpp"
#include "EjectionAnimationManager.hpp"

namespace Zappy {

ParticleEffect::ParticleEffect() : effectDuration(2.0f), timeAlive(0.0f), active(false), type(ParticleType::EJECTION_BURST) {
}

EjectionEffect::EjectionEffect() : burstIntensity(1.5f), gravity(-9.8f), airResistance(0.98f) {
    effectDuration = 3.0f;
}

void EjectionEffect::initialize(const ZappyTypes::Vector3& position, ParticleType particleType, int maxParticles) {
    origin = position;
    type = particleType;
    timeAlive = 0.0f;
    active = true;
    particles.clear();
    particles.reserve(maxParticles);

    if (type == ParticleType::EJECTION_BURST) {
        createBurstParticles(position, maxParticles);
    } else if (type == ParticleType::EJECTION_TRAIL) {
        createTrailParticles(position, maxParticles);
    }
}

void EjectionEffect::createBurstParticles(const ZappyTypes::Vector3& position, int count) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> speedDist(1.0f, 4.0f);
    std::uniform_real_distribution<float> heightDist(2.0f, 6.0f);
    std::uniform_real_distribution<float> lifeDist(1.5f, 3.0f);
    std::uniform_real_distribution<float> sizeDist(0.1f, 0.3f);

    for (int i = 0; i < count; ++i) {
        Particle particle;
        particle.position = position;
        particle.position.y += 0.5f;

        float angle = angleDist(gen);
        float speed = speedDist(gen) * burstIntensity;
        float height = heightDist(gen);

        particle.velocity.x = std::cos(angle) * speed;
        particle.velocity.z = std::sin(angle) * speed;
        particle.velocity.y = height;

        particle.acceleration.x = 0.0f;
        particle.acceleration.y = gravity;
        particle.acceleration.z = 0.0f;

        particle.maxLife = lifeDist(gen);
        particle.life = particle.maxLife;
        particle.size = sizeDist(gen);

        if (i % 3 == 0) {
            particle.color = {255, 140, 0, 255};  // Orange
        } else if (i % 3 == 1) {
            particle.color = {255, 69, 0, 255};   // Red-orange
        } else {
            particle.color = {255, 215, 0, 255};  // Gold
        }

        particle.active = true;
        particles.push_back(particle);
    }
}

void EjectionEffect::createTrailParticles(const ZappyTypes::Vector3& position, int count) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> offsetDist(-0.3f, 0.3f);
    std::uniform_real_distribution<float> lifeDist(0.8f, 1.5f);
    std::uniform_real_distribution<float> sizeDist(0.05f, 0.15f);

    for (int i = 0; i < count; ++i) {
        Particle particle;
        particle.position = position;
        particle.position.x += offsetDist(gen);
        particle.position.z += offsetDist(gen);
        particle.position.y += offsetDist(gen) + 0.2f;

        particle.velocity.x = offsetDist(gen) * 0.5f;
        particle.velocity.y = offsetDist(gen) * 0.5f + 1.0f;
        particle.velocity.z = offsetDist(gen) * 0.5f;

        particle.acceleration.x = 0.0f;
        particle.acceleration.y = gravity * 0.3f;
        particle.acceleration.z = 0.0f;

        particle.maxLife = lifeDist(gen);
        particle.life = particle.maxLife;
        particle.size = sizeDist(gen);

        int grayValue = 150 + (i % 50);
        particle.color = {static_cast<unsigned char>(grayValue),
                         static_cast<unsigned char>(grayValue),
                         static_cast<unsigned char>(grayValue), 200};

        particle.active = true;
        particles.push_back(particle);
    }
}

void EjectionEffect::update(float deltaTime) {
    if (!active) return;

    timeAlive += deltaTime;

    for (auto& particle : particles) {
        if (particle.active)
            updateParticle(&particle, deltaTime);
    }

    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
                      [](const Particle& p) { return !p.active; }),
        particles.end()
    );

    if (timeAlive >= effectDuration || particles.empty())
        active = false;
}

void EjectionEffect::updateParticle(Particle* particle, float deltaTime) {
    if (!particle || !particle->active) return;

    particle->life -= deltaTime;
    if (particle->life <= 0.0f) {
        particle->active = false;
        return;
    }

    particle->velocity.x += particle->acceleration.x * deltaTime;
    particle->velocity.y += particle->acceleration.y * deltaTime;
    particle->velocity.z += particle->acceleration.z * deltaTime;

    particle->velocity.x *= airResistance;
    particle->velocity.z *= airResistance;

    particle->position.x += particle->velocity.x * deltaTime;
    particle->position.y += particle->velocity.y * deltaTime;
    particle->position.z += particle->velocity.z * deltaTime;

    float lifeRatio = particle->life / particle->maxLife;
    particle->color.a = static_cast<unsigned char>(255 * lifeRatio);

    if (particle->position.y <= 0.0f) {
        particle->position.y = 0.0f;
        particle->velocity.y *= -0.3f;
        particle->velocity.x *= 0.7f;
        particle->velocity.z *= 0.7f;
    }
}

void EjectionEffect::render(const std::shared_ptr<IGraphicsLib>& graphicsLib) {
    if (!active || !graphicsLib) return;

    for (const auto& particle : particles) {
        if (particle.active)
            renderParticle(particle, graphicsLib);
    }
}

void EjectionEffect::renderParticle(const Particle& particle, const std::shared_ptr<IGraphicsLib>& graphicsLib) {
    graphicsLib->DrawSphere(particle.position, particle.size, particle.color);

    if (type == ParticleType::EJECTION_BURST && particle.color.a > 100) {
        ZappyTypes::Color glowColor = particle.color;
        glowColor.a = static_cast<unsigned char>(glowColor.a * 0.3f);
        graphicsLib->DrawSphere(particle.position, particle.size * 1.5f, glowColor);
    }
}

ZappyTypes::Color EjectionEffect::interpolateColor(const ZappyTypes::Color& start, const ZappyTypes::Color& end, float t) {
    t = std::max(0.0f, std::min(1.0f, t));
    return {
        static_cast<unsigned char>(start.r + (end.r - start.r) * t),
        static_cast<unsigned char>(start.g + (end.g - start.g) * t),
        static_cast<unsigned char>(start.b + (end.b - start.b) * t),
        static_cast<unsigned char>(start.a + (end.a - start.a) * t)
    };
}

bool EjectionEffect::isActive() const {
    return active;
}

void EjectionEffect::reset() {
    active = false;
    timeAlive = 0.0f;
    particles.clear();
}

BroadcastEffect::BroadcastEffect() : currentRadius(0.0f), maxRadius(12.0f), expansionSpeed(4.0f),
                                   pulseInterval(0.5f), lastPulseTime(0.0f), ringCount(2),
                                   followPlayerId(-1), followPlayer(false) {
    effectDuration = 2.5f;
    ringRadii.resize(ringCount, 0.0f);
    ringAlphas.resize(ringCount, 1.0f);
}

void BroadcastEffect::initialize(const ZappyTypes::Vector3& position, ParticleType particleType, int maxParticles) {
    origin = position;
    type = particleType;
    timeAlive = 0.0f;
    active = true;
    currentRadius = 0.0f;
    lastPulseTime = 0.0f;
    followPlayer = false;
    followPlayerId = -1;
    gameState = nullptr;

    for (int i = 0; i < ringCount; ++i) {
        ringRadii[i] = 0.0f;
        ringAlphas[i] = 1.0f;
    }

    particles.clear();
    particles.reserve(maxParticles);
}

void BroadcastEffect::initialize(int playerId, std::shared_ptr<const GameState> gameStatePtr, ParticleType particleType, int maxParticles) {
    followPlayerId = playerId;
    gameState = gameStatePtr;
    followPlayer = true;
    type = particleType;
    timeAlive = 0.0f;
    active = true;
    currentRadius = 0.0f;
    lastPulseTime = 0.0f;

    origin = getCurrentPlayerPosition();

    for (int i = 0; i < ringCount; ++i) {
        ringRadii[i] = 0.0f;
        ringAlphas[i] = 1.0f;
    }

    particles.clear();
    particles.reserve(maxParticles);
}

void BroadcastEffect::update(float deltaTime) {
    if (!active) return;

    timeAlive += deltaTime;
    if (timeAlive >= effectDuration) {
        active = false;
        return;
    }

    for (int i = 0; i < ringCount; ++i) {
        float startTime = i * pulseInterval;
        if (timeAlive >= startTime) {
            ringRadii[i] += expansionSpeed * deltaTime;
            float ringAge = timeAlive - startTime;
            ringAlphas[i] = std::max(0.0f, 1.0f - (ringAge / (effectDuration - startTime)));
        }
    }
}

void BroadcastEffect::render(const std::shared_ptr<IGraphicsLib>& graphicsLib) {
    if (!active || !graphicsLib) return;

    ZappyTypes::Vector3 currentPosition = followPlayer ? getCurrentPlayerPosition() : origin;

    for (int i = 0; i < ringCount; ++i) {
        if (ringRadii[i] > 0.0f && ringAlphas[i] > 0.0f)
            renderRing(graphicsLib, currentPosition, ringRadii[i], ringAlphas[i]);
    }
}

bool BroadcastEffect::isActive() const {
    return active;
}

void BroadcastEffect::reset() {
    active = false;
    timeAlive = 0.0f;
    currentRadius = 0.0f;
    particles.clear();
}

void BroadcastEffect::createRingParticles(const ZappyTypes::Vector3& position, float radius, int count) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> lifeDist(2.0f, 4.0f);
    std::uniform_real_distribution<float> sizeDist(0.05f, 0.15f);

    for (int i = 0; i < count; ++i) {
        Particle particle;
        float angle = angleDist(gen);

        particle.position = position;
        particle.position.x += static_cast<float>(cos(angle)) * radius;
        particle.position.z += static_cast<float>(sin(angle)) * radius;
        particle.position.y += 0.2f;

        particle.velocity.x = static_cast<float>(cos(angle)) * 2.0f;
        particle.velocity.z = static_cast<float>(sin(angle)) * 2.0f;
        particle.velocity.y = 0.5f;

        particle.acceleration = {0.0f, -1.0f, 0.0f};
        particle.color = {100, 255, 255, 255};
        particle.life = lifeDist(gen);
        particle.maxLife = particle.life;
        particle.size = sizeDist(gen) * 3.0f;
        particle.active = true;

        particles.push_back(particle);
    }
}

void BroadcastEffect::createPulseParticles(const ZappyTypes::Vector3& position, int count) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * M_PI);
    std::uniform_real_distribution<float> speedDist(1.5f, 3.5f);
    std::uniform_real_distribution<float> lifeDist(1.0f, 2.5f);
    std::uniform_real_distribution<float> sizeDist(0.08f, 0.2f);

    for (int i = 0; i < count; ++i) {
        Particle particle;
        float angle = angleDist(gen);
        float speed = speedDist(gen);

        particle.position = position;
        particle.position.y += 1.0f;

        particle.velocity.x = static_cast<float>(cos(angle)) * speed;
        particle.velocity.z = static_cast<float>(sin(angle)) * speed;
        particle.velocity.y = 2.0f;

        particle.acceleration = {0.0f, -2.0f, 0.0f};
        particle.color = {255, 255, 0, 255};
        particle.life = lifeDist(gen);
        particle.maxLife = particle.life;
        particle.size = sizeDist(gen) * 4.0f;
        particle.active = true;

        particles.push_back(particle);
    }
}

void BroadcastEffect::updateRingParticle(Particle* particle, float deltaTime) {
    if (!particle || !particle->active) return;

    particle->position.x += particle->velocity.x * deltaTime;
    particle->position.y += particle->velocity.y * deltaTime;
    particle->position.z += particle->velocity.z * deltaTime;

    particle->velocity.x += particle->acceleration.x * deltaTime;
    particle->velocity.y += particle->acceleration.y * deltaTime;
    particle->velocity.z += particle->acceleration.z * deltaTime;

    particle->velocity.x *= 0.95f;
    particle->velocity.z *= 0.95f;

    particle->life -= deltaTime;
    if (particle->life <= 0.0f) {
        particle->active = false;
        return;
    }

    float lifeRatio = particle->life / particle->maxLife;
    particle->color.a = static_cast<unsigned char>(255 * lifeRatio);
}

void BroadcastEffect::updatePulseParticle(Particle* particle, float deltaTime) {
    if (!particle || !particle->active) return;

    particle->position.x += particle->velocity.x * deltaTime;
    particle->position.y += particle->velocity.y * deltaTime;
    particle->position.z += particle->velocity.z * deltaTime;

    particle->velocity.x += particle->acceleration.x * deltaTime;
    particle->velocity.y += particle->acceleration.y * deltaTime;
    particle->velocity.z += particle->acceleration.z * deltaTime;

    particle->life -= deltaTime;
    if (particle->life <= 0.0f) {
        particle->active = false;
        return;
    }

    float pulseFreq = 8.0f;
    float pulse = (sin(timeAlive * pulseFreq) + 1.0f) * 0.5f;
    float lifeRatio = particle->life / particle->maxLife;

    particle->size = 0.1f + pulse * 0.1f;
    particle->color.a = static_cast<unsigned char>(255 * lifeRatio * (0.7f + pulse * 0.3f));
}

void BroadcastEffect::renderRing(const std::shared_ptr<IGraphicsLib>& graphicsLib,
    const ZappyTypes::Vector3& center, float radius, float alpha) {
    if (radius <= 0.0f || alpha <= 0.0f) return;

    int segments = 32;
    float angleStep = 2.0f * M_PI / segments;

    ZappyTypes::Color ringColor = {255, 100, 255, static_cast<unsigned char>(alpha * 255)};  // Bright magenta rings

    for (int i = 0; i < segments; ++i) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;

        ZappyTypes::Vector3 point1 = {
            center.x + static_cast<float>(cos(angle1)) * radius,
            center.y + 0.2f,  // Slightly above player level
            center.z + static_cast<float>(sin(angle1)) * radius
        };

        ZappyTypes::Vector3 point2 = {
            center.x + static_cast<float>(cos(angle2)) * radius,
            center.y + 0.2f,  // Slightly above player level
            center.z + static_cast<float>(sin(angle2)) * radius
        };

        graphicsLib->DrawLine3D(point1, point2, ringColor);
    }
}

void BroadcastEffect::renderParticle(const Particle& particle, const std::shared_ptr<IGraphicsLib>& graphicsLib) {
    if (!particle.active) return;

    graphicsLib->DrawSphere(particle.position, particle.size, particle.color);
}

ZappyTypes::Vector3 BroadcastEffect::getCurrentPlayerPosition() const {
    if (!gameState || !followPlayer || followPlayerId == -1)
        return origin;

    auto player = gameState->getPlayerInfo(followPlayerId);
    if (player) {
        // Use the same coordinate transformation as before
        ZappyTypes::Vector3 playerWorldPos = Zappy::EjectionAnimationManager::getInstance().convertTileToWorldPosition(
            player->getX(), player->getY(), gameState->getMapWidth(), gameState->getMapHeight());
        
        // Set the Y position to be at player level (not ground level)
        playerWorldPos.y = 1.0f;  // Player height level
        
        return playerWorldPos;
    }

    return origin;
}

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
    if (!graphicsLib) return;

    for (const auto& effect : activeEffects) {
        if (effect && effect->isActive())
            effect->render(graphicsLib);
    }
}

void ParticleSystem::createPlayerEjectionEffect(const ZappyTypes::Vector3& position, const ZappyTypes::Vector3& direction) {
    createEffect(ParticleType::EJECTION_BURST, position, 15);

    ZappyTypes::Vector3 trailPos = position;
    trailPos.x += direction.x * 0.5f;
    trailPos.z += direction.z * 0.5f;
    createEffect(ParticleType::EJECTION_TRAIL, trailPos, 10);
}

void ParticleSystem::createEffect(ParticleType type, const ZappyTypes::Vector3& position, int intensity) {
    if (activeEffects.size() >= maxActiveEffects) {
        removeInactiveEffects();
        if (activeEffects.size() >= maxActiveEffects)
            return;
    }

    std::unique_ptr<ParticleEffect> effect;

    if (type == ParticleType::EJECTION_BURST || type == ParticleType::EJECTION_TRAIL ||
        type == ParticleType::SPARK || type == ParticleType::DUST_CLOUD) {
        effect = std::make_unique<EjectionEffect>();
    } else if (type == ParticleType::BROADCAST_RING || type == ParticleType::BROADCAST_PULSE ||
               type == ParticleType::COMMUNICATION_WAVE) {
        effect = std::make_unique<BroadcastEffect>();
    } else {
        effect = std::make_unique<EjectionEffect>();
    }

    effect->initialize(position, type, intensity);
    activeEffects.push_back(std::move(effect));
}

void ParticleSystem::createPlayerBroadcastEffect(int playerId, std::shared_ptr<const GameState> gameState, const std::string& teamName) {
    (void)teamName;

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
