/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CameraController
*/

#pragma once

#include <memory>
#include "../../Shared/IGraphicsLib.hpp"
#include "../../Shared/Common.hpp"

class CameraController {
public:
    explicit CameraController(float initialDistance = 25.0f);
    ~CameraController() = default;

    void init(std::shared_ptr<IGraphicsLib> graphics);
    void update(std::shared_ptr<IGraphicsLib> graphics, bool uiHandledMouse, bool mouseOverUI);
    void reset();

    void handleMouseInput(std::shared_ptr<IGraphicsLib> graphics, bool uiHandledMouse, bool mouseOverUI);
    void handleKeyboardInput(std::shared_ptr<IGraphicsLib> graphics);
    void updateCameraPosition(std::shared_ptr<IGraphicsLib> graphics);

    bool isDragging() const { return _isDragging; }
    void setDragging(bool dragging) { _isDragging = dragging; }
    void setMousePosition(int x, int y);

    const ZappyTypes::Vector3& position() const { return _cameraPosition; }
    const ZappyTypes::Vector3& target() const { return _cameraTarget; }
    const ZappyTypes::Vector3& up() const { return _cameraUp; }

    float distance() const { return _cameraDistance; }
    float angleX() const { return _cameraAngleX; }
    float angleY() const { return _cameraAngleY; }

    void setMapDimensions(int width, int height) {
        _mapWidth = width;
        _mapHeight = height;
    }

private:
    ZappyTypes::Vector3 _cameraPosition;
    ZappyTypes::Vector3 _cameraTarget;
    ZappyTypes::Vector3 _cameraUp;
    ZappyTypes::Vector3 _initialCameraTarget;

    bool _isDragging;
    int _lastMouseX;
    int _lastMouseY;

    float _cameraDistance;
    float _cameraAngleY;
    float _cameraAngleX;
    float _mouseSensitivity;
    float _keyboardSpeed;

    int _mapWidth = 20;
    int _mapHeight = 20;
};
