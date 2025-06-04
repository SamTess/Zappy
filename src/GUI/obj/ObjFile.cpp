/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ObjFile implementation
*/

#include "ObjFile.hpp"
#include <iostream>

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