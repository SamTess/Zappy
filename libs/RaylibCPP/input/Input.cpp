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

bool Input::isMouseButtonDown(int button) {
    return IsMouseButtonDown(button);
}

bool Input::isMouseButtonReleased(int button) {
    return IsMouseButtonReleased(button);
}

int Input::getMouseX() {
    return GetMouseX();
}

int Input::getMouseY() {
    return GetMouseY();
}

Vector2 Input::getMousePosition() {
    return GetMousePosition();
}

float Input::getMouseWheelMove() {
    return GetMouseWheelMove();
}

}  // namespace raylibcpp
