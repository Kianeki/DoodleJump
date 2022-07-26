//
// Created by Kiani on 17/12/2021.
//

#include "EntityModel.h"
void EntityModel::drawEntity(Camera& camera)
{
        std::pair<float, float> scaledPos = camera.scaledPosition(position.first, position.second);
        notify(Alert::drawRequest, scaledPos); // then draw the entity
}
void EntityModel::moveEntityOnPlatform(const EntityModel& platform)
{
        position.first = platform.getPosition().first;
        position.second = platform.getPosition().second + platform.getHeight() + height;
}
bool EntityModel::fallingCollide(const EntityModel& entityModel) const
{

        if (entityModel.getCurrentSpeed() > 0.f) {
                // if player is jumping we do not need to check platform/spring/spike collision
                return false;
        }
        std::pair<float, float> collisionEntityPosition = entityModel.getPosition(); // centre of the player
        collisionEntityPosition.second -= entityModel.getHeight();
        float precision = 0.015f;

        // increase precision if collisionEntity is going slow
        if (entityModel.getCurrentSpeed() >= -0.5f) {
                precision = 0.005f;
        }
        // check if collisionEntity is partially inside/ on top of platform/spring/spike
        if ((collisionEntityPosition.second - (position.second + height)) >= -precision) {
                return true;
        }
        return false;
}
bool EntityModel::getsHit(int damage) { return false; }
bool EntityModel::updateJetpack(const EntityModel& player) { return false; }
bool EntityModel::isDead() { return false; }
int EntityModel::getType() const { return 0; }
bool EntityModel::deleteOnCollision() { return false; }
bool EntityModel::collide(EntityModel& playerModel) { return false; }
void EntityModel::setPosition(std::pair<float, float> newPosition) { position = newPosition; }
float EntityModel::getWidth() const { return width; }
float EntityModel::getHeight() const { return height; }
float EntityModel::getCurrentSpeed() const { return 0.f; }
float EntityModel::getJumpSpeed() const { return 0.f; }
PlatformModel::PlatformModel(float x, float y, PlatformType::Type randomType) : EntityModel(x, y), type(randomType)
{
        width = 0.25f;
        height = 0.02f; // 0.02
        // set their speed (default 0) and bounds in which they can move/teleport
        if (randomType == PlatformType::horizontalP) {
                platformSpeed = 0.5f;
                lowerBound = -1 + width;
                upperBound = 1 - width;
        } else if (randomType == PlatformType::verticalP) {
                platformSpeed = 0.2f;
                lowerBound = y - 4 * height;
                upperBound = y + 4 * height;
        } else if (randomType == PlatformType::verticalPteleport) {
                lowerBound = y - 6 * height;
                upperBound = y + 6 * height;
        }
}
bool PlatformModel::collide(EntityModel& entityModel)
{
        bool collision = false;
        if (fallingCollide(entityModel)) {
                collision = true;
                entityModel.jump();
                if (jumpedOn) {
                        entityModel.decreaseScore(type);
                }
                jumpedOn = true;
                if (type == PlatformType::horizontalPteleport) {
                        std::pair<float, float> newPos = getPosition();
                        std::shared_ptr<Random> random = Random::getInstance();
                        newPos.first = random->randomPlatformX(getWidth());
                        setPosition(newPos);
                } else if (getType() == PlatformType::verticalPteleport) {
                        std::pair<float, float> newPos = getPosition();
                        std::shared_ptr<Random> random = Random::getInstance();
                        newPos.second = random->randomTeleportPlatformY(getLowerBound(), getUpperBound());
                        setPosition(newPos);
                }
        }

        return collision;
}
bool PlatformModel::deleteOnCollision()
{
        if (type == PlatformType::temporaryP) {
                return true;
        }
        return false;
}
void PlatformModel::movePlatform()
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
int PlatformModel::getType() const { return type; }
float PlatformModel::getLowerBound() const { return lowerBound; }
float PlatformModel::getUpperBound() const { return upperBound; }

BulletModel::BulletModel(float x, float y, BulletType::Type bulletType) : EntityModel(x, y)
{
        width = 0.04f;
        height = 0.04f;
        if (bulletType == BulletType::friendly) {
                speedY = 1.2f;
        } else if (bulletType == BulletType::enemy) {
                speedY = -0.8f;
        }
}
bool BulletModel::collide(EntityModel& entityModel)
{
        // returns true if it hits appropriate entity and deals damage to its HP
        return entityModel.getsHit(damage);
}
void BulletModel::move()
{
        float timePerFrame = Stopwatch::getInstance()->getTimePerFrame();
        position.second += speedY * timePerFrame;
}
bool BulletModel::deleteOnCollision() { return true; }
bool LivingEntityModel::isDead()
{
        if (HP == 0) {
                return true;
        }
        return false;
}
bool LivingEntityModel::getsHit(int damage)
{
        if (!isImmune()) {
                notify(Alert::decreaseScore, std::pair<int, int>(1000 * damage, 0));
                decreaseHP(damage);
                grantImmunity();
                return true;
        }
        return false;
}
void LivingEntityModel::grantImmunity() { currentImmunityFrames = immunityFrames; }
void LivingEntityModel::updateImmunity()
{
        if (currentImmunityFrames > 0) {
                currentImmunityFrames--;
        }
}
bool LivingEntityModel::isImmune()
{
        if (currentImmunityFrames > 0) {
                return true;
        } else
                return false;
}
void LivingEntityModel::increaseHP(int value)
{
        if (HP < maxHP && HP > 0) {
                notify(Alert::increaseHP, std::pair<int, int>(value, 0));
                HP += value;
        }
}
void LivingEntityModel::decreaseHP(int value)
{
        if (HP > 0) {
                notify(Alert::decreaseHP, std::pair<int, int>(value, 0));
                HP -= value;
        }
}
int LivingEntityModel::getMaxHp() const { return maxHP; }
EnemyModel::EnemyModel(const std::unique_ptr<PlatformModel>& platform, EnemyType::Type etype)
    : LivingEntityModel(platform->getPosition().first, platform->getPosition().second)
{
        width = 0.1f;
        height = 0.1f;
        type = etype;
        if (type == EnemyType::weak) {
                maxHP = 1;
        } else if (type == EnemyType::strong) {
                maxHP = 3;
        }
        HP = maxHP;
        position.second += platform->getHeight();
}
bool EnemyModel::collide(EntityModel& entityModel)
{
        entityModel.getsHit(1);
        return false;
}
int EnemyModel::getType() const { return type; }
PlayerModel::PlayerModel(float x, float y) : LivingEntityModel(x, y)
{
        width = 0.1f;
        height = 0.1f;
        maxHP = 5;
        HP = maxHP;
        immunityFrames = 0.75 * (1.f / Stopwatch::getInstance()->getTimePerFrame());
}
void PlayerModel::gameOver() { notify(Alert::gameOver, {0.f, 0.f}); }
void PlayerModel::jump() { currentSpeedY = jumpSpeed; }
void PlayerModel::movePlayer()
{
        updateImmunity(); // updates the immunity frames (movePlayer gets called each frame)

        float timePerFrame = Stopwatch::getInstance()->getTimePerFrame();
        currentSpeedY -= gravity * timePerFrame;
        position.second += currentSpeedY * timePerFrame;
        float horizontalMovement = 0.f;
        horizontalMovement = maxPlayerSpeedX * timePerFrame * direction;
        position.first += horizontalMovement;
        if (position.first > 1.f || position.first < -1.f) {
                position.first -= 2.f * direction;
        }
}
float PlayerModel::getCurrentSpeed() const { return currentSpeedY; }
void PlayerModel::setCurrentSpeedY(float newSpeed) { currentSpeedY = newSpeed; }
void PlayerModel::setPlayerDirection(PlayerMovement::Direction dir) { direction = dir; }
float PlayerModel::getJumpSpeed() const { return jumpSpeed; }
void PlayerModel::increaseScore(int scoreIncrease)
{ // entities can update score through player
        notify(Alert::increaseScore, std::pair<int, int>(scoreIncrease, 0));
}
void PlayerModel::decreaseScore(int scoreIncrease)
{ // entities can update score through player
        notify(Alert::decreaseScore, std::pair<int, int>(scoreIncrease, 0));
}
BonusModel::BonusModel(const std::unique_ptr<PlatformModel>& platform, BonusType::Type type)
    : EntityModel(platform->getPosition().first, platform->getPosition().second)
{
        switch (type) {
        case BonusType::spring:
                width = 0.05f;
                height = 0.02f;
                break;
        case BonusType::jetpack:
                width = 0.06f;
                height = 0.06f;
                break;
        case BonusType::heart:
                width = 0.05f;
                height = 0.03f;
                break;
        case BonusType::spike:
                width = 0.10f;
                height = 0.02f;
                break;
        }

        position.second += platform->getHeight() + height;
        btype = type;
}
bool BonusModel::collide(EntityModel& entityModel)
{

        float playerSpeed = entityModel.getCurrentSpeed();
        switch (btype) {
        case BonusType::spring:
                if (fallingCollide(entityModel)) {
                        playerSpeed = entityModel.getJumpSpeed();
                        playerSpeed *= sqrt(5); // 5x as high jump
                } else {
                        return false;
                }
                break;
        case BonusType::jetpack:
                playerSpeed = entityModel.getJumpSpeed();
                playerSpeed *= sqrt(20);
                break;
        case BonusType::heart:
                entityModel.increaseHP(1);
                break;
        case BonusType::spike:
                if (fallingCollide(entityModel)) {
                        playerSpeed = entityModel.getJumpSpeed();
                        entityModel.decreaseHP(1);
                } else {
                        return false;
                }
                break;
        }
        entityModel.setCurrentSpeedY(playerSpeed);
        entityModel.increaseScore(btype); // increases score based on the bonustype that's picked up
        return true;
}
bool BonusModel::deleteOnCollision()
{
        if (btype == BonusType::heart) {
                return true;
        }
        return false;
}
int BonusModel::getType() const { return btype; }
bool BonusModel::updateJetpack(const EntityModel& player)
{
        if (player.getCurrentSpeed() < player.getJumpSpeed()) {
                return false;
        }
        std::pair<float, float> playerPosition = player.getPosition();
        float playerWidth = player.getWidth();
        float offset = playerWidth + width;
        // offset will determine how much the jetpack has to be "pushed" from the player centre
        position.first = playerPosition.first + offset;
        position.second = playerPosition.second;
        return true;
}
BGTileModel::BGTileModel(float x, float y) : EntityModel(x, y)
{
        width = 0.1f;
        height = 0.1f;
}
