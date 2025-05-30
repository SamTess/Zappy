/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib texture encapsulation
*/

#include "Utils.hpp"
#include <raymath.h>
#include <sstream>
#include <string>

namespace raylibcpp {

Color Utils::hexToColor(const std::string& hex) {
    unsigned int rgb = 0;
    std::stringstream ss;
    ss << std::hex << hex.substr(1);
    ss >> rgb;
    return {(unsigned char)((rgb >> 16) & 0xFF),
        (unsigned char)((rgb >> 8) & 0xFF), (unsigned char)(rgb & 0xFF), 255};
}

Vector3Wrap::Vector3Wrap(float x, float y, float z) {
    vec.x = x;
    vec.y = y;
    vec.z = z;
}

Vector3& Vector3Wrap::get() {
    return vec;
}

void Vector3Wrap::set(float x, float y, float z) {
    vec.x = x;
    vec.y = y;
    vec.z = z;
}

Vector3 Vector3Wrap::add(const Vector3& a, const Vector3& b) {
    return Vector3Add(a, b);
}

Vector3 Vector3Wrap::sub(const Vector3& a, const Vector3& b) {
    return Vector3Subtract(a, b);
}

Vector3 Vector3Wrap::scale(const Vector3& v, float s) {
    return Vector3Scale(v, s);
}

Vector3 Vector3Wrap::normalize(const Vector3& v) {
    return Vector3Normalize(v);
}

Vector3 Vector3Wrap::cross(const Vector3& a, const Vector3& b) {
    return Vector3CrossProduct(a, b);
}

Vector3 Vector3Wrap::transform(const Vector3& v, const Matrix& m) {
    return Vector3Transform(v, m);
}
}  // namespace raylibcpp
