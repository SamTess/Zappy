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
    // Measure text dimensions ignoring wave markers
    static Vector3 measureText(const ::Font& font, const std::string& text,
                             float fontSize, float fontSpacing, float lineSpacing);

private:
    // Calculate width for a single line
    static float calculateLineWidth(const ::Font& font, const std::string& text,
                                  int start, int end, float scale);
    
    // Count visible characters in a line (excluding wave markers)
    static int countVisibleCharacters(const std::string& text, int start, int end);
    
    // Check if position is at wave marker
    static bool isWaveMarker(const std::string& text, int pos);
};

}  // namespace raylibcpp
