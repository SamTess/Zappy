/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerInventoryData
*/

#ifndef PLAYER_INVENTORY_DATA_HPP_
#define PLAYER_INVENTORY_DATA_HPP_

#include "IMessageData.hpp"

class PlayerInventoryData : public IMessageData {
    public:
        PlayerInventoryData() : _id(0), _x(0), _y(0), _food(0), _linemate(0), _deraumere(0),
                            _sibur(0), _mendiane(0), _phiras(0), _thystame(0) {}
        PlayerInventoryData(int id, int x, int y, int food, int linemate, int deraumere,
                        int sibur, int mendiane, int phiras, int thystame)
            : _id(id), _x(x), _y(y), _food(food), _linemate(linemate), _deraumere(deraumere),
            _sibur(sibur), _mendiane(mendiane), _phiras(phiras), _thystame(thystame) {}
        MessageType getType() const override { return MessageType::PlayerInventory; }

        int getId() const { return _id; }
        int getFood() const { return _food; }
        int getLinemate() const { return _linemate; }
        int getDeraumere() const { return _deraumere; }
        int getSibur() const { return _sibur; }
        int getMendiane() const { return _mendiane; }
        int getPhiras() const { return _phiras; }
        int getThystame() const { return _thystame; }
        int getX() const { return _x; }
        int getY() const { return _y; }

        void setId(int value) { _id = value; }
        void setFood(int value) { _food = value; }
        void setLinemate(int value) { _linemate = value; }
        void setDeraumere(int value) { _deraumere = value; }
        void setSibur(int value) { _sibur = value; }
        void setMendiane(int value) { _mendiane = value; }
        void setPhiras(int value) { _phiras = value; }
        void setThystame(int value) { _thystame = value; }
        void setX(int value) { _x = value; }
        void setY(int value) { _y = value; }

    private:
        int _id;
        int _x;
        int _y;
        int _food;
        int _linemate;
        int _deraumere;
        int _sibur;
        int _mendiane;
        int _phiras;
        int _thystame;
};

#endif /* !PLAYER_INVENTORY_DATA_HPP_ */
