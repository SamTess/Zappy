/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ParticleEffect
*/

#pragma once

#include <vector>
#include <memory>
#include "../../GUI/shared/IGraphicsLib.hpp"
#include "../../GUI/shared/Common.hpp"

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
    BROADCAST_RING,
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

}  // namespace Zappy
