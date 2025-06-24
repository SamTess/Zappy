/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ParticleSystem - System for handling particle effects and animations
*/

#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include <string>
#include "../../Shared/IGraphicsLib.hpp"
#include "../../Shared/Common.hpp"

namespace Zappy {

struct Particle {
    ZappyTypes::Vector3 position;
    ZappyTypes::Vector3 velocity;
    ZappyTypes::Vector3 acceleration;
    ZappyTypes::Color color;
    float life;
    float maxLife;
    float size;
    bool active;

    Particle() : position{0, 0, 0}, velocity{0, 0, 0}, acceleration{0, 0, 0},
                 color{255, 255, 255, 255}, life(0), maxLife(1.0f), size(1.0f), active(false) {}
};

enum class ParticleType {
    EJECTION_BURST,
    EJECTION_TRAIL,
    SPARK,
    DUST_CLOUD,
    BROADCAST_RING,
    BROADCAST_PULSE,
    COMMUNICATION_WAVE
};

class ParticleEffect {
public:
    ParticleEffect();
    virtual ~ParticleEffect() = default;

    virtual void initialize(const ZappyTypes::Vector3& position, ParticleType type, int maxParticles = 20) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(const std::shared_ptr<IGraphicsLib>& graphicsLib) = 0;
    virtual bool isActive() const = 0;
    virtual void reset() = 0;

protected:
    std::vector<Particle> particles;
    ZappyTypes::Vector3 origin;
    float effectDuration;
    float timeAlive;
    bool active;
    ParticleType type;
};

class EjectionEffect : public ParticleEffect {
public:
    EjectionEffect();
    ~EjectionEffect() override = default;

    void initialize(const ZappyTypes::Vector3& position, ParticleType type, int maxParticles = 20) override;
    void update(float deltaTime) override;
    void render(const std::shared_ptr<IGraphicsLib>& graphicsLib) override;
    bool isActive() const override;
    void reset() override;

private:
    void createBurstParticles(const ZappyTypes::Vector3& position, int count);
    void createTrailParticles(const ZappyTypes::Vector3& position, int count);
    void updateParticle(Particle* particle, float deltaTime);
    void renderParticle(const Particle& particle, const std::shared_ptr<IGraphicsLib>& graphicsLib);
    ZappyTypes::Color interpolateColor(const ZappyTypes::Color& start, const ZappyTypes::Color& end, float t);

    float burstIntensity;
    float gravity;
    float airResistance;
};

class BroadcastEffect : public ParticleEffect {
public:
    BroadcastEffect();
    ~BroadcastEffect() override = default;

    void initialize(const ZappyTypes::Vector3& position, ParticleType type, int maxParticles = 30) override;
    void update(float deltaTime) override;
    void render(const std::shared_ptr<IGraphicsLib>& graphicsLib) override;
    bool isActive() const override;
    void reset() override;

private:
    void renderRing(const std::shared_ptr<IGraphicsLib>& graphicsLib, const ZappyTypes::Vector3& center, float radius, float alpha);

    float currentRadius;
    float maxRadius;
    float expansionSpeed;
    float pulseInterval;
    float lastPulseTime;
    int ringCount;
    std::vector<float> ringRadii;
    std::vector<float> ringAlphas;
};

class ParticleSystem {
public:
    static ParticleSystem& getInstance();

    void update(float deltaTime);
    void render(const std::shared_ptr<IGraphicsLib>& graphicsLib);

    void createPlayerEjectionEffect(const ZappyTypes::Vector3& position, const ZappyTypes::Vector3& direction);

    void createPlayerBroadcastEffect(const ZappyTypes::Vector3& position, const std::string& teamName);

    void createEffect(ParticleType type, const ZappyTypes::Vector3& position, int intensity = 20);

    void createBroadcastEffect(ParticleType type, const ZappyTypes::Vector3& position, int intensity = 20);

    void cleanup();
    void setMaxEffects(size_t max) { maxActiveEffects = max; }

private:
    ParticleSystem() : maxActiveEffects(50) {}
    ~ParticleSystem() = default;
    ParticleSystem(const ParticleSystem&) = delete;
    ParticleSystem& operator=(const ParticleSystem&) = delete;

    void removeInactiveEffects();

    std::vector<std::unique_ptr<ParticleEffect>> activeEffects;
    size_t maxActiveEffects;
};

}  // namespace Zappy
