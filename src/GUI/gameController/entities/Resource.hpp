/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Resource
*/

#ifndef RESOURCE_HPP_
#define RESOURCE_HPP_

#include <string>
#include <memory>
#include "../IGameEntity.hpp"
#include "../../../Shared/IGraphicsLib.hpp"
#include "../IResource.hpp"

class Resource : public IResource {
public:
    explicit Resource(ResourceType type, int quantity = 0);

    ResourceType getType() const override;
    int getQuantity() const override;
    void setQuantity(int quantity) override;
    void addQuantity(int amount) override;
    bool isEmpty() const override;
    const std::string& getName() const override;

    void render(const std::shared_ptr<IGraphicsLib>& graphicsLib,
                const ZappyTypes::Vector3& position,
                float tileSize) const override;

    void renderResource(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        const ZappyTypes::Vector3& position,
        float tileSize) const override;

private:
    ResourceType _type;
    int _quantity;
    std::string _name;

    void updateName();
};

#endif /* !RESOURCE_HPP_ */
