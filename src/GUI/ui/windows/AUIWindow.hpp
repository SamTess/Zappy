/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Abstract class for UI Windows
*/

#pragma once

#include <memory>
#include <string>
#include "IUIWindow.hpp"

namespace GUI {

/**
 * @brief Classe abstraite implémentant les comportements communs des fenêtres UI
 */
class AUIWindow : public IUIWindow {
public:
    /**
     * @brief Constructeur
     * @param guiLib Pointeur vers la bibliothèque GUI
     * @param title Titre de la fenêtre
     */
    AUIWindow(std::shared_ptr<IGuiLib> guiLib, const std::string& title);
    virtual ~AUIWindow() = default;

    void initialize(const ZappyTypes::Vector2& position,
                   const ZappyTypes::Vector2& dimensions) override;
    bool render() override;
    void updateData(const GameData& gameData) override;
    void setPosition(const ZappyTypes::Vector2& position) override;
    ZappyTypes::Vector2 getPosition() const override;
    ZappyTypes::Vector2 getDimensions() const override;
    bool isVisible() const override;
    void setVisible(bool visible) override;
    void toggleVisibility() override;
    bool startDragging(const ZappyTypes::Vector2& mousePosition) override;
    void updateDragging(const ZappyTypes::Vector2& mousePosition) override;
    void stopDragging() override;
    bool isDragging() const override;

protected:
    /**
     * @brief Méthode à implémenter par les classes dérivées pour le rendu du contenu
     */
    virtual void renderContent() = 0;

    /**
     * @brief Met à jour les données spécifiques à la fenêtre
     */
    virtual void updateSpecificData(const GameData& gameData) = 0;

    // Bibliothèque GUI
    std::shared_ptr<IGuiLib> m_guiLib;

    // Propriétés de la fenêtre
    std::string m_title;
    ZappyTypes::Vector2 m_position;
    ZappyTypes::Vector2 m_dimensions;
    ZappyTypes::Vector2 m_scrollPosition;

    // État de la fenêtre
    bool m_visible;

    // État de glissement
    bool m_dragging;
    ZappyTypes::Vector2 m_dragOffset;

    // Données de jeu
    GameData m_gameData;
};

} // namespace GUI
