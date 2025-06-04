/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MapSizeData
*/

#ifndef MAP_SIZE_DATA_HPP_
#define MAP_SIZE_DATA_HPP_

#include "IMessageData.hpp"

class MapSizeData : public IMessageData {
    public:
        MapSizeData(int width, int height) : _width(width), _height(height) {}
        MessageType getType() const override { return MessageType::MapSize; }

        int getWidth() const { return _width; }
        int getHeight() const { return _height; }

        void setWidth(int value) { _width = value; }
        void setHeight(int value) { _height = value; }

    private:
        int _width;
        int _height;
};

#endif /* !MAP_SIZE_DATA_HPP_ */
