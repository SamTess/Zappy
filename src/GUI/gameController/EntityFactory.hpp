/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EntityFactory
*/

#ifndef ENTITY_FACTORY_HPP_
#define ENTITY_FACTORY_HPP_

#include <memory>
#include <string>
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
        virtual std::shared_ptr<IPlayer> createPlayer(const PlayerInfoData& data) const = 0;
        virtual std::shared_ptr<IPlayerInventory> createPlayerInventory(const PlayerInventoryData& data) const = 0;
        virtual std::shared_ptr<IEgg> createEgg(const EggData& data) const = 0;
        virtual std::shared_ptr<ITile> createTile(int x, int y) const = 0;
        virtual std::shared_ptr<ITile> createTile(const TileContentData& data) const = 0;
        virtual std::shared_ptr<IResource> createResource(ResourceType type, int quantity = 0) const = 0;
};

class GameEntityFactory : public IEntityFactory {
    public:
        std::shared_ptr<IPlayer> createPlayer(const PlayerInfoData& data) const override;
        std::shared_ptr<IPlayerInventory> createPlayerInventory(const PlayerInventoryData& data) const override;
        std::shared_ptr<IEgg> createEgg(const EggData& data) const override;
        std::shared_ptr<ITile> createTile(int x, int y) const override;
        std::shared_ptr<ITile> createTile(const TileContentData& data) const override;
        std::shared_ptr<IResource> createResource(ResourceType type, int quantity = 0) const override;
};

class EntityFactoryManager {
    public:
        EntityFactoryManager();
        explicit EntityFactoryManager(std::unique_ptr<IEntityFactory> factory);
        void setFactory(std::unique_ptr<IEntityFactory> factory);
        const IEntityFactory& getFactory();

        std::shared_ptr<IPlayer> createPlayer(const PlayerInfoData& data);
        std::shared_ptr<IPlayerInventory> createPlayerInventory(const PlayerInventoryData& data);
        std::shared_ptr<IEgg> createEgg(const EggData& data);
        std::shared_ptr<ITile> createTile(int x, int y);
        std::shared_ptr<ITile> createTile(const TileContentData& data);
        std::shared_ptr<IResource> createResource(ResourceType type, int quantity = 0);

    private:
        std::unique_ptr<IEntityFactory> _factory;
};

#endif /* !ENTITY_FACTORY_HPP_ */
