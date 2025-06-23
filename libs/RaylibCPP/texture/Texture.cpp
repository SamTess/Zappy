/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib texture encapsulation
*/

#include "Texture.hpp"
#include <raylib.h>
#include <rlgl.h>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <string>

namespace raylibcpp {

int Texture::nextId = 1;

Texture::Texture(const std::string& path) {
    texture = LoadTexture(path.c_str());
}

Texture::~Texture() {
    if (texture.id > 0 && IsWindowReady()) {
        UnloadTexture(texture);
        texture.id = 0;
    }
}

void Texture::draw(int x, int y) const {
    DrawTexture(texture, x, y, WHITE);
}

Texture2D Texture::get() const {
    return texture;
}

bool Texture::isReady() const {
    return texture.id != 0;
}

int Texture::getNextId() {
    return nextId++;
}

Texture3DWrap::Texture3DWrap(const std::string& path) {
    texture = LoadTexture(path.c_str());
    if (texture.id == 0)
        throw std::runtime_error("Erreur chargement texture: " + path);
}

Texture3DWrap::~Texture3DWrap() {
    if (texture.id > 0 && IsWindowReady()) {
        UnloadTexture(texture);
        texture.id = 0;
    }
}

void Texture3DWrap::bind(int unit) const {
    rlActiveTextureSlot(unit);
    rlEnableTexture(texture.id);
}

Texture2D& Texture3DWrap::get() {
    return texture;
}

ModelWrap::ModelWrap(const std::string& objPath) {
    model = LoadModel(objPath.c_str());
    if (model.meshCount == 0)
        throw std::runtime_error("Erreur chargement model: " + objPath);
    bool hasTextures = false;
    for (int i = 0; i < model.materialCount; i++) {
        if (model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id > 0) {
            hasTextures = true;
            break;
        }
    }

    if (!hasTextures && model.materialCount > 0) {
        std::cout << "Aucune texture chargée automatiquement pour: " << objPath << std::endl;
    }
}

ModelWrap::~ModelWrap() {
    if (model.meshes != nullptr && model.meshCount > 0) {
        UnloadModel(model);
        model.meshes = nullptr;
        model.meshCount = 0;
    }
}

void ModelWrap::draw(Vector3 position, float scale, Color tint) const {
    DrawModel(model, position, scale, tint);
}

Model& ModelWrap::get() {
    return model;
}

}  // namespace raylibcpp
