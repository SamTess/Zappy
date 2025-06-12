/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Text3D implementation - Refactored for better readability
*/

#include "Text3D.hpp"
#include "text3d/Text3DCodepoint.hpp"
#include "text3d/Text3DRenderer.hpp"
#include "text3d/Text3DParser.hpp"
#include "text3d/Text3DMeasurement.hpp"
#include <iostream>

namespace raylibcpp {

Text3D::Text3D() {}

Text3D::~Text3D() {}

// Draw codepoint at specified position in 3D space
void Text3D::DrawTextCodepoint3D(::Font font, int codepoint, Vector3 position, 
                                float fontSize, bool backface, Color tint)
{
    Text3DCodepoint::draw(font, codepoint, position, fontSize, backface, tint);
}

// Draw a 2D text in 3D space
void Text3D::DrawText3D(::Font font, const std::string& text, Vector3 position, 
                       float fontSize, float fontSpacing, float lineSpacing, 
                       bool backface, Color tint)
{
    std::cout << "DrawText3D: " << text << std::endl;
    Text3DRenderer::renderBasicText(font, text, position, fontSize, 
                                   fontSpacing, lineSpacing, backface, tint);
}

// Draw a 2D text in 3D space and wave the parts that start with `~~` and end with `~~`
void Text3D::DrawTextWave3D(::Font font, const std::string& text, Vector3 position,
                           float fontSize, float fontSpacing, float lineSpacing,
                           bool backface, WaveTextConfig* config, float time, Color tint)
{
    std::cout << "DrawTextWave3D: " << text << std::endl;
    Text3DParser::renderWaveText(font, text, position, fontSize, fontSpacing, 
                                lineSpacing, backface, config, time, tint);
}

// Measure a text in 3D ignoring the `~~` chars
Vector3 Text3D::MeasureTextWave3D(::Font font, const std::string& text, 
                                 float fontSize, float fontSpacing, float lineSpacing)
{
    std::cout << "MeasureTextWave3D: " << text << std::endl;
    return Text3DMeasurement::measureText(font, text, fontSize, fontSpacing, lineSpacing);
}

}  // namespace raylibcpp
