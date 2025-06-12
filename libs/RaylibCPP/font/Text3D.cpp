/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Text3D implementation - Refactored for better readability
*/

#include "Text3D.hpp"

#include <iostream>
#include <string>

#include "text3d/Text3DCodepoint.hpp"
#include "text3d/Text3DRenderer.hpp"
#include "text3d/Text3DParser.hpp"
#include "text3d/Text3DMeasurement.hpp"

namespace raylibcpp {

Text3D::Text3D() {}

Text3D::~Text3D() {}

void Text3D::DrawTextCodepoint3D(::Font font, int codepoint, Vector3 position,
    float fontSize, bool backface, Color tint) {
    Text3DCodepoint::draw(font, codepoint, position, fontSize, backface, tint);
}

void Text3D::DrawText3D(::Font font, const std::string& text, Vector3 position,
    float fontSize, float fontSpacing, float lineSpacing,
    bool backface, Color tint) {
    std::cout << "DrawText3D: " << text << std::endl;
    Text3DRenderer::renderBasicText(font, text, position, fontSize,
                                   fontSpacing, lineSpacing, backface, tint);
}

void Text3D::DrawTextWave3D(::Font font, const std::string& text, Vector3 position,
    float fontSize, float fontSpacing, float lineSpacing,
    bool backface, const WaveTextConfigPtr& config, float time, Color tint) {
    std::cout << "DrawTextWave3D (smart pointer): " << text << std::endl;
    Text3DParser::renderWaveText(font, text, position, fontSize, fontSpacing,
                                lineSpacing, backface, config, time, tint);
}

Vector3 Text3D::MeasureTextWave3D(::Font font, const std::string& text,
    float fontSize, float fontSpacing, float lineSpacing) {
    std::cout << "MeasureTextWave3D: " << text << std::endl;
    return Text3DMeasurement::measureText(font, text, fontSize, fontSpacing, lineSpacing);
}

}  // namespace raylibcpp
