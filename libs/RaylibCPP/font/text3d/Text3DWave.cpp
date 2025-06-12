/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Wave effects implementation
*/

#include <math.h>
#include <memory>
#include "Text3DWave.hpp"

namespace raylibcpp {

Vector3 Text3DWave::applyWaveEffect(Vector3 basePosition, const WaveTextConfigPtr& config,
                                   float time, int charIndex) {
    if (!config) {
        return basePosition;
    }

    Vector3 wavePosition = basePosition;

    wavePosition.x += calculateWaveOffset(config->waveSpeed.x, config->waveOffset.x,
                        config->waveRange.x, time, charIndex);
    wavePosition.y += calculateWaveOffset(config->waveSpeed.y, config->waveOffset.y,
                        config->waveRange.y, time, charIndex);
    wavePosition.z += calculateWaveOffset(config->waveSpeed.z, config->waveOffset.z,
                        config->waveRange.z, time, charIndex);
    return wavePosition;
}

WaveTextConfigPtr Text3DWave::createDefaultConfig() {
    return std::make_shared<WaveTextConfig>(WaveTextConfig{
        .waveRange = { 0.45f, 0.45f, 0.45f },
        .waveSpeed = { 3.0f, 3.0f, 0.5f },
        .waveOffset = { 0.35f, 0.35f, 0.35f }
    });
}

bool Text3DWave::isValidConfig(const WaveTextConfigPtr& config) {
    return config != nullptr;
}

float Text3DWave::calculateWaveOffset(float speed, float offset, float range,
    float time, int charIndex) {
    return sinf(time * speed - charIndex * offset) * range;
}

}  // namespace raylibcpp
