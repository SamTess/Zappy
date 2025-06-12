/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Measurement implementation
*/

#include "Text3DMeasurement.hpp"
#include <algorithm>

namespace raylibcpp {

Vector3 Text3DMeasurement::measureText(const ::Font& font, const std::string& text,
                                     float fontSize, float fontSpacing, float lineSpacing) {
    if (text.empty()) {
        return {0.0f, 0.25f, 0.0f};
    }

    int len = static_cast<int>(text.length());
    int tempLen = 0;
    int lenCounter = 0;
    float tempTextWidth = 0.0f;
    float scale = fontSize / static_cast<float>(font.baseSize);
    float textHeight = scale;
    float textWidth = 0.0f;

    for (int i = 0; i < len; i++) {
        int next = 0;
        int letter = GetCodepoint(&text[i], &next);
        int index = GetGlyphIndex(font, letter);

        if (letter == 0x3f) next = 1;
        i += next - 1;

        if (letter != '\n') {
            if (isWaveMarker(text, i)) {
                i++;
            } else {
                lenCounter++;
                if (font.glyphs[index].advanceX != 0) {
                    textWidth += static_cast<float>(font.glyphs[index].advanceX) * scale;
                } else {
                    textWidth += static_cast<float>(font.recs[index].width + font.glyphs[index].offsetX) * scale;
                }
            }
        } else {
            tempTextWidth = std::max(tempTextWidth, textWidth);
            lenCounter = 0;
            textWidth = 0.0f;
            textHeight += fontSize + lineSpacing;
        }

        tempLen = std::max(tempLen, lenCounter);
    }
    tempTextWidth = std::max(tempTextWidth, textWidth);
    return {
        tempTextWidth + static_cast<float>((tempLen - 1) * fontSpacing),
        0.25f,
        textHeight
    };
}

bool Text3DMeasurement::isWaveMarker(const std::string& text, int pos) {
    return (pos < static_cast<int>(text.length()) - 1 &&
        text[pos] == '~' && text[pos + 1] == '~');
}

}  // namespace raylibcpp
