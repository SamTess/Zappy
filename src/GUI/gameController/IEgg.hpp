/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IEgg
*/

#ifndef IEGG_HPP_
#define IEGG_HPP_

#include <string>
#include <memory>
#include "IGameEntity.hpp"

class EggData;

class IEgg : public IGameEntity {
    public:
        virtual void updateFromProtocol(const EggData& data) = 0;
        virtual void renderEgg(const std::shared_ptr<IGraphicsLib>& graphicsLib,
            const ZappyTypes::Vector3& position, float tileSize, int stackIndex = 0) const = 0;
};

#endif /* !IEGG_HPP_ */
