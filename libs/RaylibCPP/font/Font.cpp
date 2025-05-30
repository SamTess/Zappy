/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Font encapsulation
*/

#include "Font.hpp"
#include <string>

namespace raylibcpp {

Font::Font(const std::string& path) {
    font = LoadFont(path.c_str());
}

Font::~Font() {
    UnloadFont(font);
}

void Font::drawText(const std::string& text, int x, int y,
    int size, Color color) const {
    DrawTextEx(font, text.c_str(), {static_cast<float>(x), static_cast<float>(y)},
        static_cast<float>(size), 1, color);
}

::Font Font::get() const {
    return font;
}

}  // namespace raylibcpp
