/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib utils encapsulation
*/

#pragma once
#include <raylib.h>
#include <string>
#include <algorithm>

namespace raylibcpp {

class Utils {
    public:
        static Color hexToColor(const std::string& hex);
};

class Vector3Wrap {
    public:
        explicit Vector3Wrap(float x = 0, float y = 0, float z = 0);
        Vector3& get();
        void set(float x, float y, float z);
        static Vector3 add(const Vector3& a, const Vector3& b);
        static Vector3 sub(const Vector3& a, const Vector3& b);
        static Vector3 scale(const Vector3& v, float s);
        static Vector3 normalize(const Vector3& v);
        static Vector3 cross(const Vector3& a, const Vector3& b);
        static Vector3 transform(const Vector3& v, const Matrix& m);
    private:
        Vector3 vec;
};

}  // namespace raylibcpp
