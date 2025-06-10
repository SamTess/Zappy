/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ObjFile
*/

#pragma once

#include <memory>
#include <string>
#include "../../Shared/IGraphicsLib.hpp"
#include "../../Shared/Common.hpp"

class ObjFile {
public:
    explicit ObjFile(std::shared_ptr<IGraphicsLib> graphics, const std::string& objPath);
    ~ObjFile();

    void load(std::shared_ptr<IGraphicsLib> graphics, const std::string& objPath);
    const std::string& path() const;
    void display(std::shared_ptr<IGraphicsLib> graphics, ZappyTypes::Vector3 position, float scale = 1.0f, ZappyTypes::Color color = {255, 255, 255, 255}) const;

private:
    std::string m_filePath;
    bool m_isLoaded;
    int m_modelId;
};
