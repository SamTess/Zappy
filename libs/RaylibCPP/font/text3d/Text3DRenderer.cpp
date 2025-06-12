/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Renderer implementation
*/

#include "Text3DRenderer.hpp"
#include "Text3DCodepoint.hpp"
#include "Text3DHelper.hpp"
#include <iostream>
#include <cmath>

namespace raylibcpp {

void Text3DRenderer::renderBasicText(const ::Font& font, const std::string& text,
                                   Vector3 position, float fontSize, float fontSpacing,
                                   float lineSpacing, bool backface, Color tint)
{
    if (text.empty()) return;
    
    int length = static_cast<int>(text.length());
    float textOffsetY = 0.0f;
    float textOffsetX = 0.0f;
    float scale = fontSize / static_cast<float>(font.baseSize);

    for (int i = 0; i < length;) {
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // Handle malformed UTF-8 sequences
        if (codepoint == 0x3f) codepointByteCount = 1;

        processCharacter(font, codepoint, index, position, textOffsetX, textOffsetY,
                       fontSize, fontSpacing, lineSpacing, scale, backface, tint);

        i += codepointByteCount;
    }
}

void Text3DRenderer::processCharacter(const ::Font& font, int codepoint, int index,
                                    Vector3& position, float& textOffsetX, float& textOffsetY,
                                    float fontSize, float fontSpacing, float lineSpacing,
                                    float scale, bool backface, Color tint)
{
    if (codepoint == '\n') {
        handleNewline(textOffsetX, textOffsetY, fontSize, lineSpacing);
        return;
    }

    // Draw visible characters (skip spaces and tabs)
    if (codepoint != ' ' && codepoint != '\t') {
        Vector3 charPosition = {
            position.x + textOffsetX,
            position.y,
            position.z + textOffsetY
        };
        Text3DCodepoint::draw(font, codepoint, charPosition, fontSize, backface, tint);
    }

    // Advance to next character position
    textOffsetX += getCharacterAdvance(font, index, scale, fontSpacing);
}

void Text3DRenderer::handleNewline(float& textOffsetX, float& textOffsetY,
                                  float fontSize, float lineSpacing)
{
    textOffsetY += fontSize + lineSpacing;
    textOffsetX = 0.0f;
}

float Text3DRenderer::getCharacterAdvance(const ::Font& font, int index, float scale, float fontSpacing)
{
    if (font.glyphs[index].advanceX == 0) {
        return static_cast<float>(font.recs[index].width) * scale + fontSpacing;
    }
    return static_cast<float>(font.glyphs[index].advanceX) * scale + fontSpacing;
}

}  // namespace raylibcpp
