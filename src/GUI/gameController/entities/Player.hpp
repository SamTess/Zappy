/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include <string>
#include <memory>
#include "../IGameEntity.hpp"
#include "../../network/protocol/messageData/MessageDataAll.hpp"
#include "../../../Shared/IGraphicsLib.hpp"
#include "../IPlayer.hpp"

class Player : public IPlayer {
public:
    Player();
    explicit Player(const PlayerInfoData& data);

    int getId() const override;
    int getX() const override;
    int getY() const override;
    void setPosition(int x, int y) override;

    int getLevel() const override;
    int getOrientation() const override;
    const std::string& getTeamName() const override;
    bool isAlive() const override;
    void updateFromProtocol(const PlayerInfoData& data) override;

    void setLevel(int level) override;
    void setOrientation(int orientation) override;
    void setTeamName(const std::string& teamName) override;
    void setIsAlive(bool alive) override;

    float getCurrentRotation() const override;
    float getLastRotation() const override;

    void updateAnimation(float deltaTime);
    ZappyTypes::Vector3 getInterpolatedPosition() const;
    bool isMoving() const;

    // Implémentation IRenderable
    void render(const std::shared_ptr<IGraphicsLib>& graphicsLib,
                const ZappyTypes::Vector3& position,
                float tileSize) const override;

    void renderPlayer(const std::shared_ptr<IGraphicsLib>& graphicsLib,
        const ZappyTypes::Vector3& position,
        float tileSize,
        int playerIndex = 0,
        int totalPlayers = 1) const override;

private:
    ZappyTypes::Color generateTeamColor() const;
    float orientationToRotationDegrees(int orientation) const;
    void startMovement(int newX, int newY);

    int _id;
    int _x;
    int _y;
    int _orientation;
    int _level;
    std::string _teamName;
    bool _isAlive;
    float _lastRotation;
    float _currentRotation;

    float _startX;
    float _startY;
    float _targetX;
    float _targetY;
    float _movementProgress;
    float _movementDuration;
    bool _isMoving;
};

#endif /* !PLAYER_HPP_ */
