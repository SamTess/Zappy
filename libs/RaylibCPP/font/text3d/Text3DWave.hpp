/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Wave effects
*/

#pragma once
#include <raylib.h>
#include <memory>
#include "Text3DCore.hpp"

namespace raylibcpp {

class Text3DWave {
public:
    static Vector3 applyWaveEffect(Vector3 basePosition, const WaveTextConfigPtr& config,
        float time, int charIndex);
    static WaveTextConfigPtr createDefaultConfig();
    static bool isValidConfig(const WaveTextConfigPtr& config);

private:
    static float calculateWaveOffset(float speed, float offset, float range,
        float time, int charIndex);
};

}  // namespace raylibcpp
