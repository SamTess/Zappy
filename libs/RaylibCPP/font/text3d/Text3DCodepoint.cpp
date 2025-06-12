/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Codepoint renderer implementation
*/

#include "Text3DCodepoint.hpp"
#include "Text3DHelper.hpp"
#include "rlgl.h"

namespace raylibcpp {

void Text3DCodepoint::draw(const ::Font& font, int codepoint, Vector3 position, 
                          float fontSize, bool backface, Color tint) {
    if (font.texture.id <= 0) return;
    
    // Get character info
    int index = Text3DHelper::getGlyphIndex(font, codepoint);
    float scale = Text3DHelper::calculateScale(font, fontSize);
    
    // Calculate position and dimensions
    Vector3 glyphPosition = Text3DHelper::calculateGlyphPosition(font, index, position, scale);
    Rectangle srcRec = Text3DHelper::calculateSourceRectangle(font, index);
    
    float width, height;
    Text3DHelper::calculateGlyphDimensions(font, index, scale, width, height);
    
    // Calculate texture coordinates
    float tx, ty, tw, th;
    Text3DHelper::calculateTextureCoordinates(font, srcRec, tx, ty, tw, th);
    
    // Draw debug boundary if enabled
    drawDebugBoundary(glyphPosition, width, height);
    
    // Render the glyph quad
    renderQuad(glyphPosition, width, height, tx, ty, tw, th, backface, tint);
}

void Text3DCodepoint::renderQuad(Vector3 position, float width, float height,
                                float tx, float ty, float tw, float th, 
                                bool backface, Color tint) {
    const float x = 0.0f;
    const float y = 0.0f;
    const float z = 0.0f;
    
    rlCheckRenderBatchLimit(4 + 4 * backface);
    
    rlPushMatrix();
    rlTranslatef(position.x, position.y, position.z);
    
    rlBegin(RL_QUADS);
    rlColor4ub(tint.r, tint.g, tint.b, tint.a);
    
    // Front Face
    rlNormal3f(0.0f, 1.0f, 0.0f);
    rlTexCoord2f(tx, ty); rlVertex3f(x, y, z);
    rlTexCoord2f(tx, th); rlVertex3f(x, y, z + height);
    rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height);
    rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);
    
    if (backface) {
        // Back Face
        rlNormal3f(0.0f, -1.0f, 0.0f);
        rlTexCoord2f(tx, ty); rlVertex3f(x, y, z);
        rlTexCoord2f(tw, ty); rlVertex3f(x + width, y, z);
        rlTexCoord2f(tw, th); rlVertex3f(x + width, y, z + height);
        rlTexCoord2f(tx, th); rlVertex3f(x, y, z + height);
    }
    
    rlEnd();
    rlPopMatrix();
}

void Text3DCodepoint::drawDebugBoundary(Vector3 position, float width, float height) {
    if (SHOW_LETTER_BOUNDRY) {
        Vector3 boundaryPos = {
            position.x + width / 2,
            position.y,
            position.z + height / 2
        };
        Vector3 boundarySize = { width, LETTER_BOUNDRY_SIZE, height };
        DrawCubeWiresV(boundaryPos, boundarySize, LETTER_BOUNDRY_COLOR);
    }
}

}  // namespace raylibcpp
