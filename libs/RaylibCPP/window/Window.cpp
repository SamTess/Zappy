/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib window encapsulation
*/

#include "Window.hpp"
#include <string>

namespace raylibcpp {

Window::Window(int width, int height, const std::string& title) {
    InitWindow(width, height, title.c_str());
}

Window::~Window() {
    CloseWindow();
}

bool Window::shouldClose() const {
    return WindowShouldClose();
}

void Window::beginDrawing() const {
    BeginDrawing();
}

void Window::endDrawing() const {
    EndDrawing();
}

void Window::clear(Color color) const {
    ClearBackground(color);
}

}  // namespace raylibcpp
