/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** User Interface implementation
*/

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <deque>
#include "../shared/GameData.hpp"
#include "../../Shared/IGuiLib.hpp"

namespace GUI {

class UserInterface {
public:
    UserInterface(std::shared_ptr<IGuiLib> guiLib);
    ~UserInterface() = default;

    // Initialisation des différents panneaux de l'interface
    void initialize(int screenWidth, int screenHeight);
    
    // Méthode principale de rendu
    void render();
    
    // Mise à jour des données
    void updateData(const GameData& gameData);
    
    // Gestion des logs
    void addLogMessage(const std::string& message);
    void clearLogs();
    
    // Sélection de case
    void setSelectedTile(int x, int y);
    
    // Gestion des broadcasts
    void addBroadcast(const std::string& team, const std::string& message);
    
    // Changement de vue
    void setViewMode(int mode);
    
private:
    // Références aux bibliothèques
    std::shared_ptr<IGuiLib> m_guiLib;
    
    // Dimensions de l'écran
    int m_screenWidth;
    int m_screenHeight;
    
    // Rectangles de définition des panneaux
    ZappyTypes::Rectangle m_logsPanelRect;
    ZappyTypes::Rectangle m_infoPanelRect;
    ZappyTypes::Rectangle m_tileInfoRect;
    ZappyTypes::Rectangle m_playerInfoRect;
    ZappyTypes::Rectangle m_broadcastsRect;
    ZappyTypes::Rectangle m_controlsRect;
    ZappyTypes::Rectangle m_timeInfoRect;
    
    // Positions de défilement pour les panneaux scrollables
    ZappyTypes::Vector2 m_scrollPos;
    ZappyTypes::Vector2 m_tileScrollPos;
    ZappyTypes::Vector2 m_playerScrollPos;
    ZappyTypes::Vector2 m_broadcastScrollPos;
    
    // État de l'interface
    bool m_showLogs;
    bool m_showTileInfo;
    bool m_showPlayerInfo;
    bool m_showBroadcasts;
    
    // Données de jeu
    GameData m_gameData;
    
    // Position de la case sélectionnée
    struct {
        int x;
        int y;
        bool selected;
    } m_selectedTile;
    
    // Liste des logs (limitée à une certaine taille)
    std::deque<std::string> m_logs;
    size_t m_maxLogs;
    
    // Liste des broadcasts récents
    struct Broadcast {
        std::string team;
        std::string message;
        float timeLeft; // Temps restant avant disparition
    };
    std::deque<Broadcast> m_broadcasts;
    
    // Mode de vue actuel
    int m_currentViewMode;
    std::vector<std::string> m_viewModes;

    // Méthodes de rendu pour chaque panneau
    void renderSidebar();
    void renderLogsPanel();
    void renderTileInfoPanel();
    void renderPlayerInfoPanel();
    void renderBroadcastsPanel();
    void renderControlsPanel();
    void renderTimeInfoPanel();
};

} // namespace GUI
