/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TileContentData
*/

#ifndef TILE_CONTENT_DATA_HPP_
#define TILE_CONTENT_DATA_HPP_

#include "IMessageData.hpp"

class TileContentData : public IMessageData {
    public:
        TileContentData(int x, int y, int food, int linemate, int deraumere,
                    int sibur, int mendiane, int phiras, int thystame)
            : _x(x), _y(y), _food(food), _linemate(linemate), _deraumere(deraumere),
            _sibur(sibur), _mendiane(mendiane), _phiras(phiras), _thystame(thystame) {}
        MessageType getType() const override { return MessageType::TileContent; }

        int getX() const { return _x; }
        int getY() const { return _y; }
        int getFood() const { return _food; }
        int getLinemate() const { return _linemate; }
        int getDeraumere() const { return _deraumere; }
        int getSibur() const { return _sibur; }
        int getMendiane() const { return _mendiane; }
        int getPhiras() const { return _phiras; }
        int getThystame() const { return _thystame; }

        void setX(int value) { _x = value; }
        void setY(int value) { _y = value; }
        void setFood(int value) { _food = value; }
        void setLinemate(int value) { _linemate = value; }
        void setDeraumere(int value) { _deraumere = value; }
        void setSibur(int value) { _sibur = value; }
        void setMendiane(int value) { _mendiane = value; }
        void setPhiras(int value) { _phiras = value; }
        void setThystame(int value) { _thystame = value; }

    private:
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

#endif /* !TILE_CONTENT_DATA_HPP_ */
