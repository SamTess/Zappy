/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IGameEntity
*/

#ifndef IGAME_ENTITY_HPP_
#define IGAME_ENTITY_HPP_

#include <string>
#include <memory>
#include <array>
#include <algorithm>
#include <vector>
#include "IRenderable.hpp"
#include "../../GUI/shared/Common.hpp"

class IGameEntity : public IRenderable {
    public:
        virtual ~IGameEntity() = default;
        virtual int getId() const = 0;
        virtual int getX() const = 0;
        virtual int getY() const = 0;
        virtual void setPosition(int x, int y) = 0;
};

#endif /* !IGAME_ENTITY_HPP_ */
