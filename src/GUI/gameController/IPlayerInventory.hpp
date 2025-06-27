/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IIenventory
*/

#ifndef IIENVENTORY_HPP_
#define IIENVENTORY_HPP_
#include <string>
#include <array>

class PlayerInventoryData;

class IPlayerInventory {
    public:
        virtual ~IPlayerInventory() = default;
        virtual int getPlayerId() const = 0;
        virtual int getX() const = 0;
        virtual int getY() const = 0;
        virtual const std::array<int, 7>& getResources() const = 0;
        virtual int getFood() const = 0;
        virtual int getLinemate() const = 0;
        virtual int getDeraumere() const = 0;
        virtual int getSibur() const = 0;
        virtual int getMendiane() const = 0;
        virtual int getPhiras() const = 0;
        virtual int getThystame() const = 0;
        virtual void updateFromProtocol(const PlayerInventoryData& data) = 0;
        virtual void setPosition(int x, int y) = 0;
        virtual int getResourceCount(int resourceType) const = 0;
};

#endif /* !IIENVENTORY_HPP_ */
