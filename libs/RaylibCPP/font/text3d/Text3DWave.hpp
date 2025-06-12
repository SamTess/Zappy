/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Wave effects
*/

#pragma once
#include <raylib.h>
#include "Text3DCore.hpp"

namespace raylibcpp {

class Text3DWave {
public:
    static Vector3 applyWaveEffect(Vector3 basePosition, const WaveTextConfig& config,
        float time, int charIndex);
    static WaveTextConfig createDefaultConfig();
    static bool isValidConfig(const WaveTextConfig* config);

private:
    static float calculateWaveOffset(float speed, float offset, float range,
        float time, int charIndex);
};

}  // namespace raylibcpp
