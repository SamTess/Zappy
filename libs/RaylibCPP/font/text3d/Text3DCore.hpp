/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Core definitions
*/

#pragma once
#include <raylib.h>
#include <string>
#include <memory>

#define LETTER_BOUNDRY_SIZE     0.25f
#define TEXT_MAX_LAYERS         32
#define LETTER_BOUNDRY_COLOR    VIOLET

namespace raylibcpp {

struct WaveTextConfig {
    Vector3 waveRange;
    Vector3 waveSpeed;
    Vector3 waveOffset;
};

using WaveTextConfigPtr = std::shared_ptr<WaveTextConfig>;
using WaveTextConfigUPtr = std::unique_ptr<WaveTextConfig>;

extern bool SHOW_LETTER_BOUNDRY;
extern bool SHOW_TEXT_BOUNDRY;

}  // namespace raylibcpp
