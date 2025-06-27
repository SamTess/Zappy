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
#include <functional>

template <typename T>
class DLLoader {
    public:
        explicit DLLoader(const std::string& libPath) {
            _handle = dlopen(libPath.c_str(), RTLD_LAZY);
            if (!_handle)
                throw std::runtime_error("Impossible de charger la bibliothèque: " + std::string(dlerror()));
        }
        ~DLLoader() {
            if (_handle)
                dlclose(_handle);
        }
        template<typename SymbolType>
        SymbolType getSymbol(const std::string& symbolName) {
            void* symbol = dlsym(_handle, symbolName.c_str());
            if (!symbol)
                throw std::runtime_error("Impossible de trouver le symbole '" + symbolName + "': " + std::string(dlerror()));
            return reinterpret_cast<SymbolType>(symbol);
        }
        std::unique_ptr<T, std::function<void(T*)>> getInstance(const std::string& creatorName) {
            try {
                using SharedCreatorFunc = std::shared_ptr<T> (*)();
                SharedCreatorFunc sharedCreator = getSymbol<SharedCreatorFunc>(creatorName);
                std::shared_ptr<T> sharedInstance = sharedCreator();
                if (!sharedInstance)
                    throw std::runtime_error("La fonction de création a retourné un pointeur null");
                return std::unique_ptr<T, std::function<void(T*)>>(
                    sharedInstance.get(),
                    [](T* /*ptr*/) { }
                );
            } catch (const std::exception& e) {
                using RawCreatorFunc = T* (*)();
                RawCreatorFunc rawCreator = getSymbol<RawCreatorFunc>(creatorName);
                T* instance = rawCreator();
                if (!instance)
                    throw std::runtime_error("La fonction de création a retourné un pointeur null");
                return std::unique_ptr<T, std::function<void(T*)>>(instance, std::default_delete<T>());
            }
        }

    private:
        void* _handle = nullptr;
};
