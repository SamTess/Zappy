/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Configuration implementation
*/

#include "Text3DConfig.hpp"
#include <algorithm>
#include <memory>

namespace raylibcpp {

WaveTextConfigPtr Text3DConfig::createDefaultWaveConfig() {
    return std::make_shared<WaveTextConfig>(WaveTextConfig{
        .waveRange = {0.45f, 0.45f, 0.45f},
        .waveSpeed = {3.0f, 3.0f, 0.5f},
        .waveOffset = {0.35f, 0.35f, 0.35f}
    });
}

WaveTextConfigPtr Text3DConfig::createWaveConfig(Vector3 range, Vector3 speed, Vector3 offset) {
    range.x = std::clamp(range.x, MIN_WAVE_RANGE, MAX_WAVE_RANGE);
    range.y = std::clamp(range.y, MIN_WAVE_RANGE, MAX_WAVE_RANGE);
    range.z = std::clamp(range.z, MIN_WAVE_RANGE, MAX_WAVE_RANGE);
    speed.x = std::clamp(speed.x, MIN_WAVE_SPEED, MAX_WAVE_SPEED);
    speed.y = std::clamp(speed.y, MIN_WAVE_SPEED, MAX_WAVE_SPEED);
    speed.z = std::clamp(speed.z, MIN_WAVE_SPEED, MAX_WAVE_SPEED);
    offset.x = std::clamp(offset.x, MIN_WAVE_OFFSET, MAX_WAVE_OFFSET);
    offset.y = std::clamp(offset.y, MIN_WAVE_OFFSET, MAX_WAVE_OFFSET);
    offset.z = std::clamp(offset.z, MIN_WAVE_OFFSET, MAX_WAVE_OFFSET);
    return std::make_shared<WaveTextConfig>(WaveTextConfig{range, speed, offset});
}

bool Text3DConfig::isValidWaveConfig(const WaveTextConfigPtr& config) {
    if (!config)
        return false;
    if (config->waveRange.x < MIN_WAVE_RANGE || config->waveRange.x > MAX_WAVE_RANGE) return false;
    if (config->waveRange.y < MIN_WAVE_RANGE || config->waveRange.y > MAX_WAVE_RANGE) return false;
    if (config->waveRange.z < MIN_WAVE_RANGE || config->waveRange.z > MAX_WAVE_RANGE) return false;
    if (config->waveSpeed.x < MIN_WAVE_SPEED || config->waveSpeed.x > MAX_WAVE_SPEED) return false;
    if (config->waveSpeed.y < MIN_WAVE_SPEED || config->waveSpeed.y > MAX_WAVE_SPEED) return false;
    if (config->waveSpeed.z < MIN_WAVE_SPEED || config->waveSpeed.z > MAX_WAVE_SPEED) return false;
    if (config->waveOffset.x < MIN_WAVE_OFFSET || config->waveOffset.x > MAX_WAVE_OFFSET) return false;
    if (config->waveOffset.y < MIN_WAVE_OFFSET || config->waveOffset.y > MAX_WAVE_OFFSET) return false;
    if (config->waveOffset.z < MIN_WAVE_OFFSET || config->waveOffset.z > MAX_WAVE_OFFSET) return false;
    return true;
}

WaveTextConfigPtr Text3DConfig::createSubtleWave() {
    return std::make_shared<WaveTextConfig>(WaveTextConfig{
        .waveRange = {0.2f, 0.2f, 0.1f},
        .waveSpeed = {1.5f, 1.5f, 0.3f},
        .waveOffset = {0.5f, 0.5f, 0.5f}
    });
}

WaveTextConfigPtr Text3DConfig::createIntenseWave() {
    return std::make_shared<WaveTextConfig>(WaveTextConfig{
        .waveRange = {1.0f, 1.0f, 0.8f},
        .waveSpeed = {5.0f, 5.0f, 1.0f},
        .waveOffset = {0.2f, 0.2f, 0.2f}
    });
}

WaveTextConfigPtr Text3DConfig::createSlowWave() {
    return std::make_shared<WaveTextConfig>(WaveTextConfig{
        .waveRange = {0.6f, 0.6f, 0.4f},
        .waveSpeed = {1.0f, 1.0f, 0.2f},
        .waveOffset = {0.8f, 0.8f, 0.8f}
    });
}

WaveTextConfigPtr Text3DConfig::createFastWave() {
    return std::make_shared<WaveTextConfig>(WaveTextConfig{
        .waveRange = {0.3f, 0.3f, 0.2f},
        .waveSpeed = {8.0f, 8.0f, 1.5f},
        .waveOffset = {0.1f, 0.1f, 0.1f}
    });
}

void Text3DConfig::adjustWaveIntensity(const WaveTextConfigPtr& config, float multiplier) {
    if (!config) return;
    config->waveRange.x = std::clamp(config->waveRange.x * multiplier, MIN_WAVE_RANGE, MAX_WAVE_RANGE);
    config->waveRange.y = std::clamp(config->waveRange.y * multiplier, MIN_WAVE_RANGE, MAX_WAVE_RANGE);
    config->waveRange.z = std::clamp(config->waveRange.z * multiplier, MIN_WAVE_RANGE, MAX_WAVE_RANGE);
}

void Text3DConfig::adjustWaveSpeed(const WaveTextConfigPtr& config, float multiplier) {
    if (!config) return;
    config->waveSpeed.x = std::clamp(config->waveSpeed.x * multiplier, MIN_WAVE_SPEED, MAX_WAVE_SPEED);
    config->waveSpeed.y = std::clamp(config->waveSpeed.y * multiplier, MIN_WAVE_SPEED, MAX_WAVE_SPEED);
    config->waveSpeed.z = std::clamp(config->waveSpeed.z * multiplier, MIN_WAVE_SPEED, MAX_WAVE_SPEED);
}

// Static members for debug options
bool Text3DConfig::_showLetterBoundary = false;
bool Text3DConfig::_showTextBoundary = false;

void Text3DConfig::setShowLetterBoundary(bool show) {
    _showLetterBoundary = show;
}

void Text3DConfig::setShowTextBoundary(bool show) {
    _showTextBoundary = show;
}

bool Text3DConfig::isShowLetterBoundary() {
    return _showLetterBoundary;
}

bool Text3DConfig::isShowTextBoundary() {
    return _showTextBoundary;
}

}  // namespace raylibcpp
