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

Texture::Texture(const std::string& path) {
    texture = LoadTexture(path.c_str());
}

Texture::~Texture() {
    UnloadTexture(texture);
}

void Texture::draw(int x, int y) const {
    DrawTexture(texture, x, y, WHITE);
}

Texture2D Texture::get() const {
    return texture;
}

Texture3DWrap::Texture3DWrap(const std::string& path) {
    texture = LoadTexture(path.c_str());
    if (texture.id == 0)
        throw std::runtime_error("Erreur chargement texture: " + path);
}

Texture3DWrap::~Texture3DWrap() {
    UnloadTexture(texture);
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
    UnloadModel(model);
}

void ModelWrap::draw(Vector3 position, float scale, Color tint) const {
    DrawModel(model, position, scale, tint);
}

Model& ModelWrap::get() {
    return model;
}

ObjFile::ObjFile(const std::string& objPath) {
    load(objPath);
}

ObjFile::~ObjFile() {
    UnloadModel(model);
}

void ObjFile::load(const std::string& objPath) {
    model = LoadModel(objPath.c_str());
    if (model.meshCount == 0)
        throw std::runtime_error("Erreur chargement model: " + objPath);
    
    filePath = objPath;
    
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

const std::string& ObjFile::path() const {
    return filePath;
}

void ObjFile::display(Vector3 position, float scale, Color tint) const {
    DrawModel(model, position, scale, tint);
}

Model& ObjFile::getModel() {
    return model;
}

ObjManager::~ObjManager() {
    objFiles.clear();
}

int ObjManager::addObjFile(const std::string& path) {
    int id = nextId++;
    try {
        objFiles[id] = std::make_unique<ObjFile>(path);
        return id;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle: " << e.what() << std::endl;
        return -1;
    }
}

void ObjManager::deleteObj(int id) {
    if (objFiles.find(id) != objFiles.end()) {
        objFiles.erase(id);
    } else {
        std::cerr << "Tentative de suppression d'un modèle inexistant (ID: " << id << ")" << std::endl;
    }
}

void ObjManager::renderAll() const {
    for (const auto& pair : objFiles) {
        pair.second->display({0, 0, 0});
    }
}

ObjFile* ObjManager::getObj(int id) {
    if (objFiles.find(id) != objFiles.end()) {
        return objFiles[id].get();
    }
    return nullptr;
}

}  // namespace raylibcpp
