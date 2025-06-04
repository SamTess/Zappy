/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ObjManager
*/

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "../../Shared/IGraphicsLib.hpp"
#include "ObjFile.hpp"

class ObjManager {
public:
    ObjManager() = default;
    ~ObjManager();

    int addObjFile(std::shared_ptr<IGraphicsLib> graphics, const std::string& path);
    void deleteObj(int id);
    void renderAll(std::shared_ptr<IGraphicsLib> graphics) const;
    ObjFile* getObj(int id);

private:
    std::unordered_map<int, std::unique_ptr<ObjFile>> m_objFiles;
    int m_nextId = 0;
};