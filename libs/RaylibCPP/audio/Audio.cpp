/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Audio encapsulation
*/

#include "Audio.hpp"

namespace raylibcpp {

Audio::Audio() {
    InitAudioDevice();
}
Audio::~Audio() {
    CloseAudioDevice();
}

}  // namespace raylibcpp
