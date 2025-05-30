/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Input encapsulation
*/

#include "Input.hpp"

namespace raylibcpp {

bool Input::isKeyPressed(int key) {
    return IsKeyPressed(key);
}

bool Input::isMouseButtonPressed(int button) {
    return IsMouseButtonPressed(button);
}

bool Input::isKeyDown(int key) {
    return IsKeyDown(key);
}

}  // namespace raylibcpp
