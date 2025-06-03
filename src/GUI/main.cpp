/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Main
*/

#include <iostream>
#include <thread>
#include <chrono>
#include "parsing/ParsingCLI.hpp"
#include "../Shared/LibraryManager.hpp"
#include "Constants.hpp"
#include "ModelManager.hpp"
#include "CameraController.hpp"
#include "UIRenderer.hpp"
#include "Renderer.hpp"

int main(int, char**)
{
    try {
        ParsingCLI parser(true);
        
        auto& libraryManager = LibraryManager::getInstance();

        if (!libraryManager.loadGraphicsLib("plugins/libraylibcpp.so")) {
            std::cerr << "Erreur de chargement de la bibliothèque graphique: " << libraryManager.getLastError() << std::endl;
            return 84;
        }
        
        if (!libraryManager.loadGuiLib("plugins/libraygui.so")) {
            std::cerr << "Erreur de chargement de la bibliothèque GUI: " << libraryManager.getLastError() << std::endl;
            return 84;
        }

        auto& graphics = libraryManager.getGraphicsLib();
        auto& gui = libraryManager.getGuiLib();

        Renderer renderer;
        renderer.init(graphics);
        
        ModelManager modelManager;
        if (!modelManager.loadGirlModel(graphics)) {
            std::cerr << "Impossible de charger le modèle 3D de la fille" << std::endl;
        } else {
            modelManager.paths().clear();
            modelManager.paths().push_back("assets/models/14-girl-obj/girl.obj");
            modelManager.currentIndex() = 0;
            modelManager.initializeRandomModels(graphics);
        }
        
        CameraController camera;
        camera.init(graphics);
        
        UIRenderer uiRenderer;
        
        while (!graphics.WindowShouldClose()) {
            camera.update(graphics);
            renderer.render(graphics, gui, modelManager, camera, uiRenderer);
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
        
        graphics.CloseWindow();
        
    } catch (const AException &e) {
        std::cerr << e.getFormattedMessage() << std::endl;
        return 84;
    } catch (const std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 84;
    } catch (...) {
        std::cerr << "An unexpected error occurred." << std::endl;
        return 84;
    }
    return 0;
}
