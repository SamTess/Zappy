/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Audio encapsulation
*/

#include "Audio.hpp"
#include <stdexcept>
#include <string>

namespace raylibcpp {

Audio::Audio() {
    InitAudioDevice();
}

Audio::~Audio() {
    CloseAudioDevice();
}

SoundWrap::SoundWrap(const std::string& path) {
    sound = LoadSound(path.c_str());
    if (sound.frameCount == 0)
        throw std::runtime_error("Erreur chargement sound: " + path);
}

SoundWrap::~SoundWrap() {
    UnloadSound(sound);
}

void SoundWrap::play() const {
    PlaySound(sound);
}

void SoundWrap::stop() const {
    StopSound(sound);
}

void SoundWrap::pause() const {
    PauseSound(sound);
}

void SoundWrap::resume() const {
    ResumeSound(sound);
}

void SoundWrap::setVolume(float volume) const {
    SetSoundVolume(sound, volume);
}

Sound& SoundWrap::get() {
    return sound;
}

MusicWrap::MusicWrap(const std::string& path) {
    music = LoadMusicStream(path.c_str());
    if (music.ctxData == nullptr)
        throw std::runtime_error("Erreur chargement music: " + path);
}

MusicWrap::~MusicWrap() {
    UnloadMusicStream(music);
}

void MusicWrap::play() const {
    PlayMusicStream(music);
}

void MusicWrap::stop() const {
    StopMusicStream(music);
}

void MusicWrap::pause() const {
    PauseMusicStream(music);
}

void MusicWrap::resume() const {
    ResumeMusicStream(music);
}

void MusicWrap::update() const {
    UpdateMusicStream(music);
}

void MusicWrap::setVolume(float volume) const {
    SetMusicVolume(music, volume);
}

Music& MusicWrap::get() {
    return music;
}
}  // namespace raylibcpp
