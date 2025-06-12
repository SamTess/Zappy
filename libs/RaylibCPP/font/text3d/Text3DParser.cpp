/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Parser implementation
*/

#include "Text3DParser.hpp"
#include "Text3DCodepoint.hpp"
#include "Text3DWave.hpp"
#include <iostream>
#include <cmath>

namespace raylibcpp {

bool Text3DParser::isWaveMarker(const std::string& text, int pos)
{
    return (pos < static_cast<int>(text.length()) - 1 && 
            text[pos] == '~' && text[pos + 1] == '~');
}

int Text3DParser::skipWaveMarkers(const std::string& text, int pos)
{
    if (isWaveMarker(text, pos)) {
        return pos + 2; // Skip both tildes
    }
    return pos;
}

void Text3DParser::renderWaveText(const ::Font& font, const std::string& text,
                                 Vector3 position, float fontSize, float fontSpacing,
                                 float lineSpacing, bool backface, 
                                 const WaveTextConfig* config, float time, Color tint)
{
    if (!Text3DWave::isValidConfig(config)) {
        std::cerr << "Error: WaveTextConfig is null!" << std::endl;
        return;
    }

    int length = static_cast<int>(text.length());
    float textOffsetY = 0.0f;
    float textOffsetX = 0.0f;
    float scale = fontSize / static_cast<float>(font.baseSize);
    bool wave = false;

    for (int i = 0, k = 0; i < length; ++k) {
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // Handle malformed UTF-8 sequences
        if (codepoint == 0x3f) codepointByteCount = 1;

        if (codepoint == '\n') {
            handleWaveNewline(textOffsetX, textOffsetY, fontSize, lineSpacing, k);
        } else if (codepoint == '~' && i + 1 < length && text[i + 1] == '~') {
            codepointByteCount += 1;
            wave = !wave;
            i += codepointByteCount;
            continue;
        } else {
            processWaveCharacter(font, codepoint, index, position, textOffsetX, textOffsetY,
                               fontSize, fontSpacing, lineSpacing, scale, backface, tint,
                               wave, config, time, k);
        }

        i += codepointByteCount;
    }
}

void Text3DParser::processWaveCharacter(const ::Font& font, int codepoint, int index,
                                       Vector3& position, float& textOffsetX, float& textOffsetY,
                                       float fontSize, float fontSpacing, float lineSpacing,
                                       float scale, bool backface, Color tint,
                                       bool waveActive, const WaveTextConfig* config,
                                       float time, int charIndex)
{
    (void)lineSpacing;
    if (codepoint != ' ' && codepoint != '\t') {
        Vector3 charPosition = {
            position.x + textOffsetX,
            position.y,
            position.z + textOffsetY
        };

        if (waveActive) {
            charPosition = Text3DWave::applyWaveEffect(charPosition, *config, time, charIndex);
        }

        Text3DCodepoint::draw(font, codepoint, charPosition, fontSize, backface, tint);
    }

    // Advance to next character position
    if (font.glyphs[index].advanceX == 0) {
        textOffsetX += static_cast<float>(font.recs[index].width) * scale + fontSpacing;
    } else {
        textOffsetX += static_cast<float>(font.glyphs[index].advanceX) * scale + fontSpacing;
    }
}

void Text3DParser::handleWaveNewline(float& textOffsetX, float& textOffsetY,
                                    float fontSize, float lineSpacing, int& charIndex)
{
    textOffsetY += fontSize + lineSpacing;
    textOffsetX = 0.0f;
    charIndex = 0;
}

}  // namespace raylibcpp
