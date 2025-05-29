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
#include <iostream>
#include "RayLib.hpp"
#include "../TypeAdapter.hpp"

RayLib::RayLib() {}

RayLib::~RayLib() {
    if (_initialized)
        CloseWindow();
}

void RayLib::InitWindow(int width, int height, const std::string& title) {
    _window = std::make_unique<raylibcpp::Window>(width, height, title);
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
    if (_window)
        _window->beginDrawing();
}

void RayLib::EndDrawing() {
    if (_window)
    _window->endDrawing();
}

void RayLib::ClearBackground(ZappyTypes::Color color) {
    if (_window)
        _window->clear(TypeAdapter::ToRaylib(color));
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
    if (_texture)
        _texture->draw(x, y);
}

void RayLib::UnloadTexture2D() {
    _texture.reset();
}

// Texte
void RayLib::LoadFont(const std::string& path) {
    _font.emplace(path);
}

void RayLib::DrawText(const std::string& text, int x, int y, int size, ZappyTypes::Color color) {
    if (_font)
        _font->drawText(text, x, y, size, TypeAdapter::ToRaylib(color));
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
    if (_sound)
        _sound->stop();
}

void RayLib::SetSoundVolume(float volume) {
    if (_sound)
        _sound->setVolume(volume);
}

void RayLib::PlayMusic(const std::string& path) {
    _music.emplace(path);
    _music->play();
}

void RayLib::StopMusic() {
    if (_music)
        _music->stop();
}

void RayLib::SetMusicVolume(float volume) {
    if (_music)
        _music->setVolume(volume);
}

void RayLib::UpdateMusic() {
    if (_music)
        _music->update();
}

// Caméras
void RayLib::CreateCamera2D() {
    _camera2D.emplace();
}

void RayLib::CreateCamera3D(ZappyTypes::Vector3 position, ZappyTypes::Vector3 target,
    ZappyTypes::Vector3 up, float fovy, int projection) {
    try {
        Vector3 rayPos = TypeAdapter::ToRaylib(position);
        Vector3 rayTarget = TypeAdapter::ToRaylib(target);
        Vector3 rayUp = TypeAdapter::ToRaylib(up);
        _camera3D = raylibcpp::Camera3DWrap(rayPos, rayTarget, rayUp, fovy, projection);
    } catch (const std::exception& e) {
        std::cerr << "Erreur création caméra 3D: " << e.what() << std::endl;
        throw;
    }
}

void RayLib::BeginCamera3D() {
    if (_camera3D.has_value()) {
        try {
            _camera3D->beginMode();
        } catch (const std::exception& e) {
            std::cerr << "Erreur BeginCamera3D: " << e.what() << std::endl;
        }
    }
}

void RayLib::EndCamera3D() {
    if (_camera3D.has_value()) {
        try {
            _camera3D->endMode();
        } catch (const std::exception& e) {
            std::cerr << "Erreur EndCamera3D: " << e.what() << std::endl;
        }
    }
}

// Textures 3D
void RayLib::LoadTexture3D(const std::string& path) {
    _texture3D.emplace(path);
}

void RayLib::BindTexture3D(int unit) {
    if (_texture3D)
        _texture3D->bind(unit);
}

void RayLib::UnloadTexture3D() {
    _texture3D.reset();
}

// Modèles 3D
void RayLib::LoadModel3D(const std::string& path) {
    _model3D.emplace(path);
}

void RayLib::DrawModel3D(ZappyTypes::Vector3 position, float scale, ZappyTypes::Color color) {
    if (_model3D)
        _model3D->draw(TypeAdapter::ToRaylib(position), scale, TypeAdapter::ToRaylib(color));
}

void RayLib::UnloadModel3D() {
    _model3D.reset();
}

extern "C" {
    std::shared_ptr<IGraphicsLib> createGraphicsLib() {
        static std::shared_ptr<RayLib> instance = std::make_shared<RayLib>();
        return instance;
    }
}
