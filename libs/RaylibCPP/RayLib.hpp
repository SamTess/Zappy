/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** RayLib encapsulation
*/

#pragma once

#include <memory>
#include <optional>
#include <string>
#include <map>

#include "../../src/Shared/IGraphicsLib.hpp"
#include "window/Window.hpp"
#include "shape/Shape.hpp"
#include "texture/Texture.hpp"
#include "font/Font.hpp"
#include "input/Input.hpp"
#include "audio/Audio.hpp"
#include "camera/Camera.hpp"

class RayLib : public IGraphicsLib {
public:
    RayLib();
    ~RayLib() override;

    void InitWindow(int width, int height, const std::string& title) override;
    void CloseWindow() override;
    bool WindowShouldClose() override;
    void BeginDrawing() override;
    void EndDrawing() override;
    void ClearBackground(ZappyTypes::Color color) override;

    void DrawRectangle(int x, int y, int width, int height, ZappyTypes::Color color) override;
    void DrawCircle(int centerX, int centerY, float radius, ZappyTypes::Color color) override;

    void DrawCube(ZappyTypes::Vector3 position, float width, float height, float length, ZappyTypes::Color color) override;
    void DrawSphere(ZappyTypes::Vector3 centerPos, float radius, ZappyTypes::Color color) override;
    void DrawPlane(ZappyTypes::Vector3 centerPos, ZappyTypes::Vector2 size, ZappyTypes::Color color) override;
    void DrawGrid(int slices, float spacing) override;
    void DrawCylinder(ZappyTypes::Vector3 position, float radiusTop, float radiusBottom, float height, int slices, ZappyTypes::Color color) override;
    void DrawLine3D(ZappyTypes::Vector3 startPos, ZappyTypes::Vector3 endPos, ZappyTypes::Color color) override;

    // Textures
    int LoadTexture2D(const std::string& path) override;
    void DrawTexture2D(int textureId, int x, int y) override;
    void UnloadTexture2D(int textureId) override;
    bool IsTextureReady(int textureId) const override;

    // Texte
    void LoadFont(const std::string& path) override;
    void DrawText(const std::string& text, int x, int y, int size, ZappyTypes::Color color) override;
    void UnloadFont() override;

    // Entrées
    bool IsKeyPressed(int key) override;
    bool IsKeyDown(int key) override;
    bool IsMouseButtonPressed(int button) override;
    bool IsMouseButtonDown(int button) override;
    bool IsMouseButtonReleased(int button) override;
    int GetMouseX() override;
    int GetMouseY() override;
    ZappyTypes::Vector2 GetMousePosition() override;
    float GetMouseWheelMove() override;

    // Audio
    void PlaySound(const std::string& path) override;
    void StopSound() override;
    void SetSoundVolume(float volume) override;
    void PlayMusic(const std::string& path) override;
    void StopMusic() override;
    void SetMusicVolume(float volume) override;
    void UpdateMusic() override;

    // Caméras
    void CreateCamera2D() override;
    void CreateCamera3D(ZappyTypes::Vector3 position, ZappyTypes::Vector3 target, ZappyTypes::Vector3 up, float fovy, int projection) override;
    void BeginCamera3D() override;
    void EndCamera3D() override;

    // Textures 3D
    void LoadTexture3D(const std::string& path) override;
    void BindTexture3D(int unit = 0) override;
    void UnloadTexture3D() override;

    // Modèles 3D
    int LoadModel3D(const std::string& path) override;
    void DrawModel3D(int modelId, ZappyTypes::Vector3 position, float scale, ZappyTypes::Color color) override;
    void DrawModelEx(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Vector3 rotationAxis, float rotationAngle, float scale) override;
    void UnloadModel3D(int modelId) override;
    int LoadModelWithTexture(const std::string& modelPath, const std::string& texturePath) override;

private:
    std::unique_ptr<raylibcpp::Window> _window;
    std::map<int, std::unique_ptr<raylibcpp::Texture>> _textures;
    std::map<int, std::unique_ptr<raylibcpp::ModelWrap>> _models;
    int _nextModelId = 1;
    std::optional<raylibcpp::Font> _font;
    std::optional<raylibcpp::SoundWrap> _sound;
    std::optional<raylibcpp::MusicWrap> _music;
    std::optional<raylibcpp::Camera2DWrap> _camera2D;
    std::optional<raylibcpp::Camera3DWrap> _camera3D;
    std::optional<raylibcpp::Texture3DWrap> _texture3D;
    bool _initialized = false;
};
