/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CameraController
*/

#include <cmath>
#include <memory>
#include <iostream>
#include <algorithm>
#include "CameraController.hpp"
#include "../Constants.hpp"

CameraController::CameraController(float initialDistance)
    : m_cameraPosition({15.0f, 15.0f, 15.0f}),
      m_cameraTarget({0.0f, 0.0f, 0.0f}),
      m_cameraUp({0.0f, 1.0f, 0.0f}),
      m_initialCameraTarget({0.0f, 0.0f, 0.0f}),
      m_isDragging(false),
      m_lastMouseX(0),
      m_lastMouseY(0),
      m_cameraDistance(initialDistance),
      m_cameraAngleY(0.8f),
      m_cameraAngleX(0.6f),
      m_mouseSensitivity(0.003f),
      m_keyboardSpeed(0.1f) {
}

void CameraController::init(std::shared_ptr<IGraphicsLib> graphics) {
    updateCameraPosition(graphics);
}

void CameraController::update(std::shared_ptr<IGraphicsLib> graphics) {
    handleMouseInput(graphics);
    handleKeyboardInput(graphics);
    updateCameraPosition(graphics);
}

void CameraController::reset() {
    m_cameraDistance = 25.0f;
    m_cameraAngleY = 0.8f;
    m_cameraAngleX = 0.6f;
    m_cameraTarget = m_initialCameraTarget;
}

void CameraController::setMousePosition(int x, int y) {
    m_lastMouseX = x;
    m_lastMouseY = y;
}

void CameraController::handleMouseInput(std::shared_ptr<IGraphicsLib> graphics) {
    if (!graphics) {
        return;
    }
    if (graphics->IsMouseButtonPressed(0)) {
        m_isDragging = true;
        m_lastMouseX = graphics->GetMouseX();
        m_lastMouseY = graphics->GetMouseY();
    }

    if (graphics->IsMouseButtonReleased(0)) {
        m_isDragging = false;
    }

    if (m_isDragging) {
        int currentMouseX = graphics->GetMouseX();
        int currentMouseY = graphics->GetMouseY();

        float deltaX = static_cast<float>(currentMouseX - m_lastMouseX);
        float deltaY = static_cast<float>(currentMouseY - m_lastMouseY);

        m_cameraAngleY += deltaX * m_mouseSensitivity;
        m_cameraAngleX -= deltaY * m_mouseSensitivity;

        if (m_cameraAngleX > 1.5f) m_cameraAngleX = 1.5f;
        if (m_cameraAngleX < -1.5f) m_cameraAngleX = -1.5f;

        m_lastMouseX = currentMouseX;
        m_lastMouseY = currentMouseY;
    }

    float wheelMovement = graphics->GetMouseWheelMove();
    if (wheelMovement != 0) {
        float zoomSpeed = m_cameraDistance * 0.1f;
        if (zoomSpeed < 0.5f) zoomSpeed = 0.5f;
        if (zoomSpeed > 3.0f) zoomSpeed = 3.0f;
        m_cameraDistance -= wheelMovement * zoomSpeed;
        if (m_cameraDistance < 2.0f) m_cameraDistance = 2.0f;
        if (m_cameraDistance > 50.0f) m_cameraDistance = 50.0f;
    }
}

void CameraController::handleKeyboardInput(std::shared_ptr<IGraphicsLib> graphics) {
    if (graphics->IsKeyPressed(KEY_R)) {
        reset();
    }
    ZappyTypes::Vector3 forward = {
        std::cos(m_cameraAngleY),
        0.0f,
        std::sin(m_cameraAngleY)
    };
    float forwardLength = std::sqrt(forward.x * forward.x + forward.z * forward.z);
    if (forwardLength > 0.0001f) {
        forward.x /= forwardLength;
        forward.z /= forwardLength;
    }
    ZappyTypes::Vector3 right = {
        -forward.z,
        0.0f,
        forward.x
    };

    ZappyTypes::Vector3 newTarget = m_cameraTarget;
    if (graphics->IsKeyDown(KEY_S) || graphics->IsKeyDown(KEY_DOWN)) {
        newTarget.x += forward.x * m_keyboardSpeed;
        newTarget.z += forward.z * m_keyboardSpeed;
    }
    if (graphics->IsKeyDown(KEY_W) || graphics->IsKeyDown(KEY_Z) || graphics->IsKeyDown(KEY_UP)) {
        newTarget.x -= forward.x * m_keyboardSpeed;
        newTarget.z -= forward.z * m_keyboardSpeed;
    }
    if (graphics->IsKeyDown(KEY_D) || graphics->IsKeyDown(KEY_RIGHT)) {
        newTarget.x -= right.x * m_keyboardSpeed;
        newTarget.z -= right.z * m_keyboardSpeed;
    }
    if (graphics->IsKeyDown(KEY_Q) || graphics->IsKeyDown(KEY_A) || graphics->IsKeyDown(KEY_LEFT)) {
        newTarget.x += right.x * m_keyboardSpeed;
        newTarget.z += right.z * m_keyboardSpeed;
    }
    float halfWidth = m_mapWidth * 1.2f;
    float halfHeight = m_mapHeight * 1.2f;
    float margin = 2.0f;
    newTarget.x = std::max(std::min(newTarget.x, halfWidth - margin), -halfWidth + margin);
    newTarget.z = std::max(std::min(newTarget.z, halfHeight - margin), -halfHeight + margin);
    m_cameraTarget = newTarget;
}

void CameraController::updateCameraPosition(std::shared_ptr<IGraphicsLib> graphics) {
    if (m_cameraDistance < 2.0f) m_cameraDistance = 2.0f;
    if (m_cameraDistance > 50.0f) m_cameraDistance = 50.0f;
    if (m_cameraAngleX < 0.1f) m_cameraAngleX = 0.1f;
    if (m_cameraAngleX > 1.5f) m_cameraAngleX = 1.5f;
    m_cameraPosition.x = m_cameraTarget.x + m_cameraDistance * std::cos(m_cameraAngleY) * std::cos(m_cameraAngleX);
    m_cameraPosition.y = m_cameraTarget.y + m_cameraDistance * std::sin(m_cameraAngleX);
    m_cameraPosition.z = m_cameraTarget.z + m_cameraDistance * std::sin(m_cameraAngleY) * std::cos(m_cameraAngleX);
    if (m_cameraPosition.y < 1.0f) {
        m_cameraPosition.y = 1.0f;
    }
    graphics->CreateCamera3D(m_cameraPosition, m_cameraTarget, m_cameraUp, 45.0f, 0);
}
