/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IncantationEndData
*/

#ifndef INCANTATION_END_DATA_HPP_
#define INCANTATION_END_DATA_HPP_

#include "IMessageData.hpp"

class IncantationEndData : public IMessageData {
    public:
        IncantationEndData(int x, int y, bool success)
            : _x(x), _y(y), _success(success) {}

        MessageType getType() const override { return MessageType::IncantationEnd; }

        int getX() const { return _x; }
        int getY() const { return _y; }
        bool isSuccess() const { return _success; }

        void setX(int value) { _x = value; }
        void setY(int value) { _y = value; }
        void setSuccess(bool value) { _success = value; }

    private:
        int _x;
        int _y;
        bool _success;
};

#endif /* !INCANTATION_END_DATA_HPP_ */
