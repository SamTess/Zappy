/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Camera encapsulation
*/

#pragma once
#include <raylib.h>

namespace raylibcpp {

class Camera2DWrap {
    public:
        Camera2DWrap();
        ~Camera2DWrap();
        Camera2D& get();
    private:
        Camera2D camera;
};

class Camera3DWrap {
    public:
        Camera3DWrap();
        Camera3DWrap(Vector3 position, Vector3 target, Vector3 up,
            float fovy, int projection = CAMERA_PERSPECTIVE);
        ~Camera3DWrap();
        Camera3D& get();
        void beginMode();
        void endMode();
    private:
        Camera3D camera;
};

}  // namespace raylibcpp
