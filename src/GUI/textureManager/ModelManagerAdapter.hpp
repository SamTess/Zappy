/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ModelManagerAdapter
*/

#pragma once

#include <memory>
#include "ModelManager.hpp"

namespace Zappy {

class ModelManagerAdapter {
    public:
        explicit ModelManagerAdapter() : managerRef(ModelManager::getInstance()) {}
        ModelManager& getManager() { return managerRef; }
        static std::shared_ptr<ModelManagerAdapter> createShared() {
            return std::make_shared<ModelManagerAdapter>();
        }

        void setGraphicsLib(std::shared_ptr<IGraphicsLib> graphicsLib) {
            managerRef.setGraphicsLib(graphicsLib);
        }
    private:
        ModelManager& managerRef;
};

} // namespace Zappy
