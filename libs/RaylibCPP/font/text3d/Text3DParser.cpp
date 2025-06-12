/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Parser implementation
*/

#include "Text3DParser.hpp"

#include <cmath>
#include <iostream>
#include <string>
#include <memory>
#include "Text3DCodepoint.hpp"
#include "Text3DWave.hpp"

namespace raylibcpp {

bool Text3DParser::isWaveMarker(const std::string& text, int pos) {
    return (pos < static_cast<int>(text.length()) - 1 &&
            text[pos] == '~' && text[pos + 1] == '~');
}

int Text3DParser::skipWaveMarkers(const std::string& text, int pos) {
    if (isWaveMarker(text, pos)) {
        return pos + 2;
    }
    return pos;
}

void Text3DParser::renderWaveText(const ::Font& font, const std::string& text,
    Vector3 position, float fontSize, float fontSpacing,
    float lineSpacing, bool backface,
    const WaveTextConfigPtr& config, float time, Color tint) {
    if (!Text3DWave::isValidConfig(config)) {
        std::cerr << "Error: WaveTextConfig is null!" << std::endl;
        return;
    }

    int length = static_cast<int>(text.length());
    auto renderState = std::make_shared<RenderState>();
    renderState->position = position;
    renderState->textOffsetY = 0.0f;
    renderState->textOffsetX = 0.0f;
    renderState->charIndex = 0;
    float scale = fontSize / static_cast<float>(font.baseSize);
    bool wave = false;

    for (int i = 0; i < length; ++renderState->charIndex) {
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // Handle malformed UTF-8 sequences
        if (codepoint == 0x3f) codepointByteCount = 1;

        if (codepoint == '\n') {
            handleWaveNewline(renderState, fontSize, lineSpacing);
        } else if (codepoint == '~' && i + 1 < length && text[i + 1] == '~') {
            codepointByteCount += 1;
            wave = !wave;
            i += codepointByteCount;
            continue;
        } else {
            processWaveCharacter(font, codepoint, index, renderState,
                fontSize, fontSpacing, lineSpacing, scale, backface, tint,
                wave, config, time);
        }

        i += codepointByteCount;
    }
}

void Text3DParser::processWaveCharacter(const ::Font& font, int codepoint, int index,
    const RenderStatePtr& renderState,
    float fontSize, float fontSpacing, float lineSpacing,
    float scale, bool backface, Color tint,
    bool waveActive, const WaveTextConfigPtr& config,
    float time) {
    (void)lineSpacing;
    if (codepoint != ' ' && codepoint != '\t') {
        Vector3 charPosition = {
            renderState->position.x + renderState->textOffsetX,
            renderState->position.y,
            renderState->position.z + renderState->textOffsetY
        };

        if (waveActive) {
            charPosition = Text3DWave::applyWaveEffect(charPosition, config, time, renderState->charIndex);
        }

        Text3DCodepoint::draw(font, codepoint, charPosition, fontSize, backface, tint);
    }
    if (font.glyphs[index].advanceX == 0) {
        renderState->textOffsetX += static_cast<float>(font.recs[index].width) * scale + fontSpacing;
    } else {
        renderState->textOffsetX += static_cast<float>(font.glyphs[index].advanceX) * scale + fontSpacing;
    }
}

void Text3DParser::handleWaveNewline(const RenderStatePtr& renderState,
    float fontSize, float lineSpacing) {
    renderState->textOffsetY += fontSize + lineSpacing;
    renderState->textOffsetX = 0.0f;
    renderState->charIndex = 0;
}

}  // namespace raylibcpp
