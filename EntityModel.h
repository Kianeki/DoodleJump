//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_ENTITYMODEL_H
#define TESTSFML_ENTITYMODEL_H

#include "Camera.h"
#include "Random.h"
#include "Stopwatch.h"
#include "Subject.h"
class EntityModel : public Subject
{
public:
        EntityModel(float x, float y) : position{x, y}, width{0}, height{0} {}
        virtual ~EntityModel() = default;
        //scales the coordinates of the entity via camera and gives them to the attached Observer(entityView)
        void drawEntity(Camera& camera)
        {
                std::pair<float, float> scaledPos = camera.scaledPosition(position.first, position.second);
                notify(Alert::drawRequest, scaledPos); // will scale the position, then draw the entity
        }

        std::pair<float, float> getPosition() const { return position; }
        float getWidth() const { return width; }
        float getHeight() const { return height; }

protected:
        std::pair<float, float> position{0, 0};
        float width{0.f};
        float height{0.f};
};
namespace PlayerMovement {
enum Direction
{
        left = -1,
        right = +1,
        none = 0
};
}
class PlatformModel : public EntityModel
{
public:
        PlatformModel(float x, float y, PlatformType::Type randomType) : EntityModel(x, y), type(randomType)
        {
                width = 0.25;
                height = 0.02; // 0.02
                if (randomType == PlatformType::horizontalP) {
                        platformSpeed = 0.5;
                        lowerBound = -1 + width;
                        upperBound = 1 - width;
                } else if (randomType == PlatformType::verticalP) {
                        platformSpeed = 0.2;
                        lowerBound = y - 4 * height;
                        upperBound = y + 4 * height;
                }
        }
        virtual ~PlatformModel() override = default;
        //Moves the PlatformModel in X or Y
        void movePlatform()
        {
                float timePerFrame = Stopwatch::getInstance()->getTimePerFrame();
                if (type == PlatformType::horizontalP) {
                        if (direction) {
                                position.first -= platformSpeed * timePerFrame;
                        } else {
                                position.first += platformSpeed * timePerFrame;
                        }
                        if (position.first < lowerBound || position.first > upperBound) {
                                direction = !direction;
                        }
                } else if (type == PlatformType::verticalP) {
                        if (direction) {
                                position.second -= platformSpeed * timePerFrame;
                        } else {
                                position.second += platformSpeed * timePerFrame;
                        }
                        if (position.second < lowerBound || position.second > upperBound) {
                                direction = !direction;
                        }
                }
        }
        PlatformType::Type getType() const { return type; }

private:
        PlatformType::Type type;
        bool direction = false;
        float lowerBound = 0;
        float upperBound = 0;
        float platformSpeed = 0;
};

class PlayerModel : public EntityModel
{
public:
        PlayerModel(float x, float y) : EntityModel(x, y)
        {
                width = 0.10f;
                height = 0.1f;
        }
        virtual ~PlayerModel() override = default;
        //Is used when the player reaches the bottom of the screen. Alerts score to save itself to file
        void dead() { notify(Alert::gameOver, {0, 0}); }
        //Moves the player in X and Y direction
        void movePlayer()
        {
                float timePerFrame = Stopwatch::getInstance()->getTimePerFrame();
                currentSpeedY -= gravity * timePerFrame;
                position.second += currentSpeedY * timePerFrame;
                float horizontalMovement = maxPlayerSpeedX * timePerFrame * direction;
                position.first += horizontalMovement;
                if (position.first > 1.f || position.first < -1.f) {
                        position.first -= 2.f * direction;
                }

        }
        //Holds ptr of platform with which player collided and bounces player off of platform
        //Also updates score if player collides with same platform twice
        void platformCollide(const std::shared_ptr<PlatformModel>& platformCollision)
        {
                if (platformCollision == lastJumpedOn) {
                        notify(Alert::decreaseScore, std::pair<int, int>(lastJumpedOn->getType(), 0));
                }
                lastJumpedOn = platformCollision;
                currentSpeedY = jumpSpeed;
        }
        float getCurrentSpeed() const { return currentSpeedY; }
        void setCurrentSpeed(float newSpeed) { currentSpeedY = newSpeed; }
        void setPlayerDirection(PlayerMovement::Direction dir) { direction = dir; }
        PlayerMovement::Direction getPlayerDirection() const { return direction; }
        float getJumpSpeed() const { return jumpSpeed; }
        void increaseScore(int scoreIncrease)
        { // world can update score through player
                notify(Alert::increaseScore, std::pair<int, int>(scoreIncrease, 0));
        }

private:
        float jumpSpeed = 1.6f;
        float maxPlayerSpeedX = 1.f;
        float currentSpeedY = 0.f;
        float gravity = 2.f;
        PlayerMovement::Direction direction = PlayerMovement::none;
        std::shared_ptr<PlatformModel> lastJumpedOn = nullptr;
};

class BonusModel : public EntityModel
{
public:
        BonusModel(const std::unique_ptr<PlatformModel>& platform, BonusType::Type type)
            : EntityModel(platform->getPosition().first, platform->getPosition().second)
        {
                switch (type) {
                case BonusType::spring:
                        width = 0.05;
                        height = 0.02;
                        break;
                case BonusType::jetpack:
                        width = 0.06;
                        height = 0.06;
                        break;
                }
                position.second += platform->getHeight() + height;
                btype = type;
        }
        virtual ~BonusModel() override = default;
        //Moves bonus according to the platform it's located on
        void movePlatformBonus(const std::shared_ptr<PlatformModel>& platform)
        {
                position.first = platform->getPosition().first;
                position.second = platform->getPosition().second + platform->getHeight() + height;
        }
        BonusType::Type getType() const { return btype; }
        //applies bonus to player
        void applyToPlayer(std::unique_ptr<PlayerModel>& player)
        {
                float playerSpeed = player->getCurrentSpeed();
                switch (btype) {
                case BonusType::spring:
                        playerSpeed *= sqrt(5); // 5x as high jump
                        break;
                case BonusType::jetpack:
                        playerSpeed *= sqrt(20);
                        break;
                }
                player->setCurrentSpeed(playerSpeed);
                player->increaseScore(btype); // increases score based on the bonustype that's picked up
        }
        // updates the bonus position according to the player interaction
        // currently only applies to jetpack
        bool update(const std::unique_ptr<PlayerModel>& player)
        {
                if (player->getCurrentSpeed() < player->getJumpSpeed()) {
                        return false;
                }
                PlayerMovement::Direction playerDirection = player->getPlayerDirection();
                std::pair<float, float> playerPosition = player->getPosition();
                float playerWidth = player->getWidth();
                float offset = playerWidth+width;
                // offset will determine how much the jetpack has to be "pushed" from the player centre
                if (playerDirection == PlayerMovement::left) {
                        position.first = playerPosition.first + offset;
                } else {
                        position.first = playerPosition.first - offset;
                }
                position.second = playerPosition.second;
                return true;
        }

private:
        BonusType::Type btype;
};

class BGTileModel : public EntityModel
{
public:
        BGTileModel(float x, float y) : EntityModel(x, y)
        {
                width = 0.1;
                height = 0.1;
        }
        virtual ~BGTileModel() override = default;
};
#endif // TESTSFML_ENTITYMODEL_H
