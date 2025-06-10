/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ObjFile implementation
*/

#include "ObjFile.hpp"
#include <iostream>
#include <string>

ObjFile::ObjFile(std::shared_ptr<IGraphicsLib> graphics, const std::string& objPath)
    : m_filePath(""), m_isLoaded(false), m_modelId(-1) {
    load(graphics, objPath);
}

ObjFile::~ObjFile() {
}

void ObjFile::load(std::shared_ptr<IGraphicsLib> graphics, const std::string& objPath) {
    try {
        m_modelId = graphics->LoadModel3D(objPath);
        m_isLoaded = (m_modelId >= 0);
        if (m_isLoaded) {
            m_filePath = objPath;
        } else {
            std::cerr << "Échec du chargement du modèle 3D: " << objPath << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle 3D: " << e.what() << std::endl;
        m_isLoaded = false;
        m_modelId = -1;
    }
}

const std::string& ObjFile::path() const {
    return m_filePath;
}

void ObjFile::display(std::shared_ptr<IGraphicsLib> graphics, ZappyTypes::Vector3 position, float scale, ZappyTypes::Color color) const {
    if (m_isLoaded && m_modelId >= 0) {
        graphics->DrawModel3D(m_modelId, position, scale, color);
    }
}
