/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IGameController
*/

#ifndef IGAME_CONTROLLER_HPP_
#define IGAME_CONTROLLER_HPP_

#include <memory>
#include "../shared/Message.hpp"
#include "../shared/commands/ICommandExecutor.hpp"
#include "../GUI/shared/IGraphicsLib.hpp"
#include "GameState.hpp"
#include "EntityFactory.hpp"

class IGameController {
    public:
        virtual ~IGameController() = default;

        virtual std::shared_ptr<const GameState> getGameState() const = 0;
        virtual void setEntityFactory(std::shared_ptr<EntityFactoryManager> factory) = 0;
        virtual void setGraphics(std::shared_ptr<IGraphicsLib> graphics) = 0;
        virtual void setCommandExecutor(std::shared_ptr<ICommandExecutor> executor) = 0;
        virtual void updateAnimations(float deltaTime) = 0;
        virtual void processMessage(const Message& message) = 0;
};

#endif /* !IGAME_CONTROLLER_HPP_ */
