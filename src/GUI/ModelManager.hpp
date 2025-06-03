/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManager
*/

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/Common.hpp"
#include "Constants.hpp"

class ObjFile {
public:
    explicit ObjFile(std::shared_ptr<IGraphicsLib> graphics, const std::string& objPath);
    ~ObjFile() = default;

    void load(std::shared_ptr<IGraphicsLib> graphics, const std::string& objPath);
    const std::string& path() const;
    void display(std::shared_ptr<IGraphicsLib> graphics, ZappyTypes::Vector3 position, float scale = 1.0f, ZappyTypes::Color color = {255, 255, 255, 255}) const;

private:
    std::string m_filePath;
    bool m_isLoaded;
};

class ModelManager {
public:
    ModelManager();
    ~ModelManager() = default;

    int addObjFile(std::shared_ptr<IGraphicsLib> graphics, const std::string& path);
    void deleteObj(int id);
    void renderAll(std::shared_ptr<IGraphicsLib> graphics) const;
    ObjFile* getObj(int id);

    bool isShowModel() const { return m_showModel; }
    void setShowModel(bool show) { m_showModel = show; }

    int GetRandomValue(int min, int max);

private:
    bool m_showModel;
    std::unordered_map<int, std::unique_ptr<ObjFile>> m_objFiles;
    int m_nextId = 0;
};
