/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Mock pour IGraphicsLib utilisé dans les tests unitaires
*/

#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../../../src/Shared/IGraphicsLib.hpp"
#include "../../../src/Shared/Common.hpp"

class MockGraphicsLib : public IGraphicsLib {
public:
    // Attributs pour suivre les appels de méthodes
    bool wasDrawTextureCalled = false;
    int lastDrawTextureId = -1;
    int lastDrawTextureX = -1;
    int lastDrawTextureY = -1;

    bool wasDrawModelCalled = false;
    ZappyTypes::Vector3 lastDrawModelPosition = {0, 0, 0};
    float lastDrawModelScale = 0.0f;
    ZappyTypes::Color lastDrawModelColor = {0, 0, 0, 0};

    bool wasDrawModelExCalled = false;
    ZappyTypes::Vector3 lastDrawModelExPosition = {0, 0, 0};
    ZappyTypes::Vector3 lastDrawModelExRotationAxis = {0, 0, 0};
    float lastDrawModelExAngle = 0.0f;
    float lastDrawModelExScale = 0.0f;

    std::vector<std::string> lastTexturePathsLoaded;
    bool textureReadyResponse = true;

    // Constructeur par défaut
    MockGraphicsLib() = default;

    // Méthodes simulées pour IGraphicsLib

    // Fenêtre
    void InitWindow(int width, int height, const std::string& title) override {}
    void CloseWindow() override {}
    bool WindowShouldClose() override { return false; }
    void BeginDrawing() override {}
    void EndDrawing() override {}
    void ClearBackground(ZappyTypes::Color color) override {}

    // Formes 2D
    void DrawRectangle(int x, int y, int width, int height, ZappyTypes::Color color) override {}
    void DrawCircle(int centerX, int centerY, float radius, ZappyTypes::Color color) override {}
    
    // Formes 3D
    void DrawCube(ZappyTypes::Vector3 position, float width, float height, float length, ZappyTypes::Color color) override {}
    void DrawSphere(ZappyTypes::Vector3 centerPos, float radius, ZappyTypes::Color color) override {}
    void DrawPlane(ZappyTypes::Vector3 centerPos, ZappyTypes::Vector2 size, ZappyTypes::Color color) override {}
    void DrawGrid(int slices, float spacing) override {}
    void DrawCylinder(ZappyTypes::Vector3 position, float radiusTop, float radiusBottom, float height, int slices, ZappyTypes::Color color) override {}
    void DrawLine3D(ZappyTypes::Vector3 startPos, ZappyTypes::Vector3 endPos, ZappyTypes::Color color) override {}

    // Textures
    int LoadTexture2D(const std::string& path) override {
        static int nextId = 1;
        lastTexturePathsLoaded.push_back(path);
        return nextId++;
    }
    
    void DrawTexture2D(int textureId, int x, int y) override {
        wasDrawTextureCalled = true;
        lastDrawTextureId = textureId;
        lastDrawTextureX = x;
        lastDrawTextureY = y;
    }
    
    void UnloadTexture2D(int textureId) override {}
    
    bool IsTextureReady(int textureId) const override {
        return textureReadyResponse;
    }

    // Texte
    void LoadFont(const std::string& path) override {}
    void DrawText(const std::string& text, int x, int y, int size, ZappyTypes::Color color) override {}
    void UnloadFont() override {}

    // Entrées
    bool IsKeyPressed(int key) override { return false; }
    bool IsKeyDown(int key) override { return false; }
    bool IsMouseButtonPressed(int button) override { return false; }
    bool IsMouseButtonDown(int button) override { return false; }
    bool IsMouseButtonReleased(int button) override { return false; }
    int GetMouseX() override { return 0; }
    int GetMouseY() override { return 0; }
    ZappyTypes::Vector2 GetMousePosition() override { return {0, 0}; }
    float GetMouseWheelMove() override { return 0.0f; }

    // Audio (optionnel)
    void PlaySound(const std::string& path) override {}
    void StopSound() override {}
    void SetSoundVolume(float volume) override {}
    void PlayMusic(const std::string& path) override {}
    void StopMusic() override {}
    void SetMusicVolume(float volume) override {}
    void UpdateMusic() override {}

    // Caméras
    void CreateCamera2D() override {}
    void CreateCamera3D(ZappyTypes::Vector3 position, ZappyTypes::Vector3 target, ZappyTypes::Vector3 up, float fovy, int projection) override {}
    void BeginCamera3D() override {}
    void EndCamera3D() override {}

    // Textures 3D
    void LoadTexture3D(const std::string& path) override {}
    void BindTexture3D(int unit) override {}
    void UnloadTexture3D() override {}

    // Modèles 3D
    void LoadModel3D(const std::string& path) override {}
    
    void DrawModel3D(ZappyTypes::Vector3 position, float scale, ZappyTypes::Color color) override {
        wasDrawModelCalled = true;
        lastDrawModelPosition = position;
        lastDrawModelScale = scale;
        lastDrawModelColor = color;
    }
    
    void DrawModelEx(ZappyTypes::Vector3 position, ZappyTypes::Vector3 rotationAxis, float rotationAngle, float scale) override {
        wasDrawModelExCalled = true;
        lastDrawModelExPosition = position;
        lastDrawModelExRotationAxis = rotationAxis;
        lastDrawModelExAngle = rotationAngle;
        lastDrawModelExScale = scale;
    }
    
    void UnloadModel3D() override {}
    
    // Modèle avec texture
    void LoadModelWithTexture(const std::string& modelPath, const std::string& texturePath) override {
        wasLoadModelWithTextureCalled = true;
        lastModelPath = modelPath;
        lastTexturePath = texturePath;
    }

    // Variables pour les tests
    bool wasLoadModelWithTextureCalled = false;
    std::string lastModelPath;
    std::string lastTexturePath;
};
