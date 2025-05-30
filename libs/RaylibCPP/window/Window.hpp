/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib window encapsulation
*/

#pragma once
#include <raylib.h>
#include <string>

namespace raylibcpp {

class Window {
    public:
        Window(int width, int height, const std::string& title);
        ~Window();
        bool shouldClose() const;
        void beginDrawing() const;
        void endDrawing() const;
        void clear(Color color = RAYWHITE) const;
};

}  // namespace raylibcpp
