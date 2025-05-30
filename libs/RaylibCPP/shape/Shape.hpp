/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib Shape encapsulation
*/

#pragma once
#include <raylib.h>

namespace raylibcpp {

class Shape {
    public:
        // 2D
        static void drawRectangle(int x, int y, int width, int height, Color color);
        static void drawCircle(int centerX, int centerY, float radius, Color color);

        // 3D
        static void drawCube(Vector3 position, float width, float height, float length, Color color);
        static void drawSphere(Vector3 centerPos, float radius, Color color);
        static void drawPlane(Vector3 centerPos, Vector2 size, Color color);
        static void drawGrid(int slices, float spacing);
        static void drawCylinder(Vector3 position, float radiusTop, float radiusBottom,
            float height, int slices, Color color);
        static void drawLine3D(Vector3 startPos, Vector3 endPos, Color color);
};

}  // namespace raylibcpp
