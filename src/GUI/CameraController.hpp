/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CameraController
*/

#pragma once

#include <memory>
#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/Common.hpp"

class CameraController {
public:
    explicit CameraController(float initialDistance = 25.0f);
    ~CameraController() = default;

    void init(std::shared_ptr<IGraphicsLib> graphics);
    void update(std::shared_ptr<IGraphicsLib> graphics);
    void reset();

    void handleMouseInput(std::shared_ptr<IGraphicsLib> graphics);
    void handleKeyboardInput(std::shared_ptr<IGraphicsLib> graphics);
    void updateCameraPosition(std::shared_ptr<IGraphicsLib> graphics);

    bool isDragging() const { return m_isDragging; }
    void setDragging(bool dragging) { m_isDragging = dragging; }
    void setMousePosition(int x, int y) { m_lastMouseX = x; m_lastMouseY = y; }

    ZappyTypes::Vector3& position() { return m_cameraPosition; }
    ZappyTypes::Vector3& target() { return m_cameraTarget; }
    ZappyTypes::Vector3& up() { return m_cameraUp; }

    float& distance() { return m_cameraDistance; }
    float& angleX() { return m_cameraAngleX; }
    float& angleY() { return m_cameraAngleY; }

private:
    ZappyTypes::Vector3 m_cameraPosition;
    ZappyTypes::Vector3 m_cameraTarget;
    ZappyTypes::Vector3 m_cameraUp;
    ZappyTypes::Vector3 m_initialCameraTarget;

    bool m_isDragging;
    int m_lastMouseX;
    int m_lastMouseY;

    float m_cameraDistance;
    float m_cameraAngleY;
    float m_cameraAngleX;
    float m_mouseSensitivity;
    float m_keyboardSpeed;
};
