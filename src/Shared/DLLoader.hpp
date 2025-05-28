/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** DLLoader
*/
#pragma once

#include <dlfcn.h>
#include <string>
#include <stdexcept>
#include <memory>

template <typename T>
class DLLoader {
public:
    /**
     * @brief Constructeur chargeant une bibliothèque dynamique
     * @param libPath Chemin vers la bibliothèque à charger
     */
    explicit DLLoader(const std::string& libPath) {
        _handle = dlopen(libPath.c_str(), RTLD_LAZY);
        if (!_handle) {
            throw std::runtime_error("Impossible de charger la bibliothèque: " + std::string(dlerror()));
        }
    }

    /**
     * @brief Destructeur déchargeant la bibliothèque
     */
    ~DLLoader() {
        if (_handle) {
            dlclose(_handle);
        }
    }

    /**
     * @brief Récupère un symbole (fonction ou variable) depuis la bibliothèque
     * @param symbolName Nom du symbole à charger
     * @return Pointeur vers le symbole chargé
     */
    template<typename SymbolType>
    SymbolType getSymbol(const std::string& symbolName) {
        void* symbol = dlsym(_handle, symbolName.c_str());
        if (!symbol) {
            throw std::runtime_error("Impossible de trouver le symbole '" + symbolName + "': " + std::string(dlerror()));
        }
        return reinterpret_cast<SymbolType>(symbol);
    }

    /**
     * @brief Crée une instance de l'interface en utilisant une fonction de création
     * @param creatorName Nom de la fonction de création dans la bibliothèque
     * @return Pointeur vers l'instance créée
     */
    std::unique_ptr<T> getInstance(const std::string& creatorName) {
        try {
            using SharedCreatorFunc = std::shared_ptr<T> (*)();
            SharedCreatorFunc sharedCreator = getSymbol<SharedCreatorFunc>(creatorName);
            std::shared_ptr<T> sharedInstance = sharedCreator();
            if (!sharedInstance)
                throw std::runtime_error("La fonction de création a retourné un pointeur null");
            return std::unique_ptr<T, std::function<void(T*)>>(
                sharedInstance.get(),
                [capture = sharedInstance](T*) { }
            );
        } catch (const std::exception& e) {
            using RawCreatorFunc = T* (*)();
            RawCreatorFunc rawCreator = getSymbol<RawCreatorFunc>(creatorName);
            T* instance = rawCreator();
            if (!instance)
                throw std::runtime_error("La fonction de création a retourné un pointeur null");
            return std::unique_ptr<T>(instance);
        }
    }

private:
    void* _handle = nullptr; // Handle de la bibliothèque chargée
};
