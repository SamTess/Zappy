/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** ControlsWindow definition
*/

#pragma once

#include <vector>
#include <string>
#include <memory>
#include "../AUIWindow.hpp"

namespace GUI {

class ControlsWindow : public AUIWindow {
public:
    explicit ControlsWindow(std::shared_ptr<IGuiLib> guiLib);
    ~ControlsWindow() = default;

    /**
     * @brief Définit le mode de vue actuel
     * @param mode Indice du mode de vue
     */
    void setViewMode(int mode);

    /**
     * @brief Récupère le mode de vue actuel
     * @return Indice du mode de vue
     */
    int getViewMode() const;

protected:
    void renderContent() override;
    void updateSpecificData(const GameData& gameData) override;

private:
    int m_currentViewMode;
    std::vector<std::string> m_viewModes;
};

} // namespace GUI
