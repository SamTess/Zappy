/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Helper utilities for common operations
*/

#pragma once
#include <raylib.h>
#include <string>

namespace raylibcpp {

namespace Text3DHelper {
    int getGlyphIndex(const ::Font& font, int codepoint);
    float calculateScale(const ::Font& font, float fontSize);
    Vector3 calculateGlyphPosition(const ::Font& font, int index, Vector3 basePosition, float scale);
    Rectangle calculateSourceRectangle(const ::Font& font, int index);
    void calculateGlyphDimensions(const ::Font& font, int index, float scale, float& width, float& height);
    bool isRenderableCharacter(int codepoint);
    float calculateAdvanceX(const ::Font& font, int index, float scale, float fontSpacing);
    void calculateTextureCoordinates(const ::Font& font, Rectangle srcRec,
        float& tx, float& ty, float& tw, float& th);
    bool isValidFont(const ::Font& font);
    bool isValidRenderParams(float fontSize, float fontSpacing, float lineSpacing);
    bool isWhitespace(int codepoint);
    bool isNewline(int codepoint);

    static constexpr float MIN_FONT_SIZE = 0.1f;
    static constexpr float MAX_FONT_SIZE = 1000.0f;
    static constexpr float MIN_SPACING = -100.0f;
    static constexpr float MAX_SPACING = 1000.0f;
}

}  // namespace raylibcpp
