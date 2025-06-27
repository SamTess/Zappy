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

std::shared_ptr<IPlayer> GameEntityFactory::createPlayer(const PlayerInfoData& data) const {
    return std::make_shared<Player>(data);
}

std::shared_ptr<IPlayerInventory> GameEntityFactory::createPlayerInventory(const PlayerInventoryData& data) const {
    return std::make_shared<PlayerInventory>(data);
}

std::shared_ptr<IEgg> GameEntityFactory::createEgg(const EggData& data) const {
    return std::make_shared<Egg>(data);
}

std::shared_ptr<ITile> GameEntityFactory::createTile(int x, int y) const {
    return std::make_shared<Tile>(x, y);
}

std::shared_ptr<ITile> GameEntityFactory::createTile(const TileContentData& data) const {
    return std::make_shared<Tile>(data);
}

std::shared_ptr<IResource> GameEntityFactory::createResource(ResourceType type, int quantity) const {
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

const IEntityFactory& EntityFactoryManager::getFactory() {
    if (!_factory)
        _factory = std::make_unique<GameEntityFactory>();
    return *_factory;
}

std::shared_ptr<IPlayer> EntityFactoryManager::createPlayer(const PlayerInfoData& data) {
    return _factory->createPlayer(data);
}

std::shared_ptr<IPlayerInventory> EntityFactoryManager::createPlayerInventory(const PlayerInventoryData& data) {
    return _factory->createPlayerInventory(data);
}

std::shared_ptr<IEgg> EntityFactoryManager::createEgg(const EggData& data) {
    return _factory->createEgg(data);
}

std::shared_ptr<ITile> EntityFactoryManager::createTile(int x, int y) {
    return _factory->createTile(x, y);
}

std::shared_ptr<ITile> EntityFactoryManager::createTile(const TileContentData& data) {
    return _factory->createTile(data);
}

std::shared_ptr<IResource> EntityFactoryManager::createResource(ResourceType type, int quantity) {
    return _factory->createResource(type, quantity);
}
