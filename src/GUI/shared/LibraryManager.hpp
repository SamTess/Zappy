/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** LibraryManager
*/

#pragma once

#include <string>
#include <memory>
#include <functional>
#include "DLLoader.hpp"
#include "IGraphicsLib.hpp"
#include "IGuiLib.hpp"

class LibraryManager {
    public:
        static LibraryManager& getInstance() {
            static LibraryManager instance;
            return instance;
        }
        bool loadGraphicsLib(const std::string& libPath, const std::string& creatorFunction = "createGraphicsLib") {
            try {
                _graphicsLoader = std::make_unique<DLLoader<IGraphicsLib>>(libPath);
                _graphicsLib = _graphicsLoader->getInstance(creatorFunction);
                return true;
            } catch (const std::exception& e) {
                _lastError = e.what();
                return false;
            }
        }
        bool loadGuiLib(const std::string& libPath, const std::string& creatorFunction = "createGuiLib") {
            try {
                _guiLoader = std::make_unique<DLLoader<IGuiLib>>(libPath);
                _guiLib = _guiLoader->getInstance(creatorFunction);
                return true;
            } catch (const std::exception& e) {
                _lastError = e.what();
                return false;
            }
        }
        std::shared_ptr<IGraphicsLib> getGraphicsLibPtr() {
            if (!_graphicsLib) {
                throw std::runtime_error("Bibliothèque graphique non chargée");
            }
            return std::shared_ptr<IGraphicsLib>(_graphicsLib.get(), [](IGraphicsLib*){});
        }

        std::shared_ptr<IGuiLib> getGuiLibPtr() {
            if (!_guiLib) {
                throw std::runtime_error("Bibliothèque GUI non chargée");
            }
            return std::shared_ptr<IGuiLib>(_guiLib.get(), [](IGuiLib*){});
        }
        IGraphicsLib& getGraphicsLib() {
            if (!_graphicsLib) {
                throw std::runtime_error("Bibliothèque graphique non chargée");
            }
            return *_graphicsLib;
        }
        IGuiLib& getGuiLib() {
            if (!_guiLib) {
                throw std::runtime_error("Bibliothèque GUI non chargée");
            }
            return *_guiLib;
        }
        const std::string& getLastError() const {
            return _lastError;
        }

    private:
        LibraryManager() = default;
        LibraryManager(const LibraryManager&) = delete;
        LibraryManager& operator=(const LibraryManager&) = delete;

        std::unique_ptr<DLLoader<IGraphicsLib>> _graphicsLoader;
        std::unique_ptr<DLLoader<IGuiLib>> _guiLoader;
        std::unique_ptr<IGraphicsLib, std::function<void(IGraphicsLib*)>> _graphicsLib;
        std::unique_ptr<IGuiLib, std::function<void(IGuiLib*)>> _guiLib;
        std::string _lastError;
};
