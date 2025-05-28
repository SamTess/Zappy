/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** RayLib implementation
*/
#include <stdexcept>
#include <memory>
#include <string>
#include <vector>
#include "RayLib.hpp"
#include "../TypeAdapter.hpp"

RayLib::RayLib() {}
RayLib::~RayLib() { if (_initialized) CloseWindow(); }

void RayLib::InitWindow(int width, int height, const char* title) {
    _window = std::make_unique<raylibcpp::Window>(width, height, std::string(title));
    _initialized = true;
}
void RayLib::CloseWindow() {
    _window.reset();
    _initialized = false;
    _texture.reset();
    _font.reset();
    _sound.reset();
    _music.reset();
}
bool RayLib::WindowShouldClose() {
    return _window ? _window->shouldClose() : true;
}
void RayLib::BeginDrawing() {
    if (_window) _window->beginDrawing();
}
void RayLib::EndDrawing() {
    if (_window) _window->endDrawing();
}
void RayLib::ClearBackground(ZappyTypes::Color color) {
    if (_window) _window->clear(TypeAdapter::ToRaylib(color));
}

// Formes 2D
void RayLib::DrawRectangle(int x, int y, int width, int height, ZappyTypes::Color color) {
    raylibcpp::Shape::drawRectangle(x, y, width, height, TypeAdapter::ToRaylib(color));
}
void RayLib::DrawCircle(int centerX, int centerY, float radius, ZappyTypes::Color color) {
    raylibcpp::Shape::drawCircle(centerX, centerY, radius, TypeAdapter::ToRaylib(color));
}
// Formes 3D
void RayLib::DrawCube(ZappyTypes::Vector3 position, float width, float height, float length, ZappyTypes::Color color) {
    raylibcpp::Shape::drawCube(TypeAdapter::ToRaylib(position), width, height, length, TypeAdapter::ToRaylib(color));
}
void RayLib::DrawSphere(ZappyTypes::Vector3 centerPos, float radius, ZappyTypes::Color color) {
    raylibcpp::Shape::drawSphere(TypeAdapter::ToRaylib(centerPos), radius, TypeAdapter::ToRaylib(color));
}
void RayLib::DrawPlane(ZappyTypes::Vector3 centerPos, ZappyTypes::Vector2 size, ZappyTypes::Color color) {
    raylibcpp::Shape::drawPlane(TypeAdapter::ToRaylib(centerPos), TypeAdapter::ToRaylib(size), TypeAdapter::ToRaylib(color));
}
void RayLib::DrawGrid(int slices, float spacing) {
    raylibcpp::Shape::drawGrid(slices, spacing);
}
void RayLib::DrawCylinder(ZappyTypes::Vector3 position, float radiusTop, float radiusBottom, float height, int slices, ZappyTypes::Color color) {
    raylibcpp::Shape::drawCylinder(TypeAdapter::ToRaylib(position), radiusTop, radiusBottom, height, slices, TypeAdapter::ToRaylib(color));
}
void RayLib::DrawLine3D(ZappyTypes::Vector3 startPos, ZappyTypes::Vector3 endPos, ZappyTypes::Color color) {
    raylibcpp::Shape::drawLine3D(TypeAdapter::ToRaylib(startPos), TypeAdapter::ToRaylib(endPos), TypeAdapter::ToRaylib(color));
}

// Textures
void RayLib::LoadTexture2D(const std::string& path) {
    _texture.emplace(path);
}
void RayLib::DrawTexture2D(int x, int y) {
    if (_texture) _texture->draw(x, y);
}
void RayLib::UnloadTexture2D() {
    _texture.reset();
}

// Texte
void RayLib::LoadFont(const std::string& path) {
    _font.emplace(path);
}
void RayLib::DrawText(const std::string& text, int x, int y, int size, ZappyTypes::Color color) {
    if (_font) _font->drawText(text, x, y, size, TypeAdapter::ToRaylib(color));
}
void RayLib::UnloadFont() {
    _font.reset();
}

// Entrées
bool RayLib::IsKeyPressed(int key) {
    return raylibcpp::Input::isKeyPressed(key);
}
bool RayLib::IsKeyDown(int key) {
    return raylibcpp::Input::isKeyDown(key);
}
bool RayLib::IsMouseButtonPressed(int button) {
    return raylibcpp::Input::isMouseButtonPressed(button);
}

// Audio
void RayLib::PlaySound(const std::string& path) {
    _sound.emplace(path);
    _sound->play();
}
void RayLib::StopSound() {
    if (_sound) _sound->stop();
}
void RayLib::SetSoundVolume(float volume) {
    if (_sound) _sound->setVolume(volume);
}
void RayLib::PlayMusic(const std::string& path) {
    _music.emplace(path);
    _music->play();
}
void RayLib::StopMusic() {
    if (_music) _music->stop();
}
void RayLib::SetMusicVolume(float volume) {
    if (_music) _music->setVolume(volume);
}
void RayLib::UpdateMusic() {
    if (_music) _music->update();
}

extern "C" {
    IGraphicsLib* createGraphicsLib() {
        static RayLib instance;
        return &instance;
    }
}
