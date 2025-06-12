/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Text3D encapsulation
*/

#pragma once
#include <raylib.h>
#include <string>
#include "../../src/Shared/Common.hpp"

namespace raylibcpp {

// Configuration structure for waving the text
struct WaveTextConfig {
    Vector3 waveRange;
    Vector3 waveSpeed;
    Vector3 waveOffset;
};

class Text3D {
    public:
        Text3D();
        ~Text3D();

        // Draw a codepoint in 3D space
        static void DrawTextCodepoint3D(::Font font, int codepoint, Vector3 position, 
                                      float fontSize, bool backface, Color tint);
        
        // Draw a 2D text in 3D space
        static void DrawText3D(::Font font, const std::string& text, Vector3 position, 
                             float fontSize, float fontSpacing, float lineSpacing, 
                             bool backface, Color tint);

        // Draw a 2D text in 3D space and wave the parts that start with `~~` and end with `~~`
        static void DrawTextWave3D(::Font font, const std::string& text, Vector3 position,
                                 float fontSize, float fontSpacing, float lineSpacing,
                                 bool backface, WaveTextConfig* config, float time, Color tint);
        
        // Measure a text in 3D ignoring the `~~` chars
        static Vector3 MeasureTextWave3D(::Font font, const std::string& text, 
                                      float fontSize, float fontSpacing, float lineSpacing);
};

}  // namespace raylibcpp
