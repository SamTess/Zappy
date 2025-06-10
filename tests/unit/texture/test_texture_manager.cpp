/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Tests unitaires pour TextureManager
*/

#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <memory>
#include <iostream>
#include "../../../src/GUI/textureManager/TextureManager.hpp"
#include "../mocks/MockGraphicsLib.hpp"

// Redirection des flux de sortie pour tester les erreurs
void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(TextureManager, getInstance, .description = "Test de récupération de l'instance du singleton")
{
    TextureManager& instance1 = TextureManager::getInstance();
    TextureManager& instance2 = TextureManager::getInstance();
    
    cr_assert_eq(&instance1, &instance2, "Le singleton doit retourner la même instance");
}

Test(TextureManager, loadTextureWithoutGraphicsLib, .init = redirect_all_std, 
    .description = "Test du chargement de texture sans bibliothèque graphique initialisée")
{
    TextureManager& manager = TextureManager::getInstance();
    // On s'assure que la lib graphique est nullptr
    manager.setGraphicsLib(nullptr);
    
    int result = manager.loadTexture("assets/models/castle_diffuse.png");
    
    cr_assert_eq(result, -1, "Le chargement doit échouer si la bibliothèque graphique n'est pas initialisée");
    // Vérifier le message d'erreur sur la sortie d'erreur
    fflush(stderr);
    cr_assert_stderr_eq_str("Erreur: GraphicsLib non initialisé dans TextureManager\n",
                         "Le message d'erreur doit informer que la lib n'est pas initialisée");
}

Test(TextureManager, loadAndUnloadTexture, .description = "Test du chargement et déchargement d'une texture")
{
    TextureManager& manager = TextureManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Test de chargement
    int textureId = manager.loadTexture("assets/models/castle_diffuse.png");
    cr_assert_gt(textureId, 0, "L'ID de texture doit être positif après chargement");
    cr_assert_eq(manager.getTextureCount(), 1, "Le compteur de textures doit être à 1");
    cr_assert(manager.hasTexture("assets/models/castle_diffuse.png"), "La texture doit être marquée comme chargée");
    cr_assert_eq(manager.getTextureId("assets/models/castle_diffuse.png"), textureId, 
                "La méthode getTextureId doit retourner le même ID");
    
    // Test de rechargement (doit retourner le même ID)
    int textureId2 = manager.loadTexture("assets/models/castle_diffuse.png");
    cr_assert_eq(textureId, textureId2, "Le rechargement doit retourner le même ID");
    cr_assert_eq(manager.getTextureCount(), 1, "Le compteur de textures doit rester à 1");
    
    // Test de déchargement
    manager.unloadTexture(textureId);
    cr_assert_eq(manager.getTextureCount(), 0, "Le compteur de textures doit être à 0 après déchargement");
    cr_assert_not(manager.hasTexture("assets/models/castle_diffuse.png"), 
                "La texture ne doit plus être marquée comme chargée");
    cr_assert_eq(manager.getTextureId("assets/models/castle_diffuse.png"), -1, 
                "getTextureId doit retourner -1 pour une texture non chargée");
}

Test(TextureManager, drawTexture, .init = redirect_all_std, 
    .description = "Test du dessin d'une texture")
{
    TextureManager& manager = TextureManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Charger une texture
    int textureId = manager.loadTexture("assets/models/castle_diffuse.png");
    
    // Dessiner la texture
    manager.drawTexture(textureId, 100, 200);
    cr_assert(mockGraphics->wasDrawTextureCalled, "La méthode DrawTexture2D du GraphicsLib doit être appelée");
    cr_assert_eq(mockGraphics->lastDrawTextureId, textureId, "L'ID de texture doit être transmis correctement");
    cr_assert_eq(mockGraphics->lastDrawTextureX, 100, "La coordonnée X doit être transmise correctement");
    cr_assert_eq(mockGraphics->lastDrawTextureY, 200, "La coordonnée Y doit être transmise correctement");
}

Test(TextureManager, drawInvalidTexture, .init = redirect_all_std,
    .description = "Test du dessin d'une texture invalide")
{
    TextureManager& manager = TextureManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    mockGraphics->textureReadyResponse = false;  // Simuler une texture non prête
    manager.setGraphicsLib(mockGraphics);
    
    // Dessiner une texture avec un ID invalide
    manager.drawTexture(999, 100, 200);
    
    fflush(stderr);
    cr_assert_stderr_eq_str("Erreur: Texture ID 999 non prête ou invalide\n",
                         "Le message d'erreur doit informer que la texture n'est pas prête");
}

Test(TextureManager, unloadAllTextures, .description = "Test du déchargement de toutes les textures")
{
    TextureManager& manager = TextureManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Charger quelques textures
    int tex1 = manager.loadTexture("assets/models/castle_diffuse.png");
    int tex2 = manager.loadTexture("assets/models/Cube/cube_diffuse.png");
    
    cr_assert_eq(manager.getTextureCount(), 2, "Le compteur de textures doit être à 2");
    
    // Décharger toutes les textures
    manager.unloadAllTextures();
    cr_assert_eq(manager.getTextureCount(), 0, "Le compteur de textures doit être à 0 après unloadAllTextures");
    cr_assert_not(manager.hasTexture("assets/models/castle_diffuse.png"), "La texture 1 ne doit plus être chargée");
    cr_assert_not(manager.hasTexture("assets/models/Cube/cube_diffuse.png"), "La texture 2 ne doit plus être chargée");
}

Test(TextureManager, unloadNonExistentTexture, .init = redirect_all_std,
    .description = "Test du déchargement d'une texture inexistante")
{
    TextureManager& manager = TextureManager::getInstance();
    auto mockGraphics = std::make_shared<MockGraphicsLib>();
    manager.setGraphicsLib(mockGraphics);
    
    // Tenter de décharger une texture qui n'existe pas
    manager.unloadTexture(999);
    
    fflush(stderr);
    cr_assert_stderr_eq_str("Tentative de libération d'une texture inexistante (ID: 999)\n",
                         "Un message d'erreur approprié doit être affiché");
}
