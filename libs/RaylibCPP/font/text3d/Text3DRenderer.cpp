/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Renderer implementation
*/

#include "Text3DRenderer.hpp"

#include <cmath>
#include <iostream>
#include <string>
#include <memory>
#include "Text3DCodepoint.hpp"
#include "Text3DHelper.hpp"

namespace raylibcpp {

void Text3DRenderer::renderBasicText(const ::Font& font, const std::string& text,
    Vector3 position, float fontSize, float fontSpacing,
    float lineSpacing, bool backface, Color tint) {
    if (text.empty())
        return;
    int length = static_cast<int>(text.length());
    auto renderState = std::make_shared<RenderState>();
    renderState->position = position;
    renderState->textOffsetY = 0.0f;
    renderState->textOffsetX = 0.0f;
    renderState->charIndex = 0;
    float scale = fontSize / static_cast<float>(font.baseSize);

    for (int i = 0; i < length;) {
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);
        if (codepoint == 0x3f) codepointByteCount = 1;
        processCharacter(font, codepoint, index, renderState,
                       fontSize, fontSpacing, lineSpacing, scale, backface, tint);
        i += codepointByteCount;
    }
}

void Text3DRenderer::processCharacter(const ::Font& font, int codepoint, int index,
    const RenderStatePtr& renderState,
    float fontSize, float fontSpacing, float lineSpacing,
    float scale, bool backface, Color tint) {
    if (codepoint == '\n') {
        handleNewline(renderState, fontSize, lineSpacing);
        return;
    }
    if (codepoint != ' ' && codepoint != '\t') {
        Vector3 charPosition = {
            renderState->position.x + renderState->textOffsetX,
            renderState->position.y,
            renderState->position.z + renderState->textOffsetY
        };
        Text3DCodepoint::draw(font, codepoint, charPosition, fontSize, backface, tint);
    }
    renderState->textOffsetX += getCharacterAdvance(font, index, scale, fontSpacing);
}

void Text3DRenderer::handleNewline(const RenderStatePtr& renderState,
    float fontSize, float lineSpacing) {
    renderState->textOffsetY += fontSize + lineSpacing;
    renderState->textOffsetX = 0.0f;
}

float Text3DRenderer::getCharacterAdvance(const ::Font& font, int index, float scale, float fontSpacing) {
    if (font.glyphs[index].advanceX == 0)
        return static_cast<float>(font.recs[index].width) * scale + fontSpacing;
    return static_cast<float>(font.glyphs[index].advanceX) * scale + fontSpacing;
}

}  // namespace raylibcpp
