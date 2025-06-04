/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameLoop
*/

#include "GameLoop.hpp"
#include <iostream>
#include <thread>
#include <memory>
#include <string>
#include <chrono>
#include "Constants.hpp"

GameLoop::GameLoop()
    : m_host("localhost"), m_port(4242) {
}

bool GameLoop::init() {
    auto& libraryManager = LibraryManager::getInstance();

    if (!libraryManager.loadGraphicsLib("plugins/libraylibcpp.so")) {
        std::cerr << "Erreur de chargement de la bibliothèque graphique: " << libraryManager.getLastError() << std::endl;
        return false;
    }

    if (!libraryManager.loadGuiLib("plugins/libraygui.so")) {
        std::cerr << "Erreur de chargement de la bibliothèque GUI: " << libraryManager.getLastError() << std::endl;
        return false;
    }

    m_graphics = libraryManager.getGraphicsLibPtr();
    m_gui = libraryManager.getGuiLibPtr();

    m_renderer = std::make_shared<Renderer>();
    m_renderer->init(m_graphics);

    m_modelManager = std::make_shared<ObjManager>();
    int girlModelId = m_modelManager->addObjFile(m_graphics, "assets/models/14-girl-obj/girl.obj");
    if (girlModelId != -1) {
        std::cout << "Modèle girl.obj chargé avec succès en utilisant le système ObjFile (ID: " << girlModelId << ")" << std::endl;
    } else {
        std::cerr << "Impossible de charger le modèle girl.obj" << std::endl;
    }

    m_camera = std::make_shared<CameraController>();
    m_camera->init(m_graphics);
    m_uiRenderer = std::make_shared<UIRenderer>();

    return true;
}

int GameLoop::run() {
    if (!m_graphics || !m_gui || !m_renderer || !m_modelManager || !m_camera || !m_uiRenderer) {
        std::cerr << "Game components not initialized properly." << std::endl;
        return 84;
    }

    while (!m_graphics->WindowShouldClose()) {
        m_camera->update(m_graphics);
        m_renderer->render(m_graphics, m_gui, m_modelManager, m_camera, m_uiRenderer);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    m_graphics->CloseWindow();
    return 0;
}

void GameLoop::setServerInfo(const std::string& host, int port) {
    m_host = host;
    m_port = port;
}
