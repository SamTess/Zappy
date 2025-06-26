/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GameSettingsManager
*/

#include "GameSettingsManager.hpp"

GameSettingsManager::GameSettingsManager()
    : _sfxEnabled(true), _musicVolume(0.8f) {
}

bool GameSettingsManager::getSfxEnabled() const {
    return _sfxEnabled;
}

void GameSettingsManager::setSfxEnabled(bool enabled) {
    _sfxEnabled = enabled;
}

float GameSettingsManager::getMusicVolume() const {
    return _musicVolume;
}

void GameSettingsManager::setMusicVolume(float volume) {
    _musicVolume = volume;
}
