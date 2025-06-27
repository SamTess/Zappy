/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IGameSettingsManager
*/

#ifndef IGAME_SETTINGS_MANAGER_HPP_
#define IGAME_SETTINGS_MANAGER_HPP_

class IGameSettingsManager {
public:
    virtual ~IGameSettingsManager() = default;

    virtual bool getSfxEnabled() const = 0;
    virtual void setSfxEnabled(bool enabled) = 0;
    virtual float getMusicVolume() const = 0;
    virtual void setMusicVolume(float volume) = 0;
};

#endif /* !IGAME_SETTINGS_MANAGER_HPP_ */
