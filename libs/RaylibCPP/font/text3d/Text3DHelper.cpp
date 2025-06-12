/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Helper utilities implementation
*/

#include "Text3DHelper.hpp"
#include "Text3DCore.hpp"

namespace raylibcpp {

namespace Text3DHelper {

int getGlyphIndex(const ::Font& font, int codepoint) {
    return GetGlyphIndex(font, codepoint);
}

float calculateScale(const ::Font& font, float fontSize) {
    return fontSize / static_cast<float>(font.baseSize);
}

Vector3 calculateGlyphPosition(const ::Font& font, int index, Vector3 basePosition, float scale) {
    Vector3 position = basePosition;
    position.x += static_cast<float>(font.glyphs[index].offsetX - font.glyphPadding) * scale;
    position.z += static_cast<float>(font.glyphs[index].offsetY - font.glyphPadding) * scale;
    return position;
}

Rectangle calculateSourceRectangle(const ::Font& font, int index) {
    return {
        font.recs[index].x - static_cast<float>(font.glyphPadding),
        font.recs[index].y - static_cast<float>(font.glyphPadding),
        font.recs[index].width + 2.0f * font.glyphPadding,
        font.recs[index].height + 2.0f * font.glyphPadding
    };
}

void calculateGlyphDimensions(const ::Font& font, int index, float scale, float& width, float& height) {
    width = static_cast<float>(font.recs[index].width + 2.0f * font.glyphPadding) * scale;
    height = static_cast<float>(font.recs[index].height + 2.0f * font.glyphPadding) * scale;
}

bool isRenderableCharacter(int codepoint) {
    return (codepoint != ' ') && (codepoint != '\t');
}

float calculateAdvanceX(const ::Font& font, int index, float scale, float fontSpacing) {
    if (font.glyphs[index].advanceX == 0) {
        return static_cast<float>(font.recs[index].width) * scale + fontSpacing;
    }
    return static_cast<float>(font.glyphs[index].advanceX) * scale + fontSpacing;
}

void calculateTextureCoordinates(const ::Font& font, Rectangle srcRec, 
                               float& tx, float& ty, float& tw, float& th) {
    tx = srcRec.x / font.texture.width;
    ty = srcRec.y / font.texture.height;
    tw = (srcRec.x + srcRec.width) / font.texture.width;
    th = (srcRec.y + srcRec.height) / font.texture.height;
}

bool isValidFont(const ::Font& font) {
    return font.texture.id > 0 && font.baseSize > 0 && font.glyphs != nullptr && font.recs != nullptr;
}

bool isValidRenderParams(float fontSize, float fontSpacing, float lineSpacing) {
    return fontSize >= MIN_FONT_SIZE && fontSize <= MAX_FONT_SIZE &&
           fontSpacing >= MIN_SPACING && fontSpacing <= MAX_SPACING &&
           lineSpacing >= MIN_SPACING && lineSpacing <= MAX_SPACING;
}

bool isWhitespace(int codepoint) {
    return codepoint == ' ' || codepoint == '\t';
}

bool isNewline(int codepoint) {
    return codepoint == '\n';
}

}  // namespace Text3DHelper

}  // namespace raylibcpp
