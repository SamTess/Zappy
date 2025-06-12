/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GraphicalContext
*/

#ifndef GRAPHICAL_CONTEXT_HPP_
#define GRAPHICAL_CONTEXT_HPP_

#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <string>
#include <array>

#include "../network/protocol/Message.hpp"
#include "../network/protocol/messageData/MessageDataAll.hpp"
#include "../network/protocol/HeaderMessage.hpp"

/**
 * @brief Structure représentant une tuile dans le contexte graphique
 */
struct TileData {
    std::array<int, 7> resources{0}; // Quantité de chaque ressource
    std::vector<int> playerIds;      // IDs des joueurs sur cette tuile
    std::vector<int> eggIds;         // IDs des oeufs sur cette tuile
    bool isIncantating{false};       // Indique si une incantation est en cours sur cette tuile
};

/**
 * @brief Observateur de changements de contexte graphique
 */
class IGraphicalContextObserver {
public:
    virtual ~IGraphicalContextObserver() = default;
    virtual void onMapSizeChanged(int width, int height) = 0;
    virtual void onTileChanged(int x, int y, const TileData& tileData) = 0;
};

/**
 * @brief Énumération des types de ressources
 */
enum class ResourceType {
    FOOD = 0,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    COUNT
};


class GraphicalContext {
public:
    typedef void (GraphicalContext::*UpdateFunction)(std::shared_ptr<IMessageData>);
    GraphicalContext();
    ~GraphicalContext() = default;

    void updateContext(const Message& message);

    /**
     * @brief Ajoute un observateur au contexte graphique
     * @param observer L'observateur à ajouter
     */
    void addObserver(std::shared_ptr<IGraphicalContextObserver> observer);

    /**
     * @brief Supprime un observateur du contexte graphique
     * @param observer L'observateur à supprimer
     */
    void removeObserver(std::shared_ptr<IGraphicalContextObserver> observer);

    /**
     * @brief Récupère la largeur de la carte
     * @return La largeur de la carte
     */
    int getMapWidth() const { return _mapWidth; }

    /**
     * @brief Récupère la hauteur de la carte
     * @return La hauteur de la carte
     */
    int getMapHeight() const { return _mapHeight; }

    /**
     * @brief Récupère les données d'une tuile
     * @param x Coordonnée X de la tuile
     * @param y Coordonnée Y de la tuile
     * @return Les données de la tuile
     */
    const TileData& getTileData(int x, int y) const;

    /**
     * @brief Vérifie si un joueur est présent sur une tuile
     * @param x Coordonnée X de la tuile
     * @param y Coordonnée Y de la tuile
     * @param playerId ID du joueur
     * @return true si le joueur est présent, false sinon
     */
    bool isPlayerOnTile(int x, int y, int playerId) const;

    /**
     * @brief Récupère la quantité d'une ressource sur une tuile
     * @param x Coordonnée X de la tuile
     * @param y Coordonnée Y de la tuile
     * @param resourceType Type de ressource
     * @return La quantité de ressource
     */
    int getResourceQuantity(int x, int y, ResourceType resourceType) const;

    /**
     * @brief Récupère le type de ressource dominant sur une tuile
     * @param x Coordonnée X de la tuile
     * @param y Coordonnée Y de la tuile
     * @return Le type de ressource dominant
     */
    ResourceType getDominantResourceType(int x, int y) const;

    /**
     * @brief Récupère les informations d'un joueur
     * @param playerId ID du joueur
     * @return Les informations du joueur ou nullptr si le joueur n'existe pas
     */
    const std::shared_ptr<PlayerInfoData> getPlayerInfo(int playerId) const;

private:
    void updateMapSize(std::shared_ptr<IMessageData> data);
    void updateTileContent(std::shared_ptr<IMessageData> data);
    void updateTeamName(std::shared_ptr<IMessageData> data);
    void updatePlayerInfo(std::shared_ptr<IMessageData> data);
    void updatePlayerInventory(std::shared_ptr<IMessageData> data);
    void updatePlayerExpulsion(std::shared_ptr<IMessageData> data);
    void updatePlayerBroadcast(std::shared_ptr<IMessageData> data);
    void updateResourceDrop(std::shared_ptr<IMessageData> data);
    void updateResourceCollect(std::shared_ptr<IMessageData> data);
    void updatePlayerDeath(std::shared_ptr<IMessageData> data);
    void updateIncantationStart(std::shared_ptr<IMessageData> data);
    void updateIncantationEnd(std::shared_ptr<IMessageData> data);
    void updateEggLaying(std::shared_ptr<IMessageData> data);
    void updateEggDrop(std::shared_ptr<IMessageData> data);
    void updateEggConnection(std::shared_ptr<IMessageData> data);
    void updateEggDeath(std::shared_ptr<IMessageData> data);
    void updateTimeUnit(std::shared_ptr<IMessageData> data);
    void updateEndGame(std::shared_ptr<IMessageData> data);
    void updateServerMessage(std::shared_ptr<IMessageData> data);


    std::vector<std::string> _teamNames;
    std::map<int, PlayerInfoData> _players;
    std::map<int, PlayerInventoryData> _inventories;
    std::map<int, EggData> _eggs;
    int _timeUnit = 100;
    bool _gameEnded = false;
    std::string _winningTeam;
    int _mapWidth = 0;
    int _mapHeight = 0;
    std::vector<std::vector<TileData>> _mapTiles;
    std::vector<std::shared_ptr<IGraphicalContextObserver>> _observers;
    std::map<MessageType, UpdateFunction> _updateFunctions;

private:
    /**
     * @brief Notifie les observateurs d'un changement de taille de carte
     */
    void notifyMapSizeChanged();
    /**
     * @brief Notifie les observateurs d'un changement de tuile
     * @param x Coordonnée X de la tuile
     * @param y Coordonnée Y de la tuile
     */
    void notifyTileChanged(int x, int y);

    /**
     * @brief Affiche la carte dans la console
     */
    void displayMap() const;

    /**
     * @brief Vérifie si les coordonnées sont valides
     * @param x Coordonnée X à vérifier
     * @param y Coordonnée Y à vérifier
     * @return true si les coordonnées sont valides, false sinon
     */
    bool isValidCoordinates(int x, int y) const;

    bool isMapInit = false;
};

#endif /* !GRAPHICAL_CONTEXT_HPP_ */
