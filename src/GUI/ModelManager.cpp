/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManager
*/

#include "ModelManager.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <ctime>

ObjFile::ObjFile(std::shared_ptr<IGraphicsLib> graphics, const std::string& objPath)
    : m_filePath(""), m_isLoaded(false) {
    load(graphics, objPath);
}

void ObjFile::load(std::shared_ptr<IGraphicsLib> graphics, const std::string& objPath) {
    try {
        std::cout << "Chargement du modèle 3D: " << objPath << std::endl;
        graphics->LoadModel3D(objPath);
        m_isLoaded = true;
        m_filePath = objPath;
        std::cout << "Modèle 3D chargé avec succès: " << objPath << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle 3D: " << e.what() << std::endl;
        m_isLoaded = false;
    }
}

const std::string& ObjFile::path() const {
    return m_filePath;
}

void ObjFile::display(std::shared_ptr<IGraphicsLib> graphics, ZappyTypes::Vector3 position, float scale, ZappyTypes::Color color) const {
    if (m_isLoaded) {
        graphics->DrawModel3D(position, scale, color);
    }
}

ModelManager::ModelManager()
    : m_showModel(true) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

int ModelManager::GetRandomValue(int min, int max) {
    return min + (std::rand() % (max - min + 1));
}

int ModelManager::addObjFile(std::shared_ptr<IGraphicsLib> graphics, const std::string& path) {
    int id = m_nextId++;
    try {
        m_objFiles[id] = std::make_unique<ObjFile>(graphics, path);
        return id;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle: " << e.what() << std::endl;
        return -1;
    }
}

void ModelManager::deleteObj(int id) {
    if (m_objFiles.find(id) != m_objFiles.end()) {
        m_objFiles.erase(id);
    } else {
        std::cerr << "Tentative de suppression d'un modèle inexistant (ID: " << id << ")" << std::endl;
    }
}

void ModelManager::renderAll(std::shared_ptr<IGraphicsLib> graphics) const {
    for (const auto& pair : m_objFiles) {
        pair.second->display(graphics, {0, 0, 0}, 1.0f);
    }
}

ObjFile* ModelManager::getObj(int id) {
    if (m_objFiles.find(id) != m_objFiles.end()) {
        return m_objFiles[id].get();
    }
    return nullptr;
}
