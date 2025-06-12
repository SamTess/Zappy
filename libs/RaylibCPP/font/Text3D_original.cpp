/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Text3D implementation
*/

#include "Text3D.hpp"
#include "text3d/Text3DCore.hpp"
#include "text3d/Text3DCodepoint.hpp"
#include "text3d/Text3DHelper.hpp"
#include "text3d/Text3DParser.hpp"
#include "text3d/Text3DWave.hpp"
#include <iostream>

namespace raylibcpp {

Text3D::Text3D() {}

Text3D::~Text3D() {}

// Draw codepoint at specified position in 3D space
void Text3D::DrawTextCodepoint3D(::Font font, int codepoint, Vector3 position, float fontSize, bool backface, Color tint)
{
    Text3DCodepoint::draw(font, codepoint, position, fontSize, backface, tint);
}

// Draw a 2D text in 3D space
void Text3D::DrawText3D(::Font font, const std::string& text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, Color tint)
{
    std::cout << "DrawTextWave3D: " << text << std::endl;
    int length = text.length();          // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0.0f;            // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;            // Offset X to next character to draw

    float scale = fontSize/(float)font.baseSize;

    for (int i = 0; i < length;)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;

        if (codepoint == '\n')
        {
            // NOTE: Fixed line spacing of 1.5 line-height
            // TODO: Support custom line spacing defined by user
            textOffsetY += fontSize + lineSpacing;
            textOffsetX = 0.0f;
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                DrawTextCodepoint3D(font, codepoint, (Vector3){ position.x + textOffsetX, position.y, position.z + textOffsetY }, fontSize, backface, tint);
            }

            if (font.glyphs[index].advanceX == 0) textOffsetX += (float)font.recs[index].width*scale + fontSpacing;
            else textOffsetX += (float)font.glyphs[index].advanceX*scale + fontSpacing;
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }
}

// Draw a 2D text in 3D space and wave the parts that start with `~~` and end with `~~`
void Text3D::DrawTextWave3D(::Font font, const std::string& text, Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, WaveTextConfig* config, float time, Color tint)
{
    std::cout << "DrawTextWave3D: " << text << std::endl;
    if (config == nullptr) {
        std::cerr << "Error: WaveTextConfig is null!" << std::endl;
        return;
    }
    int length = text.length();            // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0.0f;              // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;              // Offset X to next character to draw

    float scale = fontSize/(float)font.baseSize;

    bool wave = false;

    for (int i = 0, k = 0; i < length; ++k)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;

        if (codepoint == '\n')
        {
            // NOTE: Fixed line spacing of 1.5 line-height
            // TODO: Support custom line spacing defined by user
            textOffsetY += fontSize + lineSpacing;
            textOffsetX = 0.0f;
            k = 0;
        }
        else if (codepoint == '~')
        {
            if (i + 1 < length && text[i+1] == '~')
            {
                codepointByteCount += 1;
                wave = !wave;
                i += codepointByteCount;
                continue;
            }
        }
        else
        {
            if ((codepoint != ' ') && (codepoint != '\t'))
            {
                Vector3 pos = position;
                if (wave) // Apply the wave effect
                {
                    pos.x += sinf(time*config->waveSpeed.x-k*config->waveOffset.x)*config->waveRange.x;
                    pos.y += sinf(time*config->waveSpeed.y-k*config->waveOffset.y)*config->waveRange.y;
                    pos.z += sinf(time*config->waveSpeed.z-k*config->waveOffset.z)*config->waveRange.z;
                }

                DrawTextCodepoint3D(font, codepoint, (Vector3){ pos.x + textOffsetX, pos.y, pos.z + textOffsetY }, fontSize, backface, tint);
            }

            if (font.glyphs[index].advanceX == 0) textOffsetX += (float)font.recs[index].width*scale + fontSpacing;
            else textOffsetX += (float)font.glyphs[index].advanceX*scale + fontSpacing;
        }

        i += codepointByteCount;   // Move text bytes counter to next codepoint
    }
}

// Measure a text in 3D ignoring the `~~` chars
Vector3 Text3D::MeasureTextWave3D(::Font font, const std::string& text, float fontSize, float fontSpacing, float lineSpacing)
{
    std::cout << "DrawTextWave3D: " << text << std::endl;
    int len = text.length();
    int tempLen = 0;                // Used to count longer text line num chars
    int lenCounter = 0;

    float tempTextWidth = 0.0f;     // Used to count longer text line width

    float scale = fontSize/(float)font.baseSize;
    float textHeight = scale;
    float textWidth = 0.0f;

    int letter = 0;                 // Current character
    int index = 0;                  // Index position in sprite font

    for (int i = 0; i < len; i++)
    {
        int next = 0;
        letter = GetCodepoint(&text[i], &next);
        index = GetGlyphIndex(font, letter);

        // NOTE: normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol so to not skip any we set next = 1
        if (letter == 0x3f) next = 1;
        i += next - 1;

        if (letter != '\n')
        {
            if (letter == '~' && i + 1 < len && text[i+1] == '~')
            {
                i++;
            }
            else
            {
                lenCounter++;
                if (font.glyphs[index].advanceX != 0) textWidth += font.glyphs[index].advanceX*scale;
                else textWidth += (font.recs[index].width + font.glyphs[index].offsetX)*scale;
            }
        }
        else
        {
            if (tempTextWidth < textWidth) tempTextWidth = textWidth;
            lenCounter = 0;
            textWidth = 0.0f;
            textHeight += fontSize + lineSpacing;
        }

        if (tempLen < lenCounter) tempLen = lenCounter;
    }

    if (tempTextWidth < textWidth) tempTextWidth = textWidth;

    Vector3 vec = { 0.0f, 0.0f, 0.0f };  // Initialisation correcte de tous les champs
    vec.x = tempTextWidth + (float)((tempLen - 1)*fontSpacing); // Adds chars spacing to measure
    vec.y = 0.25f;
    vec.z = textHeight;

    return vec;
}

}  // namespace raylibcpp
