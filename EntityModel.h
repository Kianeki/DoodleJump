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
        // scales the coordinates of the entity via camera and gives them to the attached Observer(entityView)
        void drawEntity(Camera& camera)
        { // will scale the position
                std::pair<float, float> scaledPos = camera.scaledPosition(position.first, position.second);
                notify(Alert::drawRequest, scaledPos); // then draw the entity
        }
        std::pair<float, float> getPosition() const { return position; }
        float getWidth() const { return width; }
        float getHeight() const { return height; }
        virtual float  getCurrentSpeed()const{}
        virtual float  getJumpSpeed()const{}

        // makes entity jump (applies only to playerModel)
        virtual void  jump(){}

        // increases HP (applies only to LivingEntityModels)
        virtual void increaseHP(int value){}

        // decreases HP (applies only to LivingEntityModels)
        virtual void decreaseHP(int value){}

        //sets Y speed of PlayerModel to value
        virtual void setCurrentSpeed(float newSpeed) {}

        virtual void increaseScore(int scoreIncrease) {}

        virtual void decreaseScore(int scoreDecrease) {}

        void setPosition(std::pair<float,float> newPosition){ position = newPosition; }

        virtual bool collide(EntityModel& entityModel){}
        virtual void platformCollide(EntityModel& platformModel){}
        virtual int getType() const{}

        //moves an entity that is attached to a platform/player like bonus/enemy
//        virtual void updateAttachedEntity(EntityModel& attachedentity) const{
//
//        }
        //moves an entity that is attached to a platform like bonus/enemy
        virtual void moveEntityOnPlatform(const EntityModel& platform)
        {
                position.first = platform.getPosition().first;
                position.second = platform.getPosition().second + platform.getHeight() + height;
        }
        bool fallingCollide(EntityModel& entityModel) const{

                if (entityModel.getCurrentSpeed() > 0.f) {
                        // if player is jumping we do not need to check platform/spring/spike collision
                        return false;
                }
                std::pair<float, float> collisionEntityPosition = entityModel.getPosition(); // centre of the player
                collisionEntityPosition.second -= entityModel.getHeight();
                float precision = 0.015f;

                //increase precision if collisionEntity is going slow
                if (entityModel.getCurrentSpeed() >= -0.5f) {
                        precision = 0.005f;
                }
                //check if collisionEntity is partially inside/ on top of platform/spring/spike
                if ((collisionEntityPosition.second - (position.second+height)) >= -precision) {
                        //                        entityModel.setCurrentSpeed(entityModel.getJumpSpeed());
                        //                        entityModel.platformCollide(*this);
                        return true;
                }
                return false;
        }
        //updates jetpack location according to playerLocation
        virtual bool updateJetpack(const EntityModel& player){}

protected:
        std::pair<float, float> position{0.f, 0.f};
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
                width = 0.25f;
                height = 0.02f; // 0.02
                //set their speed (default 0) and bounds in which they can move/teleport
                if (randomType == PlatformType::horizontalP) {
                        platformSpeed = 0.5f;
                        lowerBound = -1 + width;
                        upperBound = 1 - width;
                } else if (randomType == PlatformType::verticalP) {
                        platformSpeed = 0.2f;
                        lowerBound = y - 4 * height;
                        upperBound = y + 4 * height;
                }
                else if (randomType == PlatformType::verticalPteleport) {
                        lowerBound = y - 6 * height;
                        upperBound = y + 6 * height;
                }
        }

        float getLowerBound() const { return lowerBound; }
        float getUpperBound() const { return upperBound; }

        bool collide(EntityModel& entityModel) override{
                bool collision = false;
                if(fallingCollide(entityModel)){
                        collision = true;
                        entityModel.jump();
                        if(jumpedOn){
                                entityModel.decreaseScore(type);
                        }
                        jumpedOn = true;
                        if(type == PlatformType::horizontalPteleport){
                                std::pair<float,float> newPos= getPosition();
                                std::shared_ptr<Random> random = Random::getInstance();
                                newPos.first = random->randomPlatformX(getWidth());
                                setPosition(newPos);
                        }
                        else if(getType() == PlatformType::verticalPteleport){
                                std::pair<float,float> newPos= getPosition();
                                std::shared_ptr<Random> random = Random::getInstance();
                                newPos.second = random->randomTeleportPlatformY(getLowerBound(), getUpperBound());
                                setPosition(newPos);
                        }
                }

                return collision;
        }

        ~PlatformModel() override = default;

//        void updateAttachedEntity(EntityModel& attached) const override{
//                std::pair<float,float> newPos(0,0);
//                newPos.first = position.first;
//                newPos.second = position.second + attached.getHeight() + height;
//                attached.setPosition(newPos);
//        }
        // Moves the PlatformModel in X or Y
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
        int getType() const override { return type; }

private:
        PlatformType::Type type;
        bool direction = false;
        float lowerBound = 0.f;
        float upperBound = 0.f;
        float platformSpeed = 0.f;
        bool jumpedOn = false;
};

class BulletModel : public EntityModel{
public:
        BulletModel(float x, float y) : EntityModel(x,y){
                width = 0.04f;
                height = 0.04f;
        }

        ~BulletModel() override = default;

        //check collision between bullet and LivingEntityModel
        bool collide (EntityModel& entityModel) override{

        }

        void move(){
                float timePerFrame = Stopwatch::getInstance()->getTimePerFrame();
                position.second += speedY * timePerFrame;
        }
private:
        float speedY = 1.2f;
        int damage = 1;
};
class LivingEntityModel: public EntityModel{
protected:
        int maxHP = 0;
        int HP = 0;
public:
        LivingEntityModel(float x, float y) : EntityModel(x, y)
        {

        }
        ~LivingEntityModel() override = default;

};
class EnemyModel : public LivingEntityModel{
private:
        EnemyType::Type type;
public:
        EnemyModel(const std::unique_ptr<PlatformModel>& platform, EnemyType::Type type): LivingEntityModel(platform->getPosition().first, platform->getPosition().second){
                width = 0.1f;
                height = 0.1f;
                if(type == EnemyType::weak){
                        maxHP = 3;
                }
                else if (type == EnemyType::strong){
                        maxHP = 5;
                }
                HP = maxHP;
                position.second += platform->getHeight();
        }
        ~EnemyModel() override = default;

        //EnemyModel collides with entityModel
        bool collide(EntityModel& entityModel) override{
                if(type == EnemyType::strong){

                }
        }
};
class PlayerModel : public LivingEntityModel
{
public:
        PlayerModel(float x, float y) : LivingEntityModel(x, y)
        {
                width = 0.1f;
                height = 0.1f;
                maxHP = 3;
                HP = maxHP;
        }
        virtual ~PlayerModel() override = default;

        // Is used when the player reaches the bottom of the screen. Alerts score to save itself to file
        void dead() { notify(Alert::gameOver, {0.f, 0.f}); }
        // sets Y speed to standard
        void jump(){
                currentSpeedY = jumpSpeed;
        }
        bool isDead(){
                if(HP == 0){
                        return true;
                }
                return false;
        }

        // Moves the player in X and Y direction
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
        // Holds ptr of platform with which player collided and bounces player off of platform
        // Also updates score if player collides with same platform twice
//        void platformCollide(EntityModel& platformCollision) override
//        {

//                if (&platformCollision == &*lastJumpedOn) {
//                        notify(Alert::decreaseScore, std::pair<int, int>(lastJumpedOn->getType(), 0));
//                }

//                lastJumpedOn.reset(&platformCollision);
//                currentSpeedY = jumpSpeed;
//        }
        float getCurrentSpeed() const override { return currentSpeedY; }
        void setCurrentSpeed(float newSpeed) override { currentSpeedY = newSpeed; }
        void setPlayerDirection(PlayerMovement::Direction dir) { direction = dir; }
        PlayerMovement::Direction getPlayerDirection() const { return direction; }
        float getJumpSpeed() const override { return jumpSpeed; }
        void increaseScore(int scoreIncrease) override
        { // entities can update score through player
                notify(Alert::increaseScore, std::pair<int, int>(scoreIncrease, 0));
        }
        void decreaseScore(int scoreIncrease) override
        { // entities can update score through player
                notify(Alert::decreaseScore, std::pair<int, int>(scoreIncrease, 0));
        }
        void increaseHP(int value) override
        { // world can updateJetpack HP through player
                if (HP<maxHP && HP >0){
                        notify(Alert::increaseHP, std::pair<int, int>(value, 0));
                        HP += value;
                }

        }
        void decreaseHP(int value) override
        { // world can updateJetpack HP through player
                if (HP>0){
                        notify(Alert::decreaseHP, std::pair<int, int>(value, 0));
                        HP -= value;

                }
        }
        int getMaxHp() const { return maxHP; }


private:
        float jumpSpeed = 1.6f;
        float maxPlayerSpeedX = 1.f;
        float currentSpeedY = 0.f;
        float gravity = 2.f;
//        int immunityFrames = 0;
        PlayerMovement::Direction direction = PlayerMovement::none;
        std::shared_ptr<EntityModel> lastJumpedOn = nullptr;
};

class BonusModel : public EntityModel
{
public:
        BonusModel(const std::unique_ptr<PlatformModel>& platform, BonusType::Type type)
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
        bool collide(EntityModel& entityModel) override{

                float playerSpeed = entityModel.getCurrentSpeed();
                switch (btype) {
                case BonusType::spring:
                        if (fallingCollide(entityModel)){
                                playerSpeed = entityModel.getJumpSpeed();
                                playerSpeed *= sqrt(5); // 5x as high jump
                        }
                        else{
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
                        if(fallingCollide(entityModel)){
                                playerSpeed = entityModel.getJumpSpeed();
                                entityModel.decreaseHP(1);
                        }
                        else{
                                return false;
                        }
                        break;
                }
                entityModel.setCurrentSpeed(playerSpeed);
                entityModel.increaseScore(btype); // increases score based on the bonustype that's picked up
                return true;
        }
        virtual ~BonusModel() override = default;
        // Moves bonus according to the platform it's located on
//        void movePlatformBonus(const std::shared_ptr<PlatformModel>& platform)
//        {
//                position.first = platform->getPosition().first;
//                position.second = platform->getPosition().second + platform->getHeight() + height;
//        }
        int getType() const override { return btype; }
        // applies bonus to player
        void applyToPlayer(std::unique_ptr<PlayerModel>& player)
        {
                float playerSpeed = player->getCurrentSpeed();
                switch (btype) {
                case BonusType::spring:
                        playerSpeed = player->getJumpSpeed();
                        playerSpeed *= sqrt(5); // 5x as high jump
                        break;
                case BonusType::jetpack:
                        playerSpeed = player->getJumpSpeed();
                        playerSpeed *= sqrt(20);
                        break;
                case BonusType::heart:
                        player->increaseHP(1);
                        break;
                case BonusType::spike:
                        player->decreaseHP(1);
                        break;
                }
                player->setCurrentSpeed(playerSpeed);
                player->increaseScore(btype); // increases score based on the bonustype that's picked up
        }
        // updates the bonus position according to the player interaction
        // currently only applies to jetpack
        bool updateJetpack(const EntityModel& player) override
        {
                if (player.getCurrentSpeed() < player.getJumpSpeed()) {
                        return false;
                }
//                PlayerMovement::Direction playerDirection = player.getPlayerDirection();
                std::pair<float, float> playerPosition = player.getPosition();
                float playerWidth = player.getWidth();
                float offset = playerWidth + width;
                // offset will determine how much the jetpack has to be "pushed" from the player centre
                position.first = playerPosition.first + offset;
//                if (playerDirection == PlayerMovement::left) {
//                        position.first = playerPosition.first + offset;
//                } else {
//                        position.first = playerPosition.first - offset;
//                }
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
                width = 0.1f;
                height = 0.1f;
        }
        virtual ~BGTileModel() override = default;
};
#endif // TESTSFML_ENTITYMODEL_H
