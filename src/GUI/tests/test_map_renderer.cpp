#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include <ctime>
#include <cstdlib>
#include "../../Shared/LibraryManager.hpp"
#include "../renderer/MapRenderer.hpp"
#include "../graphicalContext/GraphicalContext.hpp"
#include "../textureManager/ModelManager.hpp"
#include "../textureManager/ModelManagerAdapter.hpp"
#include "../cameraController/CameraController.hpp"
#include "../network/protocol/Message.hpp"
#include "../network/protocol/messageData/MapSizeData.hpp"
#include "../network/protocol/messageData/TileContentData.hpp"
#include "../network/protocol/messageData/PlayerInfoData.hpp"
#include "../network/protocol/messageData/EggData.hpp"

// Utilisez les types spécifiques au lieu d'un using namespace
namespace {
    using Zappy::MapRenderer;
    using Zappy::MapRendererObserver;
    // Déclarations correctes pour les classes non incluses dans l'espace de noms Zappy
    using ModelManager = ModelManager;
    using ResourceType = ResourceType;
}

// Test simple pour vérifier le rendu de la carte avec différentes tailles
int main() {
    // Initialiser LibraryManager et charger les bibliothèques
    LibraryManager& libraryManager = LibraryManager::getInstance();
    if (!libraryManager.loadGraphicsLib("../../../plugins/libraylibcpp.so")) {
        std::cerr << "Impossible de charger la bibliothèque graphique: " << libraryManager.getLastError() << std::endl;
        return 1;
    }
    if (!libraryManager.loadGuiLib("../../../plugins/libraygui.so")) {
        std::cerr << "Impossible de charger la bibliothèque GUI: " << libraryManager.getLastError() << std::endl;
        return 1;
    }

    auto graphics = libraryManager.getGraphicsLibPtr();
    auto gui = libraryManager.getGuiLibPtr();

    // Initialiser la fenêtre
    graphics->InitWindow(800, 600, "Test de Rendu de Carte");
    graphics->setFps(60); // Définir la fréquence d'images cible
    // Initialiser les managers
    auto& modelManagerRef = ModelManager::getInstance();
    modelManagerRef.setGraphicsLib(graphics);
    
    // Créer un shared_ptr vers l'adaptateur de ModelManager
    auto modelManagerAdapter = Zappy::ModelManagerAdapter::createShared();
    modelManagerAdapter->setGraphicsLib(graphics);

    // Créer le contexte graphique
    auto context = std::make_shared<GraphicalContext>();

    // Créer et initialiser le renderer de carte
    auto mapRenderer = std::make_shared<MapRenderer>(graphics, context, modelManagerAdapter);
    mapRenderer->initialize();

    // Créer un contrôleur de caméra pour la navigation
    auto camera = std::make_shared<CameraController>(25.0f);
    camera->init(graphics);
    // Position de la caméra ajustée pour viser le centre de la carte de 10x10
    // Avec une taille de tuile de 2.0f et un espacement de 0.5f, le centre est à (4.5, 0.0, 4.5)
    camera->position() = {4.5f, 10.0f, 10.0f};
    camera->target() = {4.5f, 0.0f, 4.5f};

    // Simuler des données de carte
    // Créer une petite carte de test (10x10)
    // Puisque les méthodes updateMapSize et updateTileContent sont privées,
    // nous allons créer un message et utiliser updateContext
    Message mapSizeMessage;
    std::shared_ptr<MapSizeData> mapSizeData = std::make_shared<MapSizeData>(20, 20);
    // La méthode setStructuredData n'existe pas dans Message, utilisons setData à la place
    mapSizeMessage.setData(mapSizeData);
    context->updateContext(mapSizeMessage);
    mapRenderer->setTileSpacing(0.5f);
    mapRenderer->setTileSize(2.0f);
    // Ajouter quelques ressources aléatoires sur la carte
    // Utiliser une seed unique pour toute la génération
    unsigned int mainSeed = static_cast<unsigned int>(time(nullptr));
    std::srand(mainSeed);

    for (int y = 0; y < 10; y++) {
        for (int x = 0; x < 10; x++) {
            // Utiliser std::rand() qui est thread-safe dans ce contexte mono-thread
            int food = std::rand() % 5;
            int linemate = std::rand() % 3;
            int deraumere = std::rand() % 2;
            int sibur = std::rand() % 2;
            int mendiane = std::rand() % 1;
            int phiras = std::rand() % 1;
            int thystame = 0;
            if (x == 5 && y == 5) // Position spéciale au centre
                thystame = 3;

            Message tileMessage;
            std::shared_ptr<TileContentData> tileData = std::make_shared<TileContentData>(
                x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame);
            // Utiliser setData à la place de setStructuredData
            tileMessage.setData(tileData);
            context->updateContext(tileMessage);
        }
    }

    // Ajouter des joueurs sur certaines tuiles
    for (int i = 1; i <= 5; ++i) {
        int x = std::rand() % 10;
        int y = std::rand() % 10;
        int orientation = std::rand() % 4 + 1; // 1-4 pour les 4 orientations
        int level = std::rand() % 8 + 1; // Niveau entre 1 et 8
        
        Message playerMessage;
        std::shared_ptr<PlayerInfoData> playerData = std::make_shared<PlayerInfoData>(
            i, x, y, orientation, level, "Equipe" + std::to_string(i % 3 + 1));
        playerMessage.setData(playerData);
        context->updateContext(playerMessage);
        
        std::cout << "Joueur " << i << " ajouté à la position (" << x << ", " << y << ")" << std::endl;
    }
    
    // Ajouter des œufs sur certaines tuiles
    for (int i = 1; i <= 3; ++i) {
        int x = std::rand() % 10;
        int y = std::rand() % 10;
        int playerId = i + 10; // ID des joueurs qui ont pondu les œufs
        
        Message eggMessage;
        std::shared_ptr<EggData> eggData = std::make_shared<EggData>(
            i, playerId, x, y);
        eggMessage.setData(eggData);
        context->updateContext(eggMessage);
        
        std::cout << "Œuf " << i << " ajouté à la position (" << x << ", " << y << ")" << std::endl;
    }

    // Boucle principale
    float currentZoomLevel = 1.0f;
    int currentMapSize = 10;
    bool resizeMap = false;

    while (!graphics->WindowShouldClose()) {
        // Mettre à jour la caméra
        camera->update(graphics);
        // Gérer le zoom avec la molette de la souris
        
        float mouseWheel = graphics->GetMouseWheelMove();
        if (mouseWheel != 0.0f) {
            currentZoomLevel += mouseWheel * 0.1f;
            currentZoomLevel = std::max(0.5f, std::min(5.0f, currentZoomLevel));
            mapRenderer->setZoomLevel(currentZoomLevel);

            // Ajuster aussi la hauteur de la caméra
            ZappyTypes::Vector3& position = camera->position();
            position.y = 10.0f / currentZoomLevel;
        }

        // Gérer le redimensionnement de la carte avec les touches R et T
        if (graphics->IsKeyPressed(ZappyTypes::KeyboardKey::Z_KEY_R)) {
            // Augmenter la taille de la carte
            currentMapSize = std::min(100, currentMapSize + 10);
            resizeMap = true;
        }
        if (graphics->IsKeyPressed(ZappyTypes::KeyboardKey::Z_KEY_T)) {
            // Diminuer la taille de la carte
            currentMapSize = std::max(5, currentMapSize - 5);
            resizeMap = true;
        }

        if (resizeMap) {
            // Recréer la carte avec la nouvelle taille
            Message newMapSizeMessage;
            std::shared_ptr<MapSizeData> newMapSizeData =
                std::make_shared<MapSizeData>(currentMapSize, currentMapSize);
            // Utiliser setData à la place de setStructuredData
            newMapSizeMessage.setData(newMapSizeData);
            context->updateContext(newMapSizeMessage);

            // Ajouter des ressources aléatoires
            // Réinitialiser la seed pour la nouvelle génération
            std::srand(static_cast<unsigned int>(time(nullptr)));

            for (int y = 0; y < currentMapSize; y++) {
                for (int x = 0; x < currentMapSize; x++) {
                    int food = std::rand() % 5;
                    int linemate = std::rand() % 3;
                    int deraumere = std::rand() % 2;
                    int sibur = std::rand() % 2;
                    int mendiane = std::rand() % 1;
                    int phiras = std::rand() % 1;
                    int thystame = 0;
                    if (x == currentMapSize/2 && y == currentMapSize/2)
                        thystame = 3;

                    Message tileMessage;
                    std::shared_ptr<TileContentData> tileData = std::make_shared<TileContentData>(
                        x, y, food, linemate, deraumere, sibur, mendiane, phiras, thystame);
                    // Utiliser setData à la place de setStructuredData
                    tileMessage.setData(tileData);
                    context->updateContext(tileMessage);
                }
            }
            
            // Ajouter à nouveau des joueurs et des œufs lors du redimensionnement
            for (int i = 1; i <= 5; ++i) {
                int x = std::rand() % currentMapSize;
                int y = std::rand() % currentMapSize;
                int orientation = std::rand() % 4 + 1;
                int level = std::rand() % 8 + 1;
                
                Message playerMessage;
                std::shared_ptr<PlayerInfoData> playerData = std::make_shared<PlayerInfoData>(
                    i, x, y, orientation, level, "Equipe" + std::to_string(i % 3 + 1));
                playerMessage.setData(playerData);
                context->updateContext(playerMessage);
            }
            
            for (int i = 1; i <= 3; ++i) {
                int x = std::rand() % currentMapSize;
                int y = std::rand() % currentMapSize;
                int playerId = i + 10;
                
                Message eggMessage;
                std::shared_ptr<EggData> eggData = std::make_shared<EggData>(
                    i, playerId, x, y);
                eggMessage.setData(eggData);
                context->updateContext(eggMessage);
            }

            // Mettre à jour la position de la caméra pour voir la carte complète
            // Le centre de la carte est maintenant à (currentMapSize-1)/2.0f * (tileSize + spacing)
            float centerX = (currentMapSize-1)/2.0f * (2.0f + 0.5f);
            float centerZ = (currentMapSize-1)/2.0f * (2.0f + 0.5f);
            camera->position() = {centerX, 15.0f, centerZ + currentMapSize * (2.0f + 0.5f) * 0.75f};
            camera->target() = {centerX, 0.0f, centerZ};

            resizeMap = false;
        }

        // Rendu
        graphics->BeginDrawing();
        graphics->ClearBackground({20, 20, 40, 255}); // Fond bleu foncé

        // Préparer le rendu 3D
        // Utiliser la caméra du CameraController
        ZappyTypes::Vector3 cameraPos = camera->position();
        ZappyTypes::Vector3 cameraTarget = camera->target();
        ZappyTypes::Vector3 up = {0.0f, 1.0f, 0.0f};
        
        // Configurer la caméra 3D avant de commencer le rendu
        graphics->CreateCamera3D(cameraPos, cameraTarget, up, 45.0f, 0);
        graphics->BeginCamera3D();
        
        // Rendu de la carte
        mapRenderer->render();

        // Terminer le rendu 3D
        graphics->EndCamera3D();

        // Afficher les instructions
        graphics->DrawText("R: Augmenter taille | T: Diminuer taille | Molette: Zoom", 10, 10, 20, {200, 200, 255, 255});
        graphics->DrawText(("Taille de la carte: " + std::to_string(currentMapSize) + "x" + std::to_string(currentMapSize)).c_str(),
                          10, 40, 20, {200, 200, 255, 255});
        graphics->DrawText(("Zoom: " + std::to_string(currentZoomLevel)).c_str(),
                          10, 70, 20, {200, 200, 255, 255});
        graphics->DrawText("Joueurs et oeufs affichés avec zoom >= 2.0", 10, 100, 20, {200, 255, 200, 255});

        graphics->EndDrawing();
    }

    // Nettoyage
    graphics->CloseWindow();

    return 0;
}
