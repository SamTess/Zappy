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
    // Draw a single codepoint in 3D space
    static void draw(const ::Font& font, int codepoint, Vector3 position, 
                    float fontSize, bool backface, Color tint);

private:
    // Render the quad for the codepoint
    static void renderQuad(Vector3 position, float width, float height,
                          float tx, float ty, float tw, float th, 
                          bool backface, Color tint);
    
    // Draw debug boundaries if enabled
    static void drawDebugBoundary(Vector3 position, float width, float height);
};

}  // namespace raylibcpp
