/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GraphicsManager
*/

#ifndef GRAPHICS_MANAGER_HPP_
#define GRAPHICS_MANAGER_HPP_

#include <memory>
#include "../../../Shared/IGraphicsLib.hpp"

class GraphicsManager {
public:
    explicit GraphicsManager(std::shared_ptr<IGraphicsLib> graphics);
    ~GraphicsManager() = default;

    void setGraphics(std::shared_ptr<IGraphicsLib> graphics);
    std::shared_ptr<IGraphicsLib> getGraphics() const;
    bool isGraphicsAvailable() const;

private:
    std::shared_ptr<IGraphicsLib> _graphics;
};

#endif /* !GRAPHICS_MANAGER_HPP_ */
