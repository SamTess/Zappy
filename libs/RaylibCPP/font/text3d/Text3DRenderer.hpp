/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Renderer - handles basic text rendering
*/

#pragma once
#include <raylib.h>
#include <string>
#include "Text3DCore.hpp"
#include "Text3DHelper.hpp"

namespace raylibcpp {

class Text3DRenderer {
public:
    static void renderBasicText(const ::Font& font, const std::string& text,
        Vector3 position, float fontSize, float fontSpacing,
        float lineSpacing, bool backface, Color tint);

private:
    static void processCharacter(const ::Font& font, int codepoint, int index,
        const RenderStatePtr& renderState,
        float fontSize, float fontSpacing, float lineSpacing,
        float scale, bool backface, Color tint);

    static void handleNewline(const RenderStatePtr& renderState,
        float fontSize, float lineSpacing);

    static float getCharacterAdvance(const ::Font& font, int index, float scale, float fontSpacing);
};

}  // namespace raylibcpp
