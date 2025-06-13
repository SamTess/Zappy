/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Test temporaire pour l'interface utilisateur
*/

#include <iostream>
#include <string>
#include <memory>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>

#include "../Shared/LibraryManager.hpp"
#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/IGuiLib.hpp"
#include "ui/UserInterface.hpp"
#include "shared/GameData.hpp"

// Largeur et hauteur par défaut pour la fenêtre
const int DEFAULT_WIDTH = 1280;
const int DEFAULT_HEIGHT = 720;

/**
 * Fonction pour générer des données de test aléatoires
 */
void generateTestData(GUI::GameData& gameData) {
    std::srand(std::time(nullptr));

    // Configuration de base de la carte
    gameData.mapWidth = 20;
    gameData.mapHeight = 20;
    gameData.time = 0.0f;
    gameData.frequency = 100;
    gameData.tick = 0;

    // Création d'équipes
    GUI::Team team1 = {"Team1", 5};
    GUI::Team team2 = {"Team2", 3};
    GUI::Team team3 = {"Team3", 7};
    gameData.teams.push_back(team1);
    gameData.teams.push_back(team2);
    gameData.teams.push_back(team3);

    // Ajout de joueurs
    for (int i = 0; i < 10; i++) {
        GUI::Player player;
        player.id = i + 1;
        player.team = gameData.teams[i % gameData.teams.size()].name;
        player.x = rand() % gameData.mapWidth;
        player.y = rand() % gameData.mapHeight;
        player.orientation = (rand() % 4) + 1;
        player.level = (rand() % 8) + 1;
        player.inventory.food = rand() % 20;
        player.inventory.linemate = rand() % 5;
        player.inventory.deraumere = rand() % 3;
        player.inventory.sibur = rand() % 3;
        player.inventory.mendiane = rand() % 2;
        player.inventory.phiras = rand() % 2;
        player.inventory.thystame = rand() % 1;

        gameData.players.push_back(player);
    }

    // Ajout de ressources sur des cases
    for (int i = 0; i < 30; i++) {
        GUI::Tile tile;
        tile.x = rand() % gameData.mapWidth;
        tile.y = rand() % gameData.mapHeight;
        tile.food = rand() % 5;
        tile.linemate = rand() % 3;
        tile.deraumere = rand() % 2;
        tile.sibur = rand() % 2;
        tile.mendiane = rand() % 1;
        tile.phiras = rand() % 1;
        tile.thystame = (rand() % 10 == 0) ? 1 : 0; // Thystame rare

        gameData.tiles.push_back(tile);
    }
}

/**
 * Fonction pour simuler des mises à jour aléatoires des données
 */
void updateTestData(GUI::GameData& gameData, float deltaTime, std::shared_ptr<GUI::UserInterface> ui) {
    // Mise à jour du temps
    gameData.time += deltaTime;
    gameData.tick++;

    // Événements aléatoires
    static float eventTimer = 0.0f;
    static float moveTimer = 0.0f;
    static float broadcastTimer = 0.0f;

    // Traiter les événements aléatoires moins fréquemment
    eventTimer += deltaTime;
    if (eventTimer > 3.0f) {
        eventTimer = 0.0f;

        int event = rand() % 4;
        switch (event) {
            case 0: // Nouveaux joueurs
                if (gameData.players.size() < 20 && !gameData.teams.empty()) {
                    GUI::Player player;
                    player.id = gameData.players.size() + 10;
                    player.team = gameData.teams[rand() % gameData.teams.size()].name;
                    player.x = rand() % gameData.mapWidth;
                    player.y = rand() % gameData.mapHeight;
                    player.orientation = (rand() % 4) + 1;
                    player.level = 1;
                    gameData.players.push_back(player);
                    
                    std::string message = "Nouveau joueur " + std::to_string(player.id) + 
                                         " de l'équipe " + player.team + " a rejoint la partie";
                    ui->addLogMessage(message);
                }
                break;

            case 1: // Évolution de joueur
                if (!gameData.players.empty()) {
                    int playerIndex = rand() % gameData.players.size();
                    if (gameData.players[playerIndex].level < 8) {
                        gameData.players[playerIndex].level++;
                        gameData.players[playerIndex].isIncantating = true;
                        
                        std::string message = "Joueur " + std::to_string(gameData.players[playerIndex].id) + 
                                             " passe au niveau " + std::to_string(gameData.players[playerIndex].level);
                        ui->addLogMessage(message);
                        ui->addBroadcast(gameData.players[playerIndex].team, 
                                        "Je suis niveau " + std::to_string(gameData.players[playerIndex].level) + " maintenant!");
                        
                        // Désactiver l'incantation après un délai
                        std::thread([&gameData, playerIndex]() {
                            std::this_thread::sleep_for(std::chrono::seconds(1));
                            if (playerIndex < gameData.players.size()) {
                                gameData.players[playerIndex].isIncantating = false;
                            }
                        }).detach();
                    }
                }
                break;

            case 2: // Mettre à jour les ressources d'une case
                if (!gameData.tiles.empty()) {
                    int tileIndex = rand() % gameData.tiles.size();
                    gameData.tiles[tileIndex].food = rand() % 5;
                    gameData.tiles[tileIndex].linemate = rand() % 3;
                    gameData.tiles[tileIndex].deraumere = rand() % 2;
                    
                    std::string message = "Ressources mises à jour sur la case (" + 
                                         std::to_string(gameData.tiles[tileIndex].x) + ", " + 
                                         std::to_string(gameData.tiles[tileIndex].y) + ")";
                    ui->addLogMessage(message);
                }
                break;

            case 3: // Mort d'un joueur
                if (gameData.players.size() > 5) { // Garder quelques joueurs
                    int playerIndex = rand() % gameData.players.size();
                    std::string team = gameData.players[playerIndex].team;
                    int id = gameData.players[playerIndex].id;
                    gameData.players.erase(gameData.players.begin() + playerIndex);
                    
                    std::string message = "Joueur " + std::to_string(id) + " de l'équipe " + team + " est mort";
                    ui->addLogMessage(message);
                }
                break;
        }
    }

    // Déplacer les joueurs de temps en temps
    moveTimer += deltaTime;
    if (moveTimer > 0.3f) {
        moveTimer = 0.0f;
        
        for (auto& player : gameData.players) {
            if (rand() % 5 == 0) { // 20% de chance de bouger
                int direction = rand() % 4;
                switch (direction) {
                    case 0: // nord
                        player.y = (player.y - 1 + gameData.mapHeight) % gameData.mapHeight;
                        player.orientation = 1;
                        break;
                    case 1: // est
                        player.x = (player.x + 1) % gameData.mapWidth;
                        player.orientation = 2;
                        break;
                    case 2: // sud
                        player.y = (player.y + 1) % gameData.mapHeight;
                        player.orientation = 3;
                        break;
                    case 3: // ouest
                        player.x = (player.x - 1 + gameData.mapWidth) % gameData.mapWidth;
                        player.orientation = 4;
                        break;
                }
            }
        }
    }

    // Générer des broadcasts occasionnels
    broadcastTimer += deltaTime;
    if (broadcastTimer > 5.0f) {
        broadcastTimer = 0.0f;
        
        if (!gameData.teams.empty()) {
            std::string team = gameData.teams[rand() % gameData.teams.size()].name;
            
            std::vector<std::string> messages = {
                "J'ai besoin de nourriture !",
                "Ressources trouvées ici !",
                "Rencontre au centre de la carte",
                "Besoin d'aide pour incantation",
                "Attention danger ici !",
                "Rejoignez-moi pour monter de niveau",
                "Je vais bien, ne vous inquiétez pas",
                "Qui a vu des pierres précieuses ?"
            };
            
            std::string message = messages[rand() % messages.size()];
            ui->addBroadcast(team, message);
        }
    }
}

/**
 * Fonction principale pour tester l'interface utilisateur
 */
int main() {
    std::cout << "Démarrage du test de l'interface utilisateur..." << std::endl;
    
    // Initialisation du générateur de nombres aléatoires
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    
    // Chargement des bibliothèques
    auto& libraryManager = LibraryManager::getInstance();
    if (!libraryManager.loadGraphicsLib("plugins/libraylibcpp.so")) {
        std::cerr << "Erreur lors du chargement de la bibliothèque graphique: " 
                  << libraryManager.getLastError() << std::endl;
        return 84;
    }
    if (!libraryManager.loadGuiLib("plugins/libraygui.so")) {
        std::cerr << "Erreur lors du chargement de la bibliothèque GUI: " 
                  << libraryManager.getLastError() << std::endl;
        return 84;
    }
    
    // Récupération des pointeurs sur les bibliothèques
    std::shared_ptr<IGraphicsLib> graphics = libraryManager.getGraphicsLibPtr();
    std::shared_ptr<IGuiLib> gui = libraryManager.getGuiLibPtr();
    
    if (!graphics || !gui) {
        std::cerr << "Impossible d'obtenir les bibliothèques." << std::endl;
        return 84;
    }
    
    // Initialisation de la fenêtre
    graphics->InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Test de l'interface Zappy");
    graphics->setFps(60);
    
    // Initialisation de l'interface utilisateur
    std::shared_ptr<GUI::UserInterface> userInterface = std::make_shared<GUI::UserInterface>(gui);
    userInterface->initialize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    userInterface->addLogMessage("Interface de test initialisée");
    userInterface->addLogMessage("Utilisez les touches suivantes pour tester :");
    userInterface->addLogMessage("T : Sélectionner une tuile aléatoire");
    userInterface->addLogMessage("B : Ajouter un broadcast de test");
    userInterface->addLogMessage("L : Ajouter un message de log");
    
    // Générer des données de test
    GUI::GameData gameData;
    generateTestData(gameData);
    userInterface->updateData(gameData);
    
    // Variables pour le calcul du temps entre les frames
    auto lastFrameTime = std::chrono::high_resolution_clock::now();
    
    // Boucle principale
    while (!graphics->WindowShouldClose()) {
        // Calcul du temps écoulé depuis la dernière frame
        auto currentTime = std::chrono::high_resolution_clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;
        
        // Mise à jour des données de test
        updateTestData(gameData, deltaTime, userInterface);
        userInterface->updateData(gameData);
        
        // Gestion des interactions clavier pour les tests
        if (graphics->IsKeyPressed(ZappyTypes::Z_KEY_T)) {
            int x = rand() % gameData.mapWidth;
            int y = rand() % gameData.mapHeight;
            userInterface->setSelectedTile(x, y);
            userInterface->addLogMessage("Sélection de la case (" + std::to_string(x) + ", " + std::to_string(y) + ")");
        }
        
        if (graphics->IsKeyPressed(ZappyTypes::Z_KEY_B)) {
            std::string team = gameData.teams[rand() % gameData.teams.size()].name;
            userInterface->addBroadcast(team, "Ceci est un broadcast de test");
        }
        
        if (graphics->IsKeyPressed(ZappyTypes::Z_KEY_L)) {
            userInterface->addLogMessage("Ceci est un message de log de test");
        }
        
        if (graphics->IsKeyPressed(ZappyTypes::Z_KEY_SPACE)) {
            // Changer le mode de vue
            static int viewMode = 0;
            viewMode = (viewMode + 1) % 4;
            userInterface->setViewMode(viewMode);
            userInterface->addLogMessage("Mode de vue changé: " + std::to_string(viewMode));
        }
        
        // Rendu
        graphics->BeginDrawing();
        graphics->ClearBackground({32, 32, 64, 255});
        
        // Simuler un rendu de jeu basique (une grille)
        graphics->DrawGrid(20, 1.0f);
        
        // Rendu de l'interface utilisateur
        userInterface->render();
        
        graphics->EndDrawing();
    }
    
    graphics->CloseWindow();
    std::cout << "Test terminé." << std::endl;
    
    return 0;
}
