/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** RayLib and RayGUI type adapter
*/

#pragma once

#include <raylib.h>
#include "../src/Shared/Common.hpp"

namespace TypeAdapter {
    inline ::Vector2 ToRaylib(const ZappyTypes::Vector2& vec) {
        ::Vector2 result;
        result.x = vec.x;
        result.y = vec.y;
        return result;
    }

    inline ZappyTypes::Vector2 FromRaylib(const ::Vector2& vec) {
        ZappyTypes::Vector2 result;
        result.x = vec.x;
        result.y = vec.y;
        return result;
    }

    inline ::Vector3 ToRaylib(const ZappyTypes::Vector3& vec) {
        ::Vector3 result;
        result.x = vec.x;
        result.y = vec.y;
        result.z = vec.z;
        return result;
    }

    inline ZappyTypes::Vector3 FromRaylib(const ::Vector3& vec) {
        ZappyTypes::Vector3 result;
        result.x = vec.x;
        result.y = vec.y;
        result.z = vec.z;
        return result;
    }

    inline ::Color ToRaylib(const ZappyTypes::Color& color) {
        ::Color result;
        result.r = color.r;
        result.g = color.g;
        result.b = color.b;
        result.a = color.a;
        return result;
    }

    inline ZappyTypes::Color FromRaylib(const ::Color& color) {
        ZappyTypes::Color result;
        result.r = color.r;
        result.g = color.g;
        result.b = color.b;
        result.a = color.a;
        return result;
    }

    inline ::Rectangle ToRaylib(const ZappyTypes::Rectangle& rect) {
        ::Rectangle result;
        result.x = rect.x;
        result.y = rect.y;
        result.width = rect.width;
        result.height = rect.height;
        return result;
    }

    inline ZappyTypes::Rectangle FromRaylib(const ::Rectangle& rect) {
        ZappyTypes::Rectangle result;
        result.x = rect.x;
        result.y = rect.y;
        result.width = rect.width;
        result.height = rect.height;
        return result;
    }
}  // namespace TypeAdapter
