/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib font encapsulation
*/

#pragma once
#include <raylib.h>
#include <string>

namespace raylibcpp {

class Font {
    public:
        explicit Font(const std::string& path);
        ~Font();
        void drawText(const std::string& text, int x, int y, int size, Color color) const;
        ::Font get() const;
    private:
        ::Font font;
};

}  // namespace raylibcpp
