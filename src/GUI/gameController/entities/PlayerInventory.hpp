/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerInventory - Implémentation concrète d'un inventaire de joueur
*/

#ifndef PLAYER_INVENTORY_HPP_
#define PLAYER_INVENTORY_HPP_

#include <array>
#include "../IGameEntity.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"
#include "../IPlayerInventory.hpp"

class PlayerInventory : public IPlayerInventory {
public:
    PlayerInventory();
    explicit PlayerInventory(const PlayerInventoryData& data);

    int getPlayerId() const override;
    int getX() const override;
    int getY() const override;
    const std::array<int, 7>& getResources() const override;

    int getFood() const override;
    int getLinemate() const override;
    int getDeraumere() const override;
    int getSibur() const override;
    int getMendiane() const override;
    int getPhiras() const override;
    int getThystame() const override;

    void updateFromProtocol(const PlayerInventoryData& data) override;
    void setPosition(int x, int y) override;

private:
    int _playerId;
    int _x, _y;
    std::array<int, 7> _resources;
};

#endif /* !PLAYER_INVENTORY_HPP_ */
