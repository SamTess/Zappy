/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameSettingsManager
*/

#ifndef GAME_SETTINGS_MANAGER_HPP_
#define GAME_SETTINGS_MANAGER_HPP_

#include "IGameSettingsManager.hpp"

class GameSettingsManager : public IGameSettingsManager {
public:
    GameSettingsManager();
    ~GameSettingsManager() = default;

    bool getSfxEnabled() const override;
    void setSfxEnabled(bool enabled) override;
    float getMusicVolume() const override;
    void setMusicVolume(float volume) override;

private:
    bool _sfxEnabled;
    float _musicVolume;
};

#endif /* !GAME_SETTINGS_MANAGER_HPP_ */
