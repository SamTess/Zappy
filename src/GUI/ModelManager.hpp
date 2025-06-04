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
#include "obj/ObjManager.hpp"
#include "../Shared/IGraphicsLib.hpp"
#include "../Shared/Common.hpp"
#include "Constants.hpp"

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
    std::unique_ptr<ObjManager> m_objManager;
};
