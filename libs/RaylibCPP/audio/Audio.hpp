/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Audio encapsulation
*/

#pragma once
#include <raylib.h>
#include <string>

namespace raylibcpp {

class Audio {
    public:
        Audio();
        ~Audio();
};

class SoundWrap {
    public:
        explicit SoundWrap(const std::string& path);
        ~SoundWrap();
        void play() const;
        void stop() const;
        void pause() const;
        void resume() const;
        void setVolume(float volume) const;
        Sound& get();
    private:
        Sound sound;
};

class MusicWrap {
    public:
        explicit MusicWrap(const std::string& path);
        ~MusicWrap();
        void play() const;
        void stop() const;
        void pause() const;
        void resume() const;
        void update() const;
        void setVolume(float volume) const;
        Music& get();
    private:
        Music music;
};

}  // namespace raylibcpp
