/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Input encapsulation
*/

#pragma once
#include <raylib.h>

namespace raylibcpp {

class Input {
    public:
        static bool isKeyPressed(int key);
        static bool isMouseButtonPressed(int button);
        static bool isKeyDown(int key);
        static bool isMouseButtonDown(int button);
        static bool isMouseButtonReleased(int button);
        static int getMouseX();
        static int getMouseY();
        static Vector2 getMousePosition();
        static float getMouseWheelMove();
};

}  // namespace raylibcpp
