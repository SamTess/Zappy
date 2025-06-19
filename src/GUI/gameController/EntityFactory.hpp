/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EntityFactory - Factory pour créer les entités de jeu de manière polymorphe
*/

#ifndef ENTITY_FACTORY_HPP_
#define ENTITY_FACTORY_HPP_

#include <memory>
#include "IGameEntity.hpp"
#include "ITile.hpp"
#include "IPlayer.hpp"
#include "IPlayerInventory.hpp"
#include "IEgg.hpp"
#include "IResource.hpp"
#include "../network/protocol/messageData/MessageDataAll.hpp"

class Player;
class PlayerInventory;
class Egg;
class Tile;
class Resource;

class IEntityFactory {
public:
    virtual ~IEntityFactory() = default;
    virtual std::shared_ptr<IPlayer> createPlayer(const PlayerInfoData& data) = 0;
    virtual std::shared_ptr<IPlayerInventory> createPlayerInventory(const PlayerInventoryData& data) = 0;
    virtual std::shared_ptr<IEgg> createEgg(const EggData& data) = 0;
    virtual std::shared_ptr<ITile> createTile(int x, int y) = 0;
    virtual std::shared_ptr<ITile> createTile(const TileContentData& data) = 0;
    virtual std::shared_ptr<IResource> createResource(ResourceType type, int quantity = 0) = 0;
};

class GameEntityFactory : public IEntityFactory {
public:
    std::shared_ptr<IPlayer> createPlayer(const PlayerInfoData& data) override;
    std::shared_ptr<IPlayerInventory> createPlayerInventory(const PlayerInventoryData& data) override;
    std::shared_ptr<IEgg> createEgg(const EggData& data) override;
    std::shared_ptr<ITile> createTile(int x, int y) override;
    std::shared_ptr<ITile> createTile(const TileContentData& data) override;
    std::shared_ptr<IResource> createResource(ResourceType type, int quantity = 0) override;
};

class EntityFactoryManager {
public:
    EntityFactoryManager();
    explicit EntityFactoryManager(std::unique_ptr<IEntityFactory> factory);
    void setFactory(std::unique_ptr<IEntityFactory> factory);
    IEntityFactory& getFactory();

private:
    std::unique_ptr<IEntityFactory> _factory;
};

#endif /* !ENTITY_FACTORY_HPP_ */
