/*
** EPITECH PROJECT, 2025
** B-YEP-400
** File description:
** Raylib texture encapsulation
*/

#include "Texture.hpp"
#include <raylib.h>
#include <rlgl.h>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <string>

namespace raylibcpp {

Texture::Texture(const std::string& path) {
    texture = LoadTexture(path.c_str());
}

Texture::~Texture() {
    UnloadTexture(texture);
}

void Texture::draw(int x, int y) const {
    DrawTexture(texture, x, y, WHITE);
}

Texture2D Texture::get() const {
    return texture;
}

Texture3DWrap::Texture3DWrap(const std::string& path) {
    texture = LoadTexture(path.c_str());
    if (texture.id == 0)
        throw std::runtime_error("Erreur chargement texture: " + path);
}

Texture3DWrap::~Texture3DWrap() {
    UnloadTexture(texture);
}

void Texture3DWrap::bind(int unit) const {
    rlActiveTextureSlot(unit);
    rlEnableTexture(texture.id);
}

Texture2D& Texture3DWrap::get() {
    return texture;
}

ModelWrap::ModelWrap(const std::string& objPath) {
    model = LoadModel(objPath.c_str());
    if (model.meshCount == 0)
        throw std::runtime_error("Erreur chargement model: " + objPath);
    
    std::string basePath = objPath.substr(0, objPath.find_last_of("/\\") + 1);
    bool hasTextures = false;
    for (int i = 0; i < model.materialCount; i++) {
        if (model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id > 0) {
            hasTextures = true;
            break;
        }
    }
    if (!hasTextures && model.materialCount > 0) {
        std::cout << "Aucune texture chargée automatiquement pour: " << objPath << std::endl;

        if (objPath.find("girl.obj") != std::string::npos) {
            std::cout << "Détection du modèle girl.obj - Tentative de chargement spécifique..." << std::endl;

            std::string textureDir = basePath + "tEXTURE/";
            std::string textureDirAlt = basePath + "texture/";
            std::string texturePath;

            std::vector<std::string> girlTextures = {
                "COLORS.jpg", 
                "BOdy Skin Base Color.png",
                "FACE Base Color apha.png",
                "top color.png",
                "bot color.jpg"
            };
            
            for (const auto& texName : girlTextures) {
                texturePath = textureDir + texName;
                
                if (FileExists(texturePath.c_str())) {
                    std::cout << "Chargement de texture spécifique: " << texturePath << std::endl;
                    Texture2D texture = LoadTexture(texturePath.c_str());
                    if (texture.id > 0) {
                        for (int i = 0; i < model.materialCount; i++) {
                            if (model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id == 0) {
                                model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                            }
                        }
                        hasTextures = true;
                        break;
                    }
                }
                texturePath = textureDirAlt + texName;
                if (FileExists(texturePath.c_str())) {
                    std::cout << "Chargement de texture alternative: " << texturePath << std::endl;
                    Texture2D texture = LoadTexture(texturePath.c_str());
                    if (texture.id > 0) {
                        for (int i = 0; i < model.materialCount; i++) {
                            if (model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id == 0) {
                                model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                            }
                        }
                        hasTextures = true;
                        break;
                    }
                }
            }
            if (hasTextures) {
                std::cout << "Texture chargée avec succès pour le modèle girl.obj" << std::endl;
                return;
            }
        }
        std::vector<std::string> possibleTextureDirs = {
            "",
            "textures/",
            "texture/",
            "tEXTURE/",
            "TEXTURE/",
            "Texture/"
        };
        
        std::vector<std::string> possibleTextureNames = {
            "texture.png", "diffuse.png", "albedo.png", "basecolor.png", 
            "color.png", "texture.jpg", "diffuse.jpg", "albedo.jpg",
            "main.png", "main.jpg", "base.png", "base.jpg"
        };
        for (const auto& dir : possibleTextureDirs) {
            for (const auto& texName : possibleTextureNames) {
                std::string texturePath = basePath + dir + texName;
                
                if (FileExists(texturePath.c_str())) {
                    std::cout << "Texture trouvée: " << texturePath << std::endl;
                    Texture2D texture = LoadTexture(texturePath.c_str());
                    if (texture.id > 0) {
                        for (int i = 0; i < model.materialCount; i++) {
                            if (model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture.id == 0) {
                                model.materials[i].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
                            }
                        }
                        hasTextures = true;
                        break;
                    }
                }
            }
            if (hasTextures) break;
        }
        if (!hasTextures) {
            std::cout << "Aucune texture trouvée pour le modèle: " << objPath << std::endl;
        }
    }
}

ModelWrap::~ModelWrap() {
    UnloadModel(model);
}

void ModelWrap::draw(Vector3 position, float scale, Color tint) const {
    DrawModel(model, position, scale, tint);
}

Model& ModelWrap::get() {
    return model;
}

}  // namespace raylibcpp
