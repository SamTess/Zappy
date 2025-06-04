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

ModelManager::ModelManager()
    : m_showModel(true), m_objManager(std::make_unique<ObjManager>()) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

int ModelManager::GetRandomValue(int min, int max) {
    return min + (std::rand() % (max - min + 1));
}

int ModelManager::addObjFile(std::shared_ptr<IGraphicsLib> graphics, const std::string& path) {
    return m_objManager->addObjFile(graphics, path);
}

void ModelManager::deleteObj(int id) {
    m_objManager->deleteObj(id);
}

void ModelManager::renderAll(std::shared_ptr<IGraphicsLib> graphics) const {
    if (m_showModel) {
        m_objManager->renderAll(graphics);
    }
}

ObjFile* ModelManager::getObj(int id) {
    return m_objManager->getObj(id);
}
