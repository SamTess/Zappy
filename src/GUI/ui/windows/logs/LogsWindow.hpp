/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** LogsWindow implementation
*/

#pragma once

#include <deque>
#include <string>
#include <memory>
#include "../AUIWindow.hpp"

namespace GUI {

class LogsWindow : public AUIWindow {
public:
    explicit LogsWindow(std::shared_ptr<IGuiLib> guiLib);
    ~LogsWindow() = default;

    /**
     * @brief Ajoute un message au journal
     * @param message Le message à ajouter
     */
    void addLogMessage(const std::string& message);

    /**
     * @brief Efface tous les messages du journal
     */
    void clearLogs();

protected:
    void renderContent() override;
    void updateSpecificData(const GameData& gameData) override;

private:
    // Liste des logs
    std::deque<std::string> m_logs;
    size_t m_maxLogs;
};

} // namespace GUI
