/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Shape encapsulation
*/

#include "Shape.hpp"

namespace raylibcpp {

void Shape::drawRectangle(int x, int y, int width, int height, Color color) {
    DrawRectangle(x, y, width, height, color);
}

void Shape::drawCircle(int centerX, int centerY, float radius, Color color) {
    DrawCircle(centerX, centerY, radius, color);
}

void Shape::drawCube(Vector3 position, float width, float height, float length, Color color) {
    DrawCube(position, width, height, length, color);
}

void Shape::drawSphere(Vector3 centerPos, float radius, Color color) {
    DrawSphere(centerPos, radius, color);
}

void Shape::drawPlane(Vector3 centerPos, Vector2 size, Color color) {
    DrawPlane(centerPos, size, color);
}

void Shape::drawGrid(int slices, float spacing) {
    DrawGrid(slices, spacing);
}

void Shape::drawCylinder(Vector3 position, float radiusTop, float radiusBottom, float height, int slices, Color color) {
    DrawCylinder(position, radiusTop, radiusBottom, height, slices, color);
}

void Shape::drawLine3D(Vector3 startPos, Vector3 endPos, Color color) {
    DrawLine3D(startPos, endPos, color);
}

}  // namespace raylibcpp
