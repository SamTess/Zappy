/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Common namespaced types
*/
#pragma once

#include <cstdint>

namespace ZappyTypes {
struct Vector2 {
    float x;
    float y;
};

struct Vector3 {
    float x;
    float y;
    float z;
};

struct Color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

struct Rectangle {
    float x;
    float y;
    float width;
    float height;
};

namespace Colors {
    static inline constexpr ZappyTypes::Color Z_LIGHTGRAY  = { 200, 200, 200, 255 };
    static inline constexpr ZappyTypes::Color Z_GRAY       = { 130, 130, 130, 255 };
    static inline constexpr ZappyTypes::Color Z_DARKGRAY   = { 80, 80, 80, 255 };
    static inline constexpr ZappyTypes::Color Z_YELLOW     = { 253, 249, 0, 255 };
    static inline constexpr ZappyTypes::Color Z_GOLD       = { 255, 203, 0, 255 };
    static inline constexpr ZappyTypes::Color Z_ORANGE     = { 255, 161, 0, 255 };
    static inline constexpr ZappyTypes::Color Z_PINK       = { 255, 109, 194, 255 };
    static inline constexpr ZappyTypes::Color Z_RED        = { 230, 41, 55, 255 };
    static inline constexpr ZappyTypes::Color Z_MAROON     = { 190, 33, 55, 255 };
    static inline constexpr ZappyTypes::Color Z_GREEN      = { 0, 228, 48, 255 };
    static inline constexpr ZappyTypes::Color Z_LIME       = { 0, 158, 47, 255 };
    static inline constexpr ZappyTypes::Color Z_DARKGREEN  = { 0, 117, 44, 255 };
    static inline constexpr ZappyTypes::Color Z_SKYBLUE    = { 102, 191, 255, 255 };
    static inline constexpr ZappyTypes::Color Z_BLUE       = { 0, 121, 241, 255 };
    static inline constexpr ZappyTypes::Color Z_DARKBLUE   = { 0, 82, 172, 255 };
    static inline constexpr ZappyTypes::Color Z_PURPLE     = { 200, 122, 255, 255 };
    static inline constexpr ZappyTypes::Color Z_VIOLET     = { 135, 60, 190, 255 };
    static inline constexpr ZappyTypes::Color Z_DARKPURPLE = { 112, 31, 126, 255 };
    static inline constexpr ZappyTypes::Color Z_BEIGE      = { 211, 176, 131, 255 };
    static inline constexpr ZappyTypes::Color Z_BROWN      = { 127, 106, 79, 255 };
    static inline constexpr ZappyTypes::Color Z_DARKBROWN  = { 76, 63, 47, 255 };
    static inline constexpr ZappyTypes::Color Z_WHITE      = { 255, 255, 255, 255 };
    static inline constexpr ZappyTypes::Color Z_BLACK      = { 0, 0, 0, 255 };
    static inline constexpr ZappyTypes::Color Z_BLANK      = { 0, 0, 0, 0 };
    static inline constexpr ZappyTypes::Color Z_MAGENTA    = { 255, 0, 255, 255 };
    static inline constexpr ZappyTypes::Color Z_RAYWHITE   = { 245, 245, 245, 255 };
} // namespace Colors
} // namespace ZappyTypes
