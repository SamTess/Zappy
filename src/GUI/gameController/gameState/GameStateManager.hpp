/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameStateManager
*/

#ifndef GAME_STATE_MANAGER_HPP_
#define GAME_STATE_MANAGER_HPP_

#include <memory>
#include "IGameStateManager.hpp"
#include "GameStatusManager.hpp"
#include "GameSettingsManager.hpp"
#include "../mapManager/MapManager.hpp"
#include "../playerManager/PlayerManager.hpp"
#include "../EntityFactory.hpp"

class GameStateManager : public IGameStateManager {
public:
    GameStateManager();
    explicit GameStateManager(std::shared_ptr<EntityFactoryManager> factory);
    ~GameStateManager() = default;

    std::shared_ptr<IMapManager> getMapManager() override;
    std::shared_ptr<const IMapManager> getMapManager() const override;
    std::shared_ptr<IPlayerManager> getPlayerManager() override;
    std::shared_ptr<const IPlayerManager> getPlayerManager() const override;
    std::shared_ptr<IGameStatusManager> getGameStatusManager() override;
    std::shared_ptr<const IGameStatusManager> getGameStatusManager() const override;
    std::shared_ptr<IGameSettingsManager> getGameSettingsManager() override;
    std::shared_ptr<const IGameSettingsManager> getGameSettingsManager() const override;

    void initialize() override;
    bool isInitialized() const override;

private:
    std::shared_ptr<MapManager> _mapManager;
    std::shared_ptr<PlayerManager> _playerManager;
    std::shared_ptr<GameStatusManager> _gameStatusManager;
    std::shared_ptr<GameSettingsManager> _gameSettingsManager;
    std::shared_ptr<EntityFactoryManager> _entityFactory;
    bool _isInitialized;
};

#endif /* !GAME_STATE_MANAGER_HPP_ */
