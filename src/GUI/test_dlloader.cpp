/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** test_dlloader
*/

#include <memory>
#include <iostream>
#include "../Shared/LibraryManager.hpp"

int main() {
    auto& manager = LibraryManager::getInstance();
    std::cout << "Chargement des bibliothèques..." << std::endl;
    if (!manager.loadGraphicsLib("../../plugins/libraylibcpp.so")) {
        std::cerr << "Erreur de chargement de RayLib: " << manager.getLastError() << std::endl;
        return 1;
    }
    if (!manager.loadGuiLib("../../plugins/libraygui.so")) {
        std::cerr << "Erreur de chargement de RayGUI: " << manager.getLastError() << std::endl;
        return 1;
    }
    auto& graphics = manager.getGraphicsLib();
    auto& gui = manager.getGuiLib();
    std::cout << "Initialisation de la fenêtre..." << std::endl;
    graphics.InitWindow(800, 600, "Test DLLoader Zappy 3D");
    std::cout << "Création de la caméra 3D..." << std::endl;
    graphics.CreateCamera3D({10.0f, 10.0f, 10.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 45.0f, 0);
    std::cout << "Démarrage de la boucle principale..." << std::endl;
    while (!graphics.WindowShouldClose()) {
        graphics.BeginDrawing();
        graphics.ClearBackground({200, 200, 255, 255});
        graphics.BeginCamera3D();
        graphics.DrawPlane({0, 0, 0}, {10, 10}, {180, 180, 180, 255});
        graphics.DrawSphere({0, 1, 0}, 1.0f, {255, 50, 50, 255});
        graphics.DrawGrid(10, 1.0f);
        graphics.EndCamera3D();
        graphics.DrawText("Scène 3D avec caméra", 20, 20, 20, {0, 0, 0, 255});
        gui.DrawButton(600, 20, 180, 30, "Bouton test");
        graphics.EndDrawing();
    }
    std::cout << "Fermeture de l'application..." << std::endl;
    graphics.CloseWindow();
    return 0;
}
