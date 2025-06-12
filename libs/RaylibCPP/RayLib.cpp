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
#include "font/Text3D.hpp"

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
    _textures.clear();
    _models.clear();
    _font.reset();
    _sound.reset();
    _music.reset();
    _texture3D.reset();
}

bool RayLib::WindowShouldClose() {
    return _window ? _window->shouldClose() : true;
}

void RayLib::setFps(int fps) {
    _window->setFps(fps);
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

void RayLib::DrawRectangle(int x, int y, int width, int height, ZappyTypes::Color color) {
    raylibcpp::Shape::drawRectangle(x, y, width, height, TypeAdapter::ToRaylib(color));
}

void RayLib::DrawCircle(int centerX, int centerY, float radius, ZappyTypes::Color color) {
    raylibcpp::Shape::drawCircle(centerX, centerY, radius, TypeAdapter::ToRaylib(color));
}

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
int RayLib::LoadTexture2D(const std::string& path) {
    try {
        int id = raylibcpp::Texture::getNextId();
        _textures[id] = std::make_unique<raylibcpp::Texture>(path);
        return id;
    } catch (const std::exception& e) {
        std::cerr << "Erreur de chargement de texture: " << e.what() << std::endl;
        return -1;
    }
}

void RayLib::DrawTexture2D(int textureId, int x, int y) {
    auto it = _textures.find(textureId);
    if (it != _textures.end())
        it->second->draw(x, y);
}

void RayLib::UnloadTexture2D(int textureId) {
    _textures.erase(textureId);
}

bool RayLib::IsTextureReady(int textureId) const {
    auto it = _textures.find(textureId);
    return it != _textures.end() && it->second->isReady();
}

// Texte
void RayLib::LoadFont(const std::string& path) {
    _font.emplace(path);
}

void RayLib::UnloadFont() {
    _font.reset();
}

void RayLib::DrawText(const std::string& text, int x, int y, int size, ZappyTypes::Color color) {
    if (_font.has_value())
        ::DrawTextEx(_font.value().get(), text.c_str(), (Vector2){static_cast<float>(x), static_cast<float>(y)}, static_cast<float>(size), 1.0f, TypeAdapter::ToRaylib(color));
    else
        ::DrawText(text.c_str(), x, y, size, TypeAdapter::ToRaylib(color));
}

void RayLib::DrawText3D(const std::string& text, ZappyTypes::Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, ZappyTypes::Color tint) {
    ::Font font = _font.has_value() ? _font.value().get() : GetFontDefault();
    raylibcpp::Text3D::DrawText3D(
        font, text, TypeAdapter::ToRaylib(position),
        fontSize, fontSpacing, lineSpacing, backface,
        TypeAdapter::ToRaylib(tint));
}

void RayLib::DrawTextWave3D(const std::string& text, ZappyTypes::Vector3 position, float fontSize, float fontSpacing, float lineSpacing, bool backface, float time, ZappyTypes::Color tint) {
    ::Font font = _font.has_value() ? _font.value().get() : GetFontDefault();
    auto config = std::make_shared<raylibcpp::WaveTextConfig>(raylibcpp::WaveTextConfig{
        .waveRange = (Vector3){ 0.45f, 0.45f, 0.45f },
        .waveSpeed = (Vector3){ 3.0f, 3.0f, 0.5f },
        .waveOffset = (Vector3){ 0.35f, 0.35f, 0.35f }
    });
    raylibcpp::Text3D::DrawTextWave3D(
        font, text, TypeAdapter::ToRaylib(position),
        fontSize, fontSpacing, lineSpacing, backface,
        config, time, TypeAdapter::ToRaylib(tint));
}

ZappyTypes::Vector3 RayLib::MeasureText3D(const std::string& text, float fontSize, float fontSpacing, float lineSpacing) {
    ::Font font = _font.has_value() ? _font.value().get() : GetFontDefault();
    Vector3 size = raylibcpp::Text3D::MeasureTextWave3D(font, text, fontSize, fontSpacing, lineSpacing);
    return TypeAdapter::FromRaylib(size);
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

bool RayLib::IsMouseButtonDown(int button) {
    return raylibcpp::Input::isMouseButtonDown(button);
}

bool RayLib::IsMouseButtonReleased(int button) {
    return raylibcpp::Input::isMouseButtonReleased(button);
}

int RayLib::GetMouseX() {
    return raylibcpp::Input::getMouseX();
}

int RayLib::GetMouseY() {
    return raylibcpp::Input::getMouseY();
}

ZappyTypes::Vector2 RayLib::GetMousePosition() {
    Vector2 pos = raylibcpp::Input::getMousePosition();
    return TypeAdapter::FromRaylib(pos);
}

float RayLib::GetMouseWheelMove() {
    return raylibcpp::Input::getMouseWheelMove();
}

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
int RayLib::LoadModel3D(const std::string& path) {
    try {
        int modelId = _nextModelId++;
        _models[modelId] = std::make_unique<raylibcpp::ModelWrap>(path);
        return modelId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle: " << path << " - " << e.what() << std::endl;
        return -1;
    }
}

void RayLib::DrawModel3D(int modelId, ZappyTypes::Vector3 position, float scale, ZappyTypes::Color color) {
    auto it = _models.find(modelId);
    if (it != _models.end() && it->second) {
        it->second->draw(TypeAdapter::ToRaylib(position), scale, TypeAdapter::ToRaylib(color));
    } else {
        std::cerr << "Tentative de dessiner un modèle inexistant (ID: " << modelId << ")" << std::endl;
    }
}

void RayLib::DrawModelEx(int modelId, ZappyTypes::Vector3 position, ZappyTypes::Vector3 rotationAxis, float rotationAngle, float scale) {
    auto it = _models.find(modelId);
    if (it != _models.end() && it->second) {
        ::DrawModelEx(
            it->second->get(),
            TypeAdapter::ToRaylib(position),
            TypeAdapter::ToRaylib(rotationAxis),
            rotationAngle,
            Vector3{scale, scale, scale},
            WHITE
        );
    } else {
        std::cerr << "Tentative de dessiner un modèle inexistant avec rotation (ID: " << modelId << ")" << std::endl;
    }
}

void RayLib::UnloadModel3D(int modelId) {
    auto it = _models.find(modelId);
    if (it != _models.end()) {
        _models.erase(it);
    } else {
        std::cerr << "Tentative de décharger un modèle inexistant (ID: " << modelId << ")" << std::endl;
    }
}

int RayLib::LoadModelWithTexture(const std::string& modelPath, const std::string& texturePath) {
    try {
        int modelId = _nextModelId++;
        _models[modelId] = std::make_unique<raylibcpp::ModelWrap>(modelPath);
        if (!texturePath.empty()) {
            Texture2D texture = ::LoadTexture(texturePath.c_str());
            if (texture.id > 0) {
                if (_models[modelId]->get().materialCount > 0) {
                    _models[modelId]->get().materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                } else {
                    ::UnloadTexture(texture);
                }
            }
        }
        return modelId;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle avec texture: " << modelPath << " - " << e.what() << std::endl;
        return -1;
    }
}

extern "C" {
    std::shared_ptr<IGraphicsLib> createGraphicsLib() {
        static std::shared_ptr<RayLib> instance = std::make_shared<RayLib>();
        return instance;
    }
}
