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
    : _cameraPosition({15.0f, 15.0f, 15.0f}),
      _cameraTarget({0.0f, 0.0f, 0.0f}),
      _cameraUp({0.0f, 1.0f, 0.0f}),
      _initialCameraTarget({0.0f, 0.0f, 0.0f}),
      _isDragging(false),
      _lastMouseX(0),
      _lastMouseY(0),
      _cameraDistance(initialDistance),
      _cameraAngleY(0.8f),
      _cameraAngleX(0.6f),
      _mouseSensitivity(0.003f),
      _keyboardSpeed(0.1f) {
}

void CameraController::init(std::shared_ptr<IGraphicsLib> graphics) {
    updateCameraPosition(graphics);
}

void CameraController::update(std::shared_ptr<IGraphicsLib> graphics, bool uiHandledMouse, bool mouseOverUI) {
    if (!uiHandledMouse && !mouseOverUI)
        handleMouseInput(graphics, false, false);
    handleKeyboardInput(graphics);
    updateCameraPosition(graphics);
}

void CameraController::reset() {
    _cameraDistance = 25.0f;
    _cameraAngleY = 0.8f;
    _cameraAngleX = 0.6f;
    _cameraTarget = _initialCameraTarget;
}

void CameraController::setMousePosition(int x, int y) {
    _lastMouseX = x;
    _lastMouseY = y;
}

void CameraController::handleMouseInput(std::shared_ptr<IGraphicsLib> graphics, bool uiHandledMouse, bool mouseOverUI) {
    if (!graphics || uiHandledMouse || mouseOverUI)
        return;
    if (graphics->IsMouseButtonPressed(0)) {
        _isDragging = true;
        _lastMouseX = graphics->GetMouseX();
        _lastMouseY = graphics->GetMouseY();
    }
    if (graphics->IsMouseButtonReleased(0))
        _isDragging = false;
    if (_isDragging) {
        int currentMouseX = graphics->GetMouseX();
        int currentMouseY = graphics->GetMouseY();
        float deltaX = static_cast<float>(currentMouseX - _lastMouseX);
        float deltaY = static_cast<float>(currentMouseY - _lastMouseY);
        _cameraAngleY += deltaX * _mouseSensitivity;
        _cameraAngleX -= deltaY * _mouseSensitivity;
        if (_cameraAngleX > 1.5f)
            _cameraAngleX = 1.5f;
        if (_cameraAngleX < -1.5f)
            _cameraAngleX = -1.5f;
        _lastMouseX = currentMouseX;
        _lastMouseY = currentMouseY;
    }
    float wheelMovement = graphics->GetMouseWheelMove();
    if (wheelMovement != 0) {
        float zoomSpeed = _cameraDistance * 0.1f;
        if (zoomSpeed < 0.5f)
            zoomSpeed = 0.5f;
        if (zoomSpeed > 3.0f)
            zoomSpeed = 3.0f;
        _cameraDistance -= wheelMovement * zoomSpeed;
        if (_cameraDistance < 2.0f)
            _cameraDistance = 2.0f;
        if (_cameraDistance > 50.0f)
            _cameraDistance = 50.0f;
    }
}

void CameraController::handleKeyboardInput(std::shared_ptr<IGraphicsLib> graphics) {
    if (graphics->IsKeyPressed(KEY_R))
        reset();
    ZappyTypes::Vector3 forward = {std::cos(_cameraAngleY), 0.0f, std::sin(_cameraAngleY)};
    float forwardLength = std::sqrt(forward.x * forward.x + forward.z * forward.z);
    if (forwardLength > 0.0001f) {
        forward.x /= forwardLength;
        forward.z /= forwardLength;
    }
    ZappyTypes::Vector3 right = { -forward.z, 0.0f, forward.x };
    ZappyTypes::Vector3 newTarget = _cameraTarget;
    if (graphics->IsKeyDown(KEY_S) || graphics->IsKeyDown(KEY_DOWN)) {
        newTarget.x += forward.x * _keyboardSpeed;
        newTarget.z += forward.z * _keyboardSpeed;
    }
    if (graphics->IsKeyDown(KEY_W) || graphics->IsKeyDown(KEY_Z) || graphics->IsKeyDown(KEY_UP)) {
        newTarget.x -= forward.x * _keyboardSpeed;
        newTarget.z -= forward.z * _keyboardSpeed;
    }
    if (graphics->IsKeyDown(KEY_D) || graphics->IsKeyDown(KEY_RIGHT)) {
        newTarget.x -= right.x * _keyboardSpeed;
        newTarget.z -= right.z * _keyboardSpeed;
    }
    if (graphics->IsKeyDown(KEY_Q) || graphics->IsKeyDown(KEY_A) || graphics->IsKeyDown(KEY_LEFT)) {
        newTarget.x += right.x * _keyboardSpeed;
        newTarget.z += right.z * _keyboardSpeed;
    }
    newTarget.x = std::max(std::min(newTarget.x, 13 - 2.0f),  -13  + 2.0f);
    newTarget.z = std::max(std::min(newTarget.z, 13 - 2.0f),  -13  + 2.0f);
    _cameraTarget = newTarget;
}

void CameraController::updateCameraPosition(std::shared_ptr<IGraphicsLib> graphics) {
    if (_cameraDistance < 2.0f)
        _cameraDistance = 2.0f;
    if (_cameraDistance > 50.0f)
        _cameraDistance = 50.0f;
    if (_cameraAngleX < 0.1f)
        _cameraAngleX = 0.1f;
    if (_cameraAngleX > 1.5f)
        _cameraAngleX = 1.5f;
    _cameraPosition.x = _cameraTarget.x + _cameraDistance * std::cos(_cameraAngleY) * std::cos(_cameraAngleX);
    _cameraPosition.y = _cameraTarget.y + _cameraDistance * std::sin(_cameraAngleX);
    _cameraPosition.z = _cameraTarget.z + _cameraDistance * std::sin(_cameraAngleY) * std::cos(_cameraAngleX);
    if (_cameraPosition.y < 1.0f)
        _cameraPosition.y = 1.0f;
    graphics->CreateCamera3D(_cameraPosition, _cameraTarget, _cameraUp, 45.0f, 0);
}
