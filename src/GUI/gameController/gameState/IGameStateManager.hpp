/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IGameStateManager
*/

#ifndef IGAME_STATE_MANAGER_HPP_
#define IGAME_STATE_MANAGER_HPP_

#include <memory>
#include "../mapManager/IMapManager.hpp"
#include "../playerManager/IPlayerManager.hpp"
#include "IGameStatusManager.hpp"
#include "IGameSettingsManager.hpp"

class IGameStateManager {
public:
    virtual ~IGameStateManager() = default;

    virtual std::shared_ptr<IMapManager> getMapManager() = 0;
    virtual std::shared_ptr<const IMapManager> getMapManager() const = 0;
    virtual std::shared_ptr<IPlayerManager> getPlayerManager() = 0;
    virtual std::shared_ptr<const IPlayerManager> getPlayerManager() const = 0;
    virtual std::shared_ptr<IGameStatusManager> getGameStatusManager() = 0;
    virtual std::shared_ptr<const IGameStatusManager> getGameStatusManager() const = 0;
    virtual std::shared_ptr<IGameSettingsManager> getGameSettingsManager() = 0;
    virtual std::shared_ptr<const IGameSettingsManager> getGameSettingsManager() const = 0;
    virtual void initialize() = 0;
    virtual bool isInitialized() const = 0;
};

#endif /* !IGAME_STATE_MANAGER_HPP_ */
