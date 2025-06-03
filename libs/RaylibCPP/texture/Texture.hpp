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
#include <memory>
#include <unordered_map>

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

class ObjFile {
    public:
        explicit ObjFile(const std::string& objPath);
        ~ObjFile();
        void load(const std::string& objPath);
        const std::string& path() const;
        void display(Vector3 position, float scale = 1.0f, Color tint = WHITE) const;
        Model& getModel();
    private:
        Model model;
        std::string filePath;
};

class ObjManager {
    public:
        ObjManager() = default;
        ~ObjManager();
        int addObjFile(const std::string& path);
        void deleteObj(int id);
        void renderAll() const;
        ObjFile* getObj(int id);
    private:
        std::unordered_map<int, std::unique_ptr<ObjFile>> objFiles;
        int nextId = 0;
};

}  // namespace raylibcpp
