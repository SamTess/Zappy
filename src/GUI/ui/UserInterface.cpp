/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** User Interface implementation
*/

#include <algorithm>
#include <sstream>
#include <iomanip>
#include <chrono>
#include "UserInterface.hpp"

namespace GUI {

UserInterface::UserInterface(std::shared_ptr<IGuiLib> guiLib)
    : m_guiLib(guiLib),
      m_screenWidth(800),
      m_screenHeight(600),
      m_showLogs(true),
      m_showTileInfo(true),
      m_showPlayerInfo(true),
      m_showBroadcasts(true),
      m_maxLogs(30),
      m_currentViewMode(0)
{
    m_selectedTile = {0, 0, false};
    m_viewModes = {"Vue standard", "Vue aérienne", "Vue wireframe", "Vue ressources"};
    
    // Position initiale des panneaux
    m_logsPanelRect = {0, 0, 0, 0};
    m_infoPanelRect = {0, 0, 0, 0};
    m_tileInfoRect = {0, 0, 0, 0};
    m_playerInfoRect = {0, 0, 0, 0};
    m_controlsRect = {0, 0, 0, 0};
    m_timeInfoRect = {0, 0, 0, 0};
    m_broadcastsRect = {0, 0, 0, 0};
    
    m_scrollPos = {0, 0};
    m_tileScrollPos = {0, 0};
    m_broadcastScrollPos = {0, 0};
    m_playerScrollPos = {0, 0};
}

void UserInterface::initialize(int screenWidth, int screenHeight)
{
    m_screenWidth = screenWidth;
    m_screenHeight = screenHeight;
    
    // Configuration des dimensions et positions des panneaux
    int sidebarWidth = 300;
    int bottomPanelHeight = 150;
    int topPanelHeight = 60;
    
    // Panneau latéral droit (informations et contrôles)
    m_infoPanelRect = {
        static_cast<float>(m_screenWidth - sidebarWidth), 
        0, 
        static_cast<float>(sidebarWidth), 
        static_cast<float>(m_screenHeight)
    };
    
    // Zone de logs en bas
    m_logsPanelRect = {
        0, 
        static_cast<float>(m_screenHeight - bottomPanelHeight), 
        static_cast<float>(m_screenWidth - sidebarWidth), 
        static_cast<float>(bottomPanelHeight)
    };
    
    // Informations temporelles en haut
    m_timeInfoRect = {
        0, 
        0, 
        static_cast<float>(m_screenWidth - sidebarWidth), 
        static_cast<float>(topPanelHeight)
    };
    
    // Sous-panneaux dans le panneau latéral
    m_tileInfoRect = {
        m_infoPanelRect.x + 10, 
        m_infoPanelRect.y + 10, 
        m_infoPanelRect.width - 20, 
        200
    };
    
    m_playerInfoRect = {
        m_infoPanelRect.x + 10, 
        m_tileInfoRect.y + m_tileInfoRect.height + 10, 
        m_infoPanelRect.width - 20, 
        200
    };
    
    m_broadcastsRect = {
        m_infoPanelRect.x + 10, 
        m_playerInfoRect.y + m_playerInfoRect.height + 10, 
        m_infoPanelRect.width - 20, 
        180
    };
    
    m_controlsRect = {
        m_infoPanelRect.x + 10, 
        m_broadcastsRect.y + m_broadcastsRect.height + 10, 
        m_infoPanelRect.width - 20, 
        180
    };
    
    // Ajouter un message de bienvenue dans les logs
    addLogMessage("Bienvenue dans Zappy GUI!");
    addLogMessage("Connecté au serveur");
    addLogMessage("Interface utilisateur initialisée");
    
    // Simuler quelques broadcasts pour l'exemple
    addBroadcast("Team1", "Je viens de monter niveau 2!");
    addBroadcast("Team2", "Besoin d'aide à la position (5,7)");
}

void UserInterface::render()
{
    // Rendu de tous les panneaux de l'interface
    renderTimeInfoPanel(); // Panneau supérieur avec informations de temps
    renderSidebar();       // Panneau latéral principal
    
    if (m_showLogs)
        renderLogsPanel(); // Panneau de logs en bas
    
    // Panneau d'information sur la case sélectionnée
    if (m_showTileInfo && m_selectedTile.selected)
        renderTileInfoPanel();
    
    // Panneau d'information sur les joueurs
    if (m_showPlayerInfo)
        renderPlayerInfoPanel();
    
    // Panneau des broadcasts récents
    if (m_showBroadcasts)
        renderBroadcastsPanel();
    
    // Panneau des contrôles et modes de vue
    renderControlsPanel();
}

void UserInterface::updateData(const GameData& gameData)
{
    m_gameData = gameData;
    
    // Mettre à jour les broadcasts - réduire le temps de vie
    auto currentTime = std::chrono::high_resolution_clock::now();
    static auto lastTime = currentTime;
    float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
    lastTime = currentTime;
    
    // Faire disparaître progressivement les broadcasts
    for (auto it = m_broadcasts.begin(); it != m_broadcasts.end();) {
        it->timeLeft -= deltaTime;
        if (it->timeLeft <= 0) {
            it = m_broadcasts.erase(it);
        } else {
            ++it;
        }
    }
}

void UserInterface::addLogMessage(const std::string& message)
{
    // Obtenir l'heure actuelle pour l'horodatage
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&time);
    
    std::stringstream ss;
    ss << "[" << std::setw(2) << std::setfill('0') << localTime.tm_hour << ":" 
       << std::setw(2) << std::setfill('0') << localTime.tm_min << ":" 
       << std::setw(2) << std::setfill('0') << localTime.tm_sec << "] " 
       << message;
    
    m_logs.push_front(ss.str());
    
    // Limiter le nombre de logs
    if (m_logs.size() > m_maxLogs) {
        m_logs.pop_back();
    }
}

void UserInterface::clearLogs()
{
    m_logs.clear();
    addLogMessage("Logs effacés");
}

void UserInterface::setSelectedTile(int x, int y)
{
    m_selectedTile.x = x;
    m_selectedTile.y = y;
    m_selectedTile.selected = true;
    
    std::stringstream ss;
    ss << "Sélection de la case (" << x << ", " << y << ")";
    addLogMessage(ss.str());
}

void UserInterface::addBroadcast(const std::string& team, const std::string& message)
{
    Broadcast bc;
    bc.team = team;
    bc.message = message;
    bc.timeLeft = 10.0f; // Durée d'affichage en secondes
    
    m_broadcasts.push_front(bc);
    
    // Limiter le nombre de broadcasts affichés
    if (m_broadcasts.size() > 10) {
        m_broadcasts.pop_back();
    }
    
    // Ajouter aussi aux logs
    std::stringstream ss;
    ss << "[BROADCAST:" << team << "] " << message;
    addLogMessage(ss.str());
}

void UserInterface::setViewMode(int mode)
{
    if (mode >= 0 && mode < static_cast<int>(m_viewModes.size())) {
        m_currentViewMode = mode;
        std::stringstream ss;
        ss << "Mode de vue changé: " << m_viewModes[mode];
        addLogMessage(ss.str());
    }
}

void UserInterface::renderSidebar()
{
    // Panneau principal latéral
    m_guiLib->DrawPanel(m_infoPanelRect.x, m_infoPanelRect.y, m_infoPanelRect.width, m_infoPanelRect.height);
    
    // Titre du panneau
    m_guiLib->DrawLabel(m_infoPanelRect.x + 10, m_infoPanelRect.y + 5, 
                       m_infoPanelRect.width - 20, 25, 
                       "ZAPPY - Panneau d'information");
    
    // Sous-panneaux
    renderTileInfoPanel();
    renderPlayerInfoPanel();
    renderBroadcastsPanel();
    renderControlsPanel();
}

void UserInterface::renderLogsPanel()
{
    // Panneau de logs en bas de l'écran
    m_guiLib->DrawPanel(m_logsPanelRect.x, m_logsPanelRect.y, m_logsPanelRect.width, m_logsPanelRect.height);
    
    // Titre du panneau de logs
    m_guiLib->DrawLabel(m_logsPanelRect.x + 10, m_logsPanelRect.y + 5, 
                       m_logsPanelRect.width - 20, 20, 
                       "Journal d'événements");
    
    // Bouton pour effacer les logs
    if (m_guiLib->ButtonPressed(m_logsPanelRect.x + m_logsPanelRect.width - 100, 
                              m_logsPanelRect.y + 5, 90, 20, "Effacer")) {
        clearLogs();
    }
    
    // On définit l'espace de contenu pour les logs
    ZappyTypes::Rectangle contentRect = {
        0, 0, 
        m_logsPanelRect.width - 40, 
        static_cast<float>(m_logs.size() * 20 + 10)
    };
    
    // Créer un panneau défilant pour les logs
    ZappyTypes::Rectangle view = m_guiLib->DrawScrollPanel(
        m_logsPanelRect.x + 10, 
        m_logsPanelRect.y + 30, 
        m_logsPanelRect.width - 20, 
        m_logsPanelRect.height - 40,
        "",
        contentRect,
        std::make_shared<ZappyTypes::Vector2>(m_scrollPos)
    );
    
    // Afficher chaque log dans la zone visible
    int i = 0;
    for (const auto& log : m_logs) {
        float yPos = view.y + i * 20 + 5;
        // Ne dessiner que si visible dans la zone de défilement
        if (yPos >= view.y && yPos <= view.y + view.height) {
            m_guiLib->DrawLabel(view.x + 5, yPos, view.width - 10, 20, log);
        }
        i++;
    }
}

void UserInterface::renderTileInfoPanel()
{
    // Panneau d'informations sur la case sélectionnée
    m_guiLib->DrawGroupBox(m_tileInfoRect.x, m_tileInfoRect.y, 
                         m_tileInfoRect.width, m_tileInfoRect.height, 
                         "Informations sur la case");
    
    if (m_selectedTile.selected) {
        // Trouver la case sélectionnée dans les données
        auto tileIt = std::find_if(m_gameData.tiles.begin(), m_gameData.tiles.end(),
            [this](const auto& tile) {
                return tile.x == m_selectedTile.x && tile.y == m_selectedTile.y;
            });
        
        float yOffset = m_tileInfoRect.y + 30;
        
        // Afficher les coordonnées
        std::stringstream coords;
        coords << "Position: (" << m_selectedTile.x << ", " << m_selectedTile.y << ")";
        m_guiLib->DrawLabel(m_tileInfoRect.x + 10, yOffset, 
                          m_tileInfoRect.width - 20, 20, 
                          coords.str());
        yOffset += 25;
        
        // Afficher les ressources si la case est connue
        if (tileIt != m_gameData.tiles.end()) {
            const auto& tile = *tileIt;
            
            std::stringstream resources;
            resources << "Nourriture: " << tile.food;
            m_guiLib->DrawLabel(m_tileInfoRect.x + 10, yOffset, 
                              m_tileInfoRect.width - 20, 20, 
                              resources.str());
            yOffset += 20;
            
            resources.str(""); resources << "Linemate: " << tile.linemate;
            m_guiLib->DrawLabel(m_tileInfoRect.x + 10, yOffset, 
                              m_tileInfoRect.width - 20, 20, 
                              resources.str());
            yOffset += 20;
            
            resources.str(""); resources << "Deraumere: " << tile.deraumere;
            m_guiLib->DrawLabel(m_tileInfoRect.x + 10, yOffset, 
                              m_tileInfoRect.width - 20, 20, 
                              resources.str());
            yOffset += 20;
            
            resources.str(""); resources << "Sibur: " << tile.sibur;
            m_guiLib->DrawLabel(m_tileInfoRect.x + 10, yOffset, 
                              m_tileInfoRect.width - 20, 20, 
                              resources.str());
            yOffset += 20;
            
            resources.str(""); resources << "Mendiane: " << tile.mendiane;
            m_guiLib->DrawLabel(m_tileInfoRect.x + 10, yOffset, 
                              m_tileInfoRect.width - 20, 20, 
                              resources.str());
            yOffset += 20;
            
            resources.str(""); resources << "Phiras: " << tile.phiras;
            m_guiLib->DrawLabel(m_tileInfoRect.x + 10, yOffset, 
                             m_tileInfoRect.width - 20, 20, 
                             resources.str());
            yOffset += 20;
            
            resources.str(""); resources << "Thystame: " << tile.thystame;
            m_guiLib->DrawLabel(m_tileInfoRect.x + 10, yOffset, 
                             m_tileInfoRect.width - 20, 20, 
                             resources.str());
        } else {
            m_guiLib->DrawLabel(m_tileInfoRect.x + 10, yOffset, 
                             m_tileInfoRect.width - 20, 20, 
                             "Aucune donnée disponible pour cette case");
        }
    } else {
        m_guiLib->DrawLabel(m_tileInfoRect.x + 10, m_tileInfoRect.y + 30, 
                         m_tileInfoRect.width - 20, 20, 
                         "Aucune case sélectionnée");
    }
}

void UserInterface::renderPlayerInfoPanel()
{
    // Panneau d'informations sur les joueurs
    m_guiLib->DrawGroupBox(m_playerInfoRect.x, m_playerInfoRect.y, 
                         m_playerInfoRect.width, m_playerInfoRect.height, 
                         "Joueurs");
    
    // Afficher le nombre de joueurs
    std::stringstream playerCount;
    playerCount << "Nombre de joueurs: " << m_gameData.players.size();
    m_guiLib->DrawLabel(m_playerInfoRect.x + 10, m_playerInfoRect.y + 25, 
                      m_playerInfoRect.width - 20, 20, 
                      playerCount.str());
    
    // Bouton de basculement pour l'affichage détaillé des joueurs
    bool prevShowPlayerInfo = m_showPlayerInfo;
    m_showPlayerInfo = m_guiLib->DrawToggle(
        m_playerInfoRect.x + m_playerInfoRect.width - 100, 
        m_playerInfoRect.y + 25, 
        90, 20, 
        "Détails", 
        m_showPlayerInfo
    );
    
    // Si le bouton est cliqué (état change), forcer l'état opposé
    if (m_guiLib->ButtonPressed(m_playerInfoRect.x + m_playerInfoRect.width - 100, 
                              m_playerInfoRect.y + 25, 90, 20, "")) {
        m_showPlayerInfo = !prevShowPlayerInfo;
    }
    
    if (!m_showPlayerInfo || m_gameData.players.empty()) {
        return;
    }
    
    // On définit l'espace de contenu pour la liste des joueurs
    ZappyTypes::Rectangle contentRect = {
        0, 0, 
        m_playerInfoRect.width - 40, 
        static_cast<float>(m_gameData.players.size() * 60 + 10)
    };
    
    // Créer un panneau défilant pour les joueurs
    // Augmenter la hauteur du contenu pour éviter les problèmes de défilement
    contentRect.height = std::max(contentRect.height, m_playerInfoRect.height * 2);
    
    ZappyTypes::Rectangle view = m_guiLib->DrawScrollPanel(
        m_playerInfoRect.x + 10, 
        m_playerInfoRect.y + 50, 
        m_playerInfoRect.width - 20, 
        m_playerInfoRect.height - 60,
        "",
        contentRect,
        std::make_shared<ZappyTypes::Vector2>(m_playerScrollPos)
    );
    
    // Afficher les informations pour chaque joueur
    int i = 0;
    for (const auto& player : m_gameData.players) {
        float yPos = view.y + i * 60;
        
        // Ne dessiner que si visible dans la zone de défilement
        if (yPos >= view.y && yPos <= view.y + view.height) {
            std::stringstream playerInfo;
            playerInfo << "ID: " << player.id << " | Équipe: " << player.team << " | Niveau: " << player.level;
            m_guiLib->DrawLabel(view.x + 5, yPos, view.width - 10, 20, playerInfo.str());
            
            playerInfo.str("");
            playerInfo << "Position: (" << player.x << ", " << player.y << ") | Orientation: " << player.orientation;
            m_guiLib->DrawLabel(view.x + 5, yPos + 20, view.width - 10, 20, playerInfo.str());
            
            std::string status = player.isIncantating ? "Incantation en cours" : "Normal";
            status += player.isEgg ? " (Oeuf)" : "";
            m_guiLib->DrawLabel(view.x + 5, yPos + 40, view.width - 10, 20, "État: " + status);
        }
        i++;
    }
}

void UserInterface::renderBroadcastsPanel()
{
    // Panneau des broadcasts récents
    m_guiLib->DrawGroupBox(m_broadcastsRect.x, m_broadcastsRect.y, 
                         m_broadcastsRect.width, m_broadcastsRect.height, 
                         "Broadcasts récents");
    
    // Afficher le nombre de broadcasts
    std::stringstream bcCount;
    bcCount << "Nombre de messages: " << m_broadcasts.size();
    m_guiLib->DrawLabel(m_broadcastsRect.x + 10, m_broadcastsRect.y + 25, 
                      m_broadcastsRect.width - 20, 20, 
                      bcCount.str());
    
    // Bouton de basculement pour l'affichage des broadcasts
    bool prevShowBroadcasts = m_showBroadcasts;
    m_showBroadcasts = m_guiLib->DrawToggle(
        m_broadcastsRect.x + m_broadcastsRect.width - 100, 
        m_broadcastsRect.y + 25, 
        90, 20, 
        "Afficher", 
        m_showBroadcasts
    );
    
    // Si le bouton est cliqué (état change), forcer l'état opposé
    if (m_guiLib->ButtonPressed(m_broadcastsRect.x + m_broadcastsRect.width - 100, 
                              m_broadcastsRect.y + 25, 90, 20, "")) {
        m_showBroadcasts = !prevShowBroadcasts;
    }
    
    if (!m_showBroadcasts || m_broadcasts.empty()) {
        return;
    }
    
    // On définit l'espace de contenu pour la liste des broadcasts
    ZappyTypes::Rectangle contentRect = {
        0, 0, 
        m_broadcastsRect.width - 40, 
        static_cast<float>(m_broadcasts.size() * 40 + 10)
    };
    
    // Créer un panneau défilant pour les broadcasts
    // Augmenter la hauteur du contenu pour éviter les problèmes de défilement
    contentRect.height = std::max(contentRect.height, m_broadcastsRect.height * 2);
    
    ZappyTypes::Rectangle view = m_guiLib->DrawScrollPanel(
        m_broadcastsRect.x + 10, 
        m_broadcastsRect.y + 50, 
        m_broadcastsRect.width - 20, 
        m_broadcastsRect.height - 60,
        "",
        contentRect,
        std::make_shared<ZappyTypes::Vector2>(m_broadcastScrollPos)
    );
    
    // Afficher chaque broadcast dans la zone visible
    int i = 0;
    for (const auto& bc : m_broadcasts) {
        float yPos = view.y + i * 40;
        
        // Ne dessiner que si visible dans la zone de défilement
        if (yPos >= view.y && yPos <= view.y + view.height) {
            // Équipe émettrice
            m_guiLib->DrawLabel(view.x + 5, yPos, view.width - 10, 20, 
                              "Équipe: " + bc.team);
            
            // Message
            m_guiLib->DrawLabel(view.x + 5, yPos + 20, view.width - 10, 20, 
                              bc.message);
        }
        i++;
    }
}

void UserInterface::renderControlsPanel()
{
    // Panneau des contrôles de l'interface
    m_guiLib->DrawGroupBox(m_controlsRect.x, m_controlsRect.y, 
                         m_controlsRect.width, m_controlsRect.height, 
                         "Contrôles");
    
    float yOffset = m_controlsRect.y + 30;
    
    // Boutons de basculement pour chaque panneau
    m_showLogs = m_guiLib->DrawToggle(
        m_controlsRect.x + 10, 
        yOffset, 
        m_controlsRect.width - 20, 
        25, 
        "Afficher les logs", 
        m_showLogs
    );
    yOffset += 30;
    
    m_showTileInfo = m_guiLib->DrawToggle(
        m_controlsRect.x + 10, 
        yOffset, 
        m_controlsRect.width - 20, 
        25, 
        "Informations de case", 
        m_showTileInfo
    );
    yOffset += 30;
    
    // Sélecteur de mode de vue
    m_guiLib->DrawLabel(m_controlsRect.x + 10, yOffset, 
                      m_controlsRect.width - 20, 20, 
                      "Mode de vue:");
    yOffset += 25;
    
    // Crée un groupe de boutons radio pour les modes de vue
    // Avec des boutons individuels pour une meilleure visibilité
    float buttonHeight = 25;
    float spacing = 5;
    int newSelectedMode = m_currentViewMode;
    
    for (size_t i = 0; i < m_viewModes.size(); i++) {
        bool isSelected = (static_cast<int>(i) == m_currentViewMode);
        
        // Dessiner un cadre autour du mode sélectionné
        if (isSelected) {
            m_guiLib->DrawPanel(
                m_controlsRect.x + 8, 
                yOffset + i * (buttonHeight + spacing) - 2,
                m_controlsRect.width - 16,
                buttonHeight + 4
            );
        }
        
        // Vérifier si ce bouton est pressé
        if (m_guiLib->ButtonPressed(
                m_controlsRect.x + 10, 
                yOffset + i * (buttonHeight + spacing), 
                m_controlsRect.width - 20,
                buttonHeight, 
                m_viewModes[i])) {
            newSelectedMode = i;
        }
    }
    
    // Mise à jour du mode sélectionné si changement
    int selectedMode = newSelectedMode;
    
    // Mettre à jour le mode de vue si changé
    if (selectedMode != m_currentViewMode) {
        setViewMode(selectedMode);
    }
}

void UserInterface::renderTimeInfoPanel()
{
    // Panneau d'information sur le temps et la fréquence en haut à droite
    float panelWidth = 200.0f;
    // Pas besoin de définir la hauteur du panneau car nous positionnons les éléments manuellement
    float panelX = m_screenWidth - panelWidth;
    float panelY = 0;
    
    // On affiche directement les infos sans panneau pour économiser de l'espace
    std::stringstream timeInfo;
    timeInfo << "Temps: " << std::fixed << std::setprecision(2) << m_gameData.time;
    m_guiLib->DrawLabel(panelX, panelY, panelWidth, 20, timeInfo.str());
    
    std::stringstream freqInfo;
    freqInfo << "Fréquence: " << m_gameData.frequency << " Hz";
    m_guiLib->DrawLabel(panelX, panelY + 20, panelWidth, 20, freqInfo.str());
    
    std::stringstream tickInfo;
    tickInfo << "Tick: " << m_gameData.tick;
    m_guiLib->DrawLabel(panelX, panelY + 40, panelWidth, 20, tickInfo.str());
}

} // namespace GUI
