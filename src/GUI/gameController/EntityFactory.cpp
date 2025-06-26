/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** EntityFactory
*/

#include <memory>
#include <string>
#include <utility>
#include "EntityFactory.hpp"
#include "entities/Player.hpp"
#include "entities/Egg.hpp"
#include "entities/Resource.hpp"
#include "entities/Tile.hpp"
#include "entities/PlayerInventory.hpp"

std::shared_ptr<IPlayer> GameEntityFactory::createPlayer(const PlayerInfoData& data) {
    return std::make_shared<Player>(data);
}

std::shared_ptr<IPlayerInventory> GameEntityFactory::createPlayerInventory(const PlayerInventoryData& data) {
    return std::make_shared<PlayerInventory>(data);
}

std::shared_ptr<IEgg> GameEntityFactory::createEgg(const EggData& data) {
    return std::make_shared<Egg>(data);
}

std::shared_ptr<ITile> GameEntityFactory::createTile(int x, int y) {
    return std::make_shared<Tile>(x, y);
}

std::shared_ptr<ITile> GameEntityFactory::createTile(const TileContentData& data) {
    return std::make_shared<Tile>(data);
}

std::shared_ptr<IResource> GameEntityFactory::createResource(ResourceType type, int quantity) {
    return std::make_shared<Resource>(type, quantity);
}

EntityFactoryManager::EntityFactoryManager() {
    _factory = std::make_unique<GameEntityFactory>();
}

EntityFactoryManager::EntityFactoryManager(std::unique_ptr<IEntityFactory> factory)
    : _factory(std::move(factory)) {
}

void EntityFactoryManager::setFactory(std::unique_ptr<IEntityFactory> factory) {
    _factory = std::move(factory);
}

IEntityFactory& EntityFactoryManager::getFactory() {
    if (!_factory) {
        _factory = std::make_unique<GameEntityFactory>();
    }
    return *_factory;
}

std::shared_ptr<IPlayer> EntityFactoryManager::createPlayer(const PlayerInfoData& data) {
    return getFactory().createPlayer(data);
}

std::shared_ptr<IPlayerInventory> EntityFactoryManager::createPlayerInventory(const PlayerInventoryData& data) {
    return getFactory().createPlayerInventory(data);
}

std::shared_ptr<IEgg> EntityFactoryManager::createEgg(const EggData& data) {
    return getFactory().createEgg(data);
}

std::shared_ptr<ITile> EntityFactoryManager::createTile(int x, int y) {
    return getFactory().createTile(x, y);
}

std::shared_ptr<ITile> EntityFactoryManager::createTile(const TileContentData& data) {
    return getFactory().createTile(data);
}

std::shared_ptr<IResource> EntityFactoryManager::createResource(ResourceType type, int quantity) {
    return getFactory().createResource(type, quantity);
}
