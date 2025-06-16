/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EggData
*/

#ifndef EGG_DATA_HPP_
#define EGG_DATA_HPP_

#include "IMessageData.hpp"

class EggData : public IMessageData {
    public:
        enum class EggAction {
            Drop,       // enw - Egg creation
            Connection, // ebo - Egg hatching/connection
            Death,      // edi - Egg death
            Laying      // pfk - Player starts laying an egg
        };

        EggData() : _eggId(0), _playerId(0), _x(0), _y(0), _action(EggAction::Drop) {}
        EggData(int eggId, int playerId, int x, int y, EggAction action = EggAction::Drop)
            : _eggId(eggId), _playerId(playerId), _x(x), _y(y), _action(action) {}

        MessageType getType() const override { return MessageType::Egg; }
        EggAction getAction() const { return _action; }

        int getEggId() const { return _eggId; }
        int getPlayerId() const { return _playerId; }
        int getX() const { return _x; }
        int getY() const { return _y; }

        void setEggId(int value) { _eggId = value; }
        void setPlayerId(int value) { _playerId = value; }
        void setX(int value) { _x = value; }
        void setY(int value) { _y = value; }

    private:
        int _eggId;
        int _playerId;
        int _x;
        int _y;
        EggAction _action;
};

#endif /* !EGG_DATA_HPP_ */
