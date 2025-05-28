/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** GraphicLib interface
*/
#pragma once

#include <string>
#include <vector>
#include "Common.hpp"

class IGraphicsLib {
public:
    virtual ~IGraphicsLib() = default;
    // Fenêtre
    virtual void InitWindow(int width, int height, const char* title) = 0;
    virtual void CloseWindow() = 0;
    virtual bool WindowShouldClose() = 0;
    virtual void BeginDrawing() = 0;
    virtual void EndDrawing() = 0;
    virtual void ClearBackground(ZappyTypes::Color color) = 0;

    // Formes 2D
    virtual void DrawRectangle(int x, int y, int width, int height, ZappyTypes::Color color) = 0;
    virtual void DrawCircle(int centerX, int centerY, float radius, ZappyTypes::Color color) = 0;
    // Formes 3D
    virtual void DrawCube(ZappyTypes::Vector3 position, float width, float height, float length, ZappyTypes::Color color) = 0;
    virtual void DrawSphere(ZappyTypes::Vector3 centerPos, float radius, ZappyTypes::Color color) = 0;
    virtual void DrawPlane(ZappyTypes::Vector3 centerPos, ZappyTypes::Vector2 size, ZappyTypes::Color color) = 0;
    virtual void DrawGrid(int slices, float spacing) = 0;
    virtual void DrawCylinder(ZappyTypes::Vector3 position, float radiusTop, float radiusBottom, float height, int slices, ZappyTypes::Color color) = 0;
    virtual void DrawLine3D(ZappyTypes::Vector3 startPos, ZappyTypes::Vector3 endPos, ZappyTypes::Color color) = 0;

    // Textures
    virtual void LoadTexture2D(const std::string& path) = 0;
    virtual void DrawTexture2D(int x, int y) = 0;
    virtual void UnloadTexture2D() = 0;

    // Texte
    virtual void LoadFont(const std::string& path) = 0;
    virtual void DrawText(const std::string& text, int x, int y, int size, ZappyTypes::Color color) = 0;
    virtual void UnloadFont() = 0;

    // Entrées
    virtual bool IsKeyPressed(int key) = 0;
    virtual bool IsKeyDown(int key) = 0;
    virtual bool IsMouseButtonPressed(int button) = 0;

    // Audio (optionnel)
    virtual void PlaySound(const std::string& path) = 0;
    virtual void StopSound() = 0;
    virtual void SetSoundVolume(float volume) = 0;
    virtual void PlayMusic(const std::string& path) = 0;
    virtual void StopMusic() = 0;
    virtual void SetMusicVolume(float volume) = 0;
    virtual void UpdateMusic() = 0;
};
