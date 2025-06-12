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

// Constantes pour les touches du clavier
enum KeyboardKey {
    Z_KEY_NULL = 0,
    Z_KEY_APOSTROPHE = 39,
    Z_KEY_COMMA = 44,
    Z_KEY_MINUS = 45,
    Z_KEY_PERIOD = 46,
    Z_KEY_SLASH = 47,
    Z_KEY_ZERO = 48,
    Z_KEY_ONE = 49,
    Z_KEY_TWO = 50,
    Z_KEY_THREE = 51,
    Z_KEY_FOUR = 52,
    Z_KEY_FIVE = 53,
    Z_KEY_SIX = 54,
    Z_KEY_SEVEN = 55,
    Z_KEY_EIGHT = 56,
    Z_KEY_NINE = 57,
    Z_KEY_SEMICOLON = 59,
    Z_KEY_EQUAL = 61,
    Z_KEY_A = 65,
    Z_KEY_B = 66,
    Z_KEY_C = 67,
    Z_KEY_D = 68,
    Z_KEY_E = 69,
    Z_KEY_F = 70,
    Z_KEY_G = 71,
    Z_KEY_H = 72,
    Z_KEY_I = 73,
    Z_KEY_J = 74,
    Z_KEY_K = 75,
    Z_KEY_L = 76,
    Z_KEY_M = 77,
    Z_KEY_N = 78,
    Z_KEY_O = 79,
    Z_KEY_P = 80,
    Z_KEY_Q = 81,
    Z_KEY_R = 82,
    Z_KEY_S = 83,
    Z_KEY_T = 84,
    Z_KEY_U = 85,
    Z_KEY_V = 86,
    Z_KEY_W = 87,
    Z_KEY_X = 88,
    Z_KEY_Y = 89,
    Z_KEY_Z = 90,
    Z_KEY_ESCAPE = 256,
    Z_KEY_ENTER = 257,
    Z_KEY_SPACE = 32,
};

} // namespace ZappyTypes
