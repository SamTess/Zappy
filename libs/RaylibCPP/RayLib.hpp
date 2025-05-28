#pragma once

#include <memory>
#include <optional>
#include <string>

#include "../../src/Shared/IGraphicsLib.hpp"
#include "window/Window.hpp"
#include "shape/Shape.hpp"
#include "texture/Texture.hpp"
#include "font/Font.hpp"
#include "input/Input.hpp"
#include "audio/Audio.hpp"

class RayLib : public IGraphicsLib {
public:
    RayLib();
    ~RayLib() override;
    // Fenêtre
    void InitWindow(int width, int height, const char* title) override;
    void CloseWindow() override;
    bool WindowShouldClose() override;
    void BeginDrawing() override;
    void EndDrawing() override;
    void ClearBackground(ZappyTypes::Color color) override;

    // Formes 2D
    void DrawRectangle(int x, int y, int width, int height, ZappyTypes::Color color) override;
    void DrawCircle(int centerX, int centerY, float radius, ZappyTypes::Color color) override;
    // Formes 3D
    void DrawCube(ZappyTypes::Vector3 position, float width, float height, float length, ZappyTypes::Color color) override;
    void DrawSphere(ZappyTypes::Vector3 centerPos, float radius, ZappyTypes::Color color) override;
    void DrawPlane(ZappyTypes::Vector3 centerPos, ZappyTypes::Vector2 size, ZappyTypes::Color color) override;
    void DrawGrid(int slices, float spacing) override;
    void DrawCylinder(ZappyTypes::Vector3 position, float radiusTop, float radiusBottom, float height, int slices, ZappyTypes::Color color) override;
    void DrawLine3D(ZappyTypes::Vector3 startPos, ZappyTypes::Vector3 endPos, ZappyTypes::Color color) override;

    // Textures
    void LoadTexture2D(const std::string& path) override;
    void DrawTexture2D(int x, int y) override;
    void UnloadTexture2D() override;

    // Texte
    void LoadFont(const std::string& path) override;
    void DrawText(const std::string& text, int x, int y, int size, ZappyTypes::Color color) override;
    void UnloadFont() override;

    // Entrées
    bool IsKeyPressed(int key) override;
    bool IsKeyDown(int key) override;
    bool IsMouseButtonPressed(int button) override;

    // Audio
    void PlaySound(const std::string& path) override;
    void StopSound() override;
    void SetSoundVolume(float volume) override;
    void PlayMusic(const std::string& path) override;
    void StopMusic() override;
    void SetMusicVolume(float volume) override;
    void UpdateMusic() override;

private:
    std::unique_ptr<raylibcpp::Window> _window;
    std::optional<raylibcpp::Texture> _texture;
    std::optional<raylibcpp::Font> _font;
    std::optional<raylibcpp::SoundWrap> _sound;
    std::optional<raylibcpp::MusicWrap> _music;
    bool _initialized = false;
};
