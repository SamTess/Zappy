/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Egg
*/

#ifndef EGG_HPP_
#define EGG_HPP_

#include <memory>
#include "../IGameEntity.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"
#include "../../../GUI/shared/IGraphicsLib.hpp"
#include "../IEgg.hpp"

class Egg : public IEgg {
    public:
        Egg();
        explicit Egg(const EggData& data);

        int getId() const override;
        int getX() const override;
        int getY() const override;
        void setPosition(int x, int y) override;
        void updateFromProtocol(const EggData& data) override;
        void render(const std::shared_ptr<IGraphicsLib>& graphicsLib,
                    const ZappyTypes::Vector3& position, float tileSize) const override;
        void renderEgg(const std::shared_ptr<IGraphicsLib>& graphicsLib,
            const ZappyTypes::Vector3& position,
            float tileSize,
            int stackIndex = 0) const override;

    private:
        int _eggId;
        int _x;
        int _y;
};

#endif /* !EGG_HPP_ */
