/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Parser for handling special markers and text processing
*/

#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include "Text3DCore.hpp"

namespace raylibcpp {

class Text3DParser {
public:
    // Check if position is at wave marker
    static bool isWaveMarker(const std::string& text, int pos);
    
    // Skip wave markers and return new position
    static int skipWaveMarkers(const std::string& text, int pos);
    
    // Render text with wave effects
    static void renderWaveText(const ::Font& font, const std::string& text,
                              Vector3 position, float fontSize, float fontSpacing,
                              float lineSpacing, bool backface, 
                              const WaveTextConfig* config, float time, Color tint);

private:
    // Process a single character with potential wave effect
    static void processWaveCharacter(const ::Font& font, int codepoint, int index,
                                   Vector3& position, float& textOffsetX, float& textOffsetY,
                                   float fontSize, float fontSpacing, float lineSpacing,
                                   float scale, bool backface, Color tint,
                                   bool waveActive, const WaveTextConfig* config,
                                   float time, int charIndex);
    
    // Handle newline character in wave text
    static void handleWaveNewline(float& textOffsetX, float& textOffsetY,
                                float fontSize, float lineSpacing, int& charIndex);
};

}  // namespace raylibcpp
