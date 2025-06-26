/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tile - Implémentation concrète d'une tuile
*/

#ifndef TILE_HPP_
#define TILE_HPP_

#include <array>
#include <vector>
#include <algorithm>
#include <memory>
#include "../IGameEntity.hpp"
#include "../ITile.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"
#include "../../../Shared/IGraphicsLib.hpp"

class Tile : public ITile {
public:
    explicit Tile(int x = 0, int y = 0);
    explicit Tile(const TileContentData& data);

    int getX() const override;
    int getY() const override;
    const std::array<int, 7>& getResources() const override;
    int getResourceQuantity(ResourceType type) const override;
    void setResourceQuantity(ResourceType type, int quantity) override;

    const std::vector<int>& getPlayerIds() const override;
    const std::vector<int>& getEggIds() const override;
    bool isIncantating() const override;
    void setIncantating(bool incantating) override;

    void addPlayer(int playerId) override;
    void removePlayer(int playerId) override;
    void addEgg(int eggId) override;
    void removeEgg(int eggId) override;

    void updateFromProtocol(const TileContentData& data) override;
    void render(const std::shared_ptr<IGraphicsLib>& graphicsLib,
                const ZappyTypes::Vector3& position,
                float tileSize) const override;

private:
    int _x, _y;
    std::array<int, 7> _resources;
    std::vector<int> _playerIds;
    std::vector<int> _eggIds;
    bool _isIncantating;
};

#endif /* !TILE_HPP_ */
