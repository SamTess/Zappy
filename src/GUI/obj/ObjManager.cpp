/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ObjManager implementation
*/

#include "ObjManager.hpp"
#include <iostream>

ObjManager::~ObjManager() {
    m_objFiles.clear();
}

int ObjManager::addObjFile(std::shared_ptr<IGraphicsLib> graphics, const std::string& path) {
    int id = m_nextId++;
    try {
        m_objFiles[id] = std::make_unique<ObjFile>(graphics, path);
        return id;
    } catch (const std::exception& e) {
        std::cerr << "Erreur lors du chargement du modèle: " << e.what() << std::endl;
        return -1;
    }
}

void ObjManager::deleteObj(int id) {
    if (m_objFiles.find(id) != m_objFiles.end()) {
        m_objFiles.erase(id);
    } else {
        std::cerr << "Tentative de suppression d'un modèle inexistant (ID: " << id << ")" << std::endl;
    }
}

void ObjManager::renderAll(std::shared_ptr<IGraphicsLib> graphics) const {
    for (const auto& pair : m_objFiles) {
        // Par défaut, on affiche le modèle à sa position (0,0,0)
        pair.second->display(graphics, {0, 0, 0}, 1.0f);
    }
}

ObjFile* ObjManager::getObj(int id) {
    if (m_objFiles.find(id) != m_objFiles.end()) {
        return m_objFiles[id].get();
    }
    return nullptr;
}