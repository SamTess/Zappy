/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour ModelManager
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <memory>
#include <iostream>
#include "../../../src/GUI/textureManager/ModelManager.hpp"
#include "../mocks/MockGraphicsLib.hpp"

// Redirection des flux de sortie pour tester les erreurs
void redirect_all_std_model(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(ModelManager, getInstance, .description = "Test de récupération de l'instance du singleton")
{
    ModelManager& instance1 = ModelManager::getInstance();
    ModelManager& instance2 = ModelManager::getInstance();
    
    cr_assert_eq(&instance1, &instance2, "Le singleton doit retourner la même instance");
}

Test(ModelManager, loadModelWithoutGraphicsLib, .init = redirect_all_std_model, 
    .description = "Test du chargement de modèle sans bibliothèque graphique initialisée")
{
    ModelManager& manager = ModelManager::getInstance();
    // On s'assure que la lib graphique est nullptr
    manager.setGraphicsLib(nullptr);
    
    int result = manager.loadModel("assets/models/castle.obj");
    
    cr_assert_eq(result, -1, "Le chargement doit échouer si la bibliothèque graphique n'est pas initialisée");
    fflush(stderr);
    cr_assert_stderr_eq_str("Erreur: GraphicsLib non initialisé dans ModelManager\n",
                         "Le message d'erreur doit informer que la lib n'est pas initialisée");
}

Test(ModelManager, loadAndUnloadModel, .description = "Test du chargement et déchargement d'un modèle")
{
    ModelManager& manager = ModelManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Important: Initialiser également le TextureManager avec la même instance mockGraphics
    TextureManager& textureManager = TextureManager::getInstance();
    textureManager.setGraphicsLib(mockGraphics);
    
    // Test de chargement
    int modelId = manager.loadModel("assets/models/castle.obj", "assets/models/castle_diffuse.png");
    cr_assert_gt(modelId, 0, "L'ID de modèle doit être positif après chargement");
    cr_assert_eq(manager.getModelCount(), 1, "Le compteur de modèles doit être à 1");
    cr_assert(manager.hasModel("assets/models/castle.obj"), "Le modèle doit être marqué comme chargé");
    cr_assert_eq(manager.getModelId("assets/models/castle.obj"), modelId, 
                "La méthode getModelId doit retourner le même ID");
    
    // Test de rechargement (doit retourner le même ID)
    int modelId2 = manager.loadModel("assets/models/castle.obj");
    cr_assert_eq(modelId, modelId2, "Le rechargement doit retourner le même ID");
    cr_assert_eq(manager.getModelCount(), 1, "Le compteur de modèles doit rester à 1");
    
    // Test de déchargement
    manager.unloadModel(modelId);
    cr_assert_eq(manager.getModelCount(), 0, "Le compteur de modèles doit être à 0 après déchargement");
    cr_assert_not(manager.hasModel("assets/models/castle.obj"), 
                "Le modèle ne doit plus être marqué comme chargé");
    cr_assert_eq(manager.getModelId("assets/models/castle.obj"), -1, 
                "getModelId doit retourner -1 pour un modèle non chargé");
}

Test(ModelManager, loadModelWithMultipleTextures, .description = "Test du chargement d'un modèle avec plusieurs textures")
{
    ModelManager& manager = ModelManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Important: Initialiser également le TextureManager avec la même instance mockGraphics
    TextureManager& textureManager = TextureManager::getInstance();
    textureManager.setGraphicsLib(mockGraphics);
    
    std::vector<std::string> texturePaths = {
        "assets/models/castle_diffuse.png"
    };
    
    int modelId = manager.loadModelWithTextures("assets/models/castle.obj", texturePaths);
    cr_assert_gt(modelId, 0, "L'ID de modèle doit être positif après chargement");
    cr_assert_eq(manager.getModelCount(), 1, "Le compteur de modèles doit être à 1");
    
    // Vérifier que le modèle existe dans le système
    cr_assert(manager.hasModel("assets/models/castle.obj"), "Le modèle doit être marqué comme chargé");
    
    // Vérifier que les textures ont été assignées (difficile à tester sans accès direct à la structure Model3D)
    // On vérifie au moins que les appels à LoadTexture2D ont été faits sur le mock
    cr_assert_eq(mockGraphics->lastTexturePathsLoaded.size(), 1, "Une texture devrait avoir été chargée");
}

Test(ModelManager, drawModel, .init = redirect_all_std_model,
    .description = "Test du dessin d'un modèle")
{
    ModelManager& manager = ModelManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Important: Initialiser également le TextureManager avec la même instance mockGraphics
    TextureManager& textureManager = TextureManager::getInstance();
    textureManager.setGraphicsLib(mockGraphics);
    
    // Charger un modèle
    int modelId = manager.loadModel("assets/models/castle.obj", "assets/models/castle_diffuse.png");
    
    // Position pour le dessin
    ZappyTypes::Vector3 position = {10.0f, 5.0f, 20.0f};
    ZappyTypes::Color color = ZappyTypes::Colors::Z_WHITE;
    
    // Dessiner le modèle
    manager.drawModel(modelId, position, color);
    
    cr_assert(mockGraphics->wasDrawModelCalled, "La méthode DrawModel3D du GraphicsLib doit être appelée");
    cr_assert_float_eq(mockGraphics->lastDrawModelPosition.x, position.x, 0.001f, "La position X doit être transmise correctement");
    cr_assert_float_eq(mockGraphics->lastDrawModelPosition.y, position.y, 0.001f, "La position Y doit être transmise correctement");
    cr_assert_float_eq(mockGraphics->lastDrawModelPosition.z, position.z, 0.001f, "La position Z doit être transmise correctement");
}

Test(ModelManager, drawModelEx, .init = redirect_all_std_model,
    .description = "Test du dessin d'un modèle avec rotation")
{
    ModelManager& manager = ModelManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Important: Initialiser également le TextureManager avec la même instance mockGraphics
    TextureManager& textureManager = TextureManager::getInstance();
    textureManager.setGraphicsLib(mockGraphics);
    
    // Charger un modèle
    int modelId = manager.loadModel("assets/models/castle.obj", "assets/models/castle_diffuse.png");
    
    // Paramètres pour le dessin
    ZappyTypes::Vector3 position = {10.0f, 5.0f, 20.0f};
    ZappyTypes::Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
    float rotationAngle = 45.0f;
    float scale = 2.0f;
    
    // Dessiner le modèle avec rotation
    manager.drawModelEx(modelId, position, rotationAxis, rotationAngle, scale);
    
    cr_assert(mockGraphics->wasDrawModelExCalled, "La méthode DrawModelEx du GraphicsLib doit être appelée");
    cr_assert_float_eq(mockGraphics->lastDrawModelExPosition.x, position.x, 0.001f, "La position X doit être transmise correctement");
    cr_assert_float_eq(mockGraphics->lastDrawModelExPosition.y, position.y, 0.001f, "La position Y doit être transmise correctement");
    cr_assert_float_eq(mockGraphics->lastDrawModelExPosition.z, position.z, 0.001f, "La position Z doit être transmise correctement");
    cr_assert_float_eq(mockGraphics->lastDrawModelExRotationAxis.x, rotationAxis.x, 0.001f, "L'axe de rotation X doit être transmis correctement");
    cr_assert_float_eq(mockGraphics->lastDrawModelExRotationAxis.y, rotationAxis.y, 0.001f, "L'axe de rotation Y doit être transmis correctement");
    cr_assert_float_eq(mockGraphics->lastDrawModelExRotationAxis.z, rotationAxis.z, 0.001f, "L'axe de rotation Z doit être transmis correctement");
    cr_assert_float_eq(mockGraphics->lastDrawModelExAngle, rotationAngle, 0.001f, "L'angle de rotation doit être transmis correctement");
    cr_assert_float_eq(mockGraphics->lastDrawModelExScale, scale, 0.001f, "L'échelle doit être transmise correctement");
}

Test(ModelManager, drawInvalidModel, .init = redirect_all_std_model,
    .description = "Test du dessin d'un modèle invalide")
{
    ModelManager& manager = ModelManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Important: Initialiser également le TextureManager avec la même instance mockGraphics
    TextureManager& textureManager = TextureManager::getInstance();
    textureManager.setGraphicsLib(mockGraphics);
    
    // Position pour le dessin
    ZappyTypes::Vector3 position = {10.0f, 5.0f, 20.0f};
    
    // Tenter de dessiner un modèle avec un ID invalide
    manager.drawModel(999, position);
    
    fflush(stderr);
    cr_assert_stderr_eq_str("Erreur: Modèle ID 999 non trouvé\n",
                         "Le message d'erreur doit informer que le modèle n'existe pas");
}

Test(ModelManager, unloadAllModels, .description = "Test du déchargement de tous les modèles")
{
    ModelManager& manager = ModelManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);

    // Charger quelques modèles
    int model1 = manager.loadModel("assets/models/castle.obj", "assets/models/castle_diffuse.png");
    int model2 = manager.loadModel("assets/models/Cube/cube.obj", "assets/models/Cube/cube_diffuse.png");
    
    cr_assert_eq(manager.getModelCount(), 2, "Le compteur de modèles doit être à 2");
    
    // Décharger tous les modèles
    manager.unloadAllModels();
    cr_assert_eq(manager.getModelCount(), 0, "Le compteur de modèles doit être à 0 après unloadAllModels");
    cr_assert_not(manager.hasModel("assets/models/castle.obj"), "Le modèle 1 ne doit plus être chargé");
    cr_assert_not(manager.hasModel("assets/models/Cube/cube.obj"), "Le modèle 2 ne doit plus être chargé");
}

Test(ModelManager, unloadNonExistentModel, .init = redirect_all_std_model,
    .description = "Test du déchargement d'un modèle inexistant")
{
    ModelManager& manager = ModelManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Tenter de décharger un modèle qui n'existe pas
    manager.unloadModel(999);
    
    fflush(stderr);
    cr_assert_stderr_eq_str("Tentative de libération d'un modèle inexistant (ID: 999)\n",
                         "Un message d'erreur approprié doit être affiché");
}

Test(ModelManager, getBoundingBox, .description = "Test de récupération de la boîte englobante")
{
    ModelManager& manager = ModelManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);

    // Charger un modèle
    int modelId = manager.loadModel("assets/models/castle.obj", "assets/models/castle_diffuse.png");

    // Récupérer la boîte englobante
    auto [min, max] = manager.getBoundingBox(modelId);

    // Par défaut, les valeurs devraient être initialisées à zéro dans notre implémentation
    // Mais les tests vérifient juste que la fonction retourne quelque chose sans erreur
    cr_assert_float_eq(min.x, 0.0f, 0.001f, "BoundingBox min X doit être correctement initialisé");
    cr_assert_float_eq(min.y, 0.0f, 0.001f, "BoundingBox min Y doit être correctement initialisé");
    cr_assert_float_eq(min.z, 0.0f, 0.001f, "BoundingBox min Z doit être correctement initialisé");
    cr_assert_float_eq(max.x, 0.0f, 0.001f, "BoundingBox max X doit être correctement initialisé");
    cr_assert_float_eq(max.y, 0.0f, 0.001f, "BoundingBox max Y doit être correctement initialisé");
    cr_assert_float_eq(max.z, 0.0f, 0.001f, "BoundingBox max Z doit être correctement initialisé");
}
