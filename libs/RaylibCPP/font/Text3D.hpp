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
#include "text3d/Text3DCore.hpp"

namespace raylibcpp {

class Text3D {
    public:
        Text3D();
        ~Text3D();

        static void DrawTextCodepoint3D(::Font font, int codepoint, Vector3 position,
            float fontSize, bool backface, Color tint);

        static void DrawText3D(::Font font, const std::string& text, Vector3 position,
            float fontSize, float fontSpacing, float lineSpacing,
            bool backface, Color tint);

        static void DrawTextWave3D(::Font font, const std::string& text, Vector3 position,
            float fontSize, float fontSpacing, float lineSpacing,
            bool backface, WaveTextConfig* config, float time, Color tint);

        static Vector3 MeasureTextWave3D(::Font font, const std::string& text,
            float fontSize, float fontSpacing, float lineSpacing);
};

}  // namespace raylibcpp
