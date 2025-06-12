/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Configuration management
*/

#pragma once
#include <raylib.h>
#include <memory>
#include "Text3DCore.hpp"

namespace raylibcpp {

class Text3DConfig {
public:
    static WaveTextConfigPtr createDefaultWaveConfig();
    static WaveTextConfigPtr createWaveConfig(Vector3 range, Vector3 speed, Vector3 offset);
    static bool isValidWaveConfig(const WaveTextConfigPtr& config);
    static WaveTextConfigPtr createSubtleWave();
    static WaveTextConfigPtr createIntenseWave();
    static WaveTextConfigPtr createSlowWave();
    static WaveTextConfigPtr createFastWave();
    static void adjustWaveIntensity(const WaveTextConfigPtr& config, float multiplier);
    static void adjustWaveSpeed(const WaveTextConfigPtr& config, float multiplier);

private:
    static constexpr float MIN_WAVE_RANGE = 0.0f;
    static constexpr float MAX_WAVE_RANGE = 10.0f;
    static constexpr float MIN_WAVE_SPEED = 0.0f;
    static constexpr float MAX_WAVE_SPEED = 20.0f;
    static constexpr float MIN_WAVE_OFFSET = 0.0f;
    static constexpr float MAX_WAVE_OFFSET = 5.0f;
};

}  // namespace raylibcpp
