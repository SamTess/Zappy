/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Text3D Codepoint renderer
*/

#pragma once
#include <raylib.h>
#include "Text3DCore.hpp"

namespace raylibcpp {

class Text3DCodepoint {
public:
    static void draw(const ::Font& font, int codepoint, Vector3 position,
        float fontSize, bool backface, Color tint);

private:
    static void renderQuad(const ::Font& font, Vector3 position, float width, float height,
        float tx, float ty, float tw, float th,
        bool backface, Color tint);

    static void drawDebugBoundary(Vector3 position, float width, float height);
};

}  // namespace raylibcpp
