/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** CameraController
*/

#include "CameraController.hpp"
#include "Constants.hpp"
#include <cmath>

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
      m_keyboardSpeed(0.5f)
{
}

void CameraController::init(IGraphicsLib& graphics)
{
    updateCameraPosition(graphics);
}

void CameraController::update(IGraphicsLib& graphics)
{
    handleMouseInput(graphics);
    handleKeyboardInput(graphics);
    updateCameraPosition(graphics);
}

void CameraController::reset()
{
    m_cameraDistance = 25.0f;
    m_cameraAngleY = 0.8f;
    m_cameraAngleX = 0.6f;
    m_cameraTarget = m_initialCameraTarget;
}

void CameraController::handleMouseInput(IGraphicsLib& graphics)
{
    if (graphics.IsMouseButtonPressed(0)) {
        m_isDragging = true;
        m_lastMouseX = graphics.GetMouseX();
        m_lastMouseY = graphics.GetMouseY();
    }
    
    if (graphics.IsMouseButtonReleased(0)) {
        m_isDragging = false;
    }
    
    if (m_isDragging) {
        int currentMouseX = graphics.GetMouseX();
        int currentMouseY = graphics.GetMouseY();
        
        float deltaX = static_cast<float>(currentMouseX - m_lastMouseX);
        float deltaY = static_cast<float>(currentMouseY - m_lastMouseY);
        
        m_cameraAngleY += deltaX * m_mouseSensitivity;
        m_cameraAngleX -= deltaY * m_mouseSensitivity;
        
        if (m_cameraAngleX > 1.5f) m_cameraAngleX = 1.5f;
        if (m_cameraAngleX < -1.5f) m_cameraAngleX = -1.5f;
        
        m_lastMouseX = currentMouseX;
        m_lastMouseY = currentMouseY;
    }
    
    float wheelMovement = graphics.GetMouseWheelMove();
    if (wheelMovement != 0) {
        m_cameraDistance -= wheelMovement * 2.0f;
        
        if (m_cameraDistance < 2.0f) m_cameraDistance = 2.0f;
        if (m_cameraDistance > 50.0f) m_cameraDistance = 50.0f;
    }
}

void CameraController::handleKeyboardInput(IGraphicsLib& graphics)
{
    if (graphics.IsKeyPressed(KEY_R)) {
        reset();
    }
    
    if (graphics.IsKeyDown(KEY_W) || graphics.IsKeyDown(KEY_UP)) {
        m_cameraTarget.z -= m_keyboardSpeed;
    }
    if (graphics.IsKeyDown(KEY_S) || graphics.IsKeyDown(KEY_DOWN)) {
        m_cameraTarget.z += m_keyboardSpeed;
    }
    if (graphics.IsKeyDown(KEY_A) || graphics.IsKeyDown(KEY_LEFT)) {
        m_cameraTarget.x -= m_keyboardSpeed;
    }
    if (graphics.IsKeyDown(KEY_D) || graphics.IsKeyDown(KEY_RIGHT)) {
        m_cameraTarget.x += m_keyboardSpeed;
    }
}

void CameraController::updateCameraPosition(IGraphicsLib& graphics)
{
    m_cameraPosition.x = m_cameraTarget.x + m_cameraDistance * std::cos(m_cameraAngleY) * std::cos(m_cameraAngleX);
    m_cameraPosition.y = m_cameraTarget.y + m_cameraDistance * std::sin(m_cameraAngleX);
    m_cameraPosition.z = m_cameraTarget.z + m_cameraDistance * std::sin(m_cameraAngleY) * std::cos(m_cameraAngleX);
    
    graphics.CreateCamera3D(m_cameraPosition, m_cameraTarget, m_cameraUp, 45.0f, 0);
}
