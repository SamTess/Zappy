/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Camera encapsulation
*/

#include "Camera.hpp"

namespace raylibcpp {

Camera2DWrap::Camera2DWrap() {
    camera.offset = (Vector2){0.0f, 0.0f};
    camera.target = (Vector2){0.0f, 0.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

Camera2DWrap::~Camera2DWrap() {}

Camera2D& Camera2DWrap::get() {
    return camera;
}

Camera3DWrap::Camera3DWrap() {
    camera.position = (Vector3){4.0f, 4.0f, 10.0f};
    camera.target = (Vector3){0.0f, 1.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

Camera3DWrap::Camera3DWrap(Vector3 position, Vector3 target, Vector3 up, float fovy, int projection) {
    camera.position = position;
    camera.target = target;
    camera.up = up;
    camera.fovy = fovy;
    camera.projection = projection;
}

Camera3DWrap::~Camera3DWrap() {}

Camera3D& Camera3DWrap::get() {
    return camera;
}

void Camera3DWrap::beginMode() {
    BeginMode3D(camera);
}

void Camera3DWrap::endMode() {
    EndMode3D();
}
}  // namespace raylibcpp
