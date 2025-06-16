/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Measurement utilities
*/

#pragma once
#include <raylib.h>
#include <string>

namespace raylibcpp {

class Text3DMeasurement {
public:
    static Vector3 measureText(const ::Font& font, const std::string& text,
        float fontSize, float fontSpacing, float lineSpacing);

private:
    static bool isWaveMarker(const std::string& text, int pos);
};

}  // namespace raylibcpp
