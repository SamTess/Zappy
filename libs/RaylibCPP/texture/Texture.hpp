/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib texture encapsulation
*/

#pragma once
#include <raylib.h>
#include <string>
#include <vector>
#include <iostream>

namespace raylibcpp {

class Texture {
    public:
        explicit Texture(const std::string& path);
        ~Texture();
        void draw(int x, int y) const;
        Texture2D get() const;
    private:
        Texture2D texture;
};

class Texture3DWrap {
    public:
        explicit Texture3DWrap(const std::string& path);
        ~Texture3DWrap();
        void bind(int unit = 0) const;
        Texture2D& get();
    private:
        Texture2D texture;
};

class ModelWrap {
    public:
        explicit ModelWrap(const std::string& objPath);
        ~ModelWrap();
        void draw(Vector3 position, float scale = 1.0f, Color tint = WHITE) const;
        Model& get();
    private:
        Model model;
};

}  // namespace raylibcpp
