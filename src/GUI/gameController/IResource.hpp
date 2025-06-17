/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IRessources
*/

#ifndef IRESSOURCES_HPP_
#define IRESSOURCES_HPP_

#include <string>
#include <memory>
#include "IGameEntity.hpp"

class IResource : public IRenderable {
public:
    virtual ~IResource() = default;
    virtual ResourceType getType() const = 0;
    virtual int getQuantity() const = 0;
    virtual void setQuantity(int quantity) = 0;
    virtual void addQuantity(int amount) = 0;
    virtual bool isEmpty() const = 0;
    virtual const std::string& getName() const = 0;
    virtual void renderResource(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        const ZappyTypes::Vector3& position,
        float tileSize) const = 0;
};

#endif /* !IRESSOURCES_HPP_ */
