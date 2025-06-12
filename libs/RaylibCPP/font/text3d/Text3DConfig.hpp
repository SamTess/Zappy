/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Configuration management
*/

#pragma once
#include <raylib.h>
#include "Text3DCore.hpp"

namespace raylibcpp {

class Text3DConfig {
public:
    // Create default wave configuration
    static WaveTextConfig createDefaultWaveConfig();
    
    // Create custom wave configuration
    static WaveTextConfig createWaveConfig(Vector3 range, Vector3 speed, Vector3 offset);
    
    // Validate wave configuration
    static bool isValidWaveConfig(const WaveTextConfig* config);
    
    // Create preset configurations
    static WaveTextConfig createSubtleWave();
    static WaveTextConfig createIntenseWave();
    static WaveTextConfig createSlowWave();
    static WaveTextConfig createFastWave();
    
    // Adjust existing configuration
    static void adjustWaveIntensity(WaveTextConfig& config, float multiplier);
    static void adjustWaveSpeed(WaveTextConfig& config, float multiplier);

private:
    // Validation limits
    static constexpr float MIN_WAVE_RANGE = 0.0f;
    static constexpr float MAX_WAVE_RANGE = 10.0f;
    static constexpr float MIN_WAVE_SPEED = 0.0f;
    static constexpr float MAX_WAVE_SPEED = 20.0f;
    static constexpr float MIN_WAVE_OFFSET = 0.0f;
    static constexpr float MAX_WAVE_OFFSET = 5.0f;
};

}  // namespace raylibcpp
