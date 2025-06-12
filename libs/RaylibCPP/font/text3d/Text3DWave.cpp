/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Wave effects implementation
*/

#include "Text3DWave.hpp"
#include <math.h>

namespace raylibcpp {

Vector3 Text3DWave::applyWaveEffect(Vector3 basePosition, const WaveTextConfig& config, 
                                   float time, int charIndex) {
    Vector3 wavePosition = basePosition;
    
    wavePosition.x += calculateWaveOffset(config.waveSpeed.x, config.waveOffset.x, 
                                         config.waveRange.x, time, charIndex);
    wavePosition.y += calculateWaveOffset(config.waveSpeed.y, config.waveOffset.y, 
                                         config.waveRange.y, time, charIndex);
    wavePosition.z += calculateWaveOffset(config.waveSpeed.z, config.waveOffset.z, 
                                         config.waveRange.z, time, charIndex);
    
    return wavePosition;
}

WaveTextConfig Text3DWave::createDefaultConfig() {
    WaveTextConfig config;
    config.waveSpeed = { 3.0f, 3.0f, 0.5f };
    config.waveOffset = { 0.35f, 0.35f, 0.35f };
    config.waveRange = { 0.45f, 0.45f, 0.45f };
    return config;
}

bool Text3DWave::isValidConfig(const WaveTextConfig* config) {
    return config != nullptr;
}

float Text3DWave::calculateWaveOffset(float speed, float offset, float range, 
                                     float time, int charIndex) {
    return sinf(time * speed - charIndex * offset) * range;
}

}  // namespace raylibcpp
