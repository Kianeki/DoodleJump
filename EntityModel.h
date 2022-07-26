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

        /**
         * scales the coordinates of the entity via camera and gives them to the attached Observer(entityView)
         * This draws the entity on screen
         */
        void drawEntity(Camera& camera);

        std::pair<float, float> getPosition() const { return position; }

        float getWidth() const;

        float getHeight() const;

        virtual float getCurrentSpeed() const;

        virtual float getJumpSpeed() const;

        // makes entity jump (applies only to playerModel, rest are no-op)
        virtual void jump() {}

        // increases HP (applies only to LivingEntityModels, rest are no-op)
        virtual void increaseHP(int value) {}

        // decreases HP (applies only to LivingEntityModels, rest are no-op)
        virtual void decreaseHP(int value) {}

        /**
         * Sets Y speed of PlayerModel to value (rest are no-op)
         *  This is used by the bonus to update the PlayerModel
         */
        virtual void setCurrentSpeedY(float newSpeed) {}

        // increases score via observer in PlayerModel
        virtual void increaseScore(int scoreIncrease) {}

        // decreases score via observer in PlayerModel
        virtual void decreaseScore(int scoreDecrease) {}

        // Set (x,y) position of entity
        void setPosition(std::pair<float, float> newPosition);

        // returns true if there is a valid collision between the entity and the player
        virtual bool collide(EntityModel& playerModel);

        // Returns the type of bonus/Enemy
        virtual int getType() const;

        // Returns true if entity must be deleted on valid collision
        virtual bool deleteOnCollision();

        // Returns true if entity has 0 HP (only applies to livingEntities)
        virtual bool isDead();

        // moves an entity that is attached to a platform like a bonus/enemy
        virtual void moveEntityOnPlatform(const EntityModel& platform);

        /**
         * returns true if there is a valid collision(only when falling) between the entity and the player
         * The player must be able to pass through platforms or some bonuses when jumping so they only have collision
         * when player is falling
         */
        bool fallingCollide(const EntityModel& entityModel) const;

        // updates jetpack location according to playerLocation
        virtual bool updateJetpack(const EntityModel& player);

        // An entity getsHit if it collides with a bullet, currently only livingEntities can get hit
        // returns true if it is a valid hit ( valid hit =  bullet hits LivingEntity)
        virtual bool getsHit(int damage);

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
        PlatformModel(float x, float y, PlatformType::Type randomType);

        float getLowerBound() const;
        float getUpperBound() const;

        // entityModel(player) collides with platform
        bool collide(EntityModel& entityModel) override;

        bool deleteOnCollision() override;
        ~PlatformModel() override = default;

        // Moves the PlatformModel in X or Y direction
        void movePlatform();
        int getType() const override;

private:
        PlatformType::Type type;
        bool direction = false;
        float lowerBound = 0.f;
        float upperBound = 0.f;
        float platformSpeed = 0.f;
        bool jumpedOn = false;
};

class BulletModel : public EntityModel
{
public:
        BulletModel(float x, float y, BulletType::Type bulletType);

        ~BulletModel() override = default;

        // check collision between bullet and LivingEntityModel
        bool collide(EntityModel& entityModel) override;

        bool deleteOnCollision() override;

        void move();

private:
        float speedY = 0.f;
        int damage = 1;
};
class LivingEntityModel : public EntityModel
{
protected:
        int maxHP = 0;
        int HP = 0;
        int currentImmunityFrames = 0;
        int immunityFrames = 0;

public:
        LivingEntityModel(float x, float y) : EntityModel(x, y) {}

        ~LivingEntityModel() override = default;

        bool isDead() override;

        int getMaxHp() const;

        // if LivingEntity gets hit when not immune, it takes damage, gains immunity( and decreases score in case player
        // got hit)
        bool getsHit(int damage) override;

        // grants immunityFrames to entity
        virtual void grantImmunity();

        // used to tick down immunityFrames
        virtual void updateImmunity();

        virtual bool isImmune();

        // increases HP of livingEntity
        void increaseHP(int value) override;

        // decreases HP of livingEntity
        void decreaseHP(int value) override;

        bool deleteOnCollision() override { return false; }
};
class EnemyModel : public LivingEntityModel
{
private:
        EnemyType::Type type;

public:
        EnemyModel(const std::unique_ptr<PlatformModel>& platform, EnemyType::Type etype);
        ~EnemyModel() override = default;

        // EnemyModel collides with entityModel(currently only useful for player)
        bool collide(EntityModel& entityModel) override;

        int getType() const override;
};
class PlayerModel : public LivingEntityModel
{
public:
        PlayerModel(float x, float y);
        ~PlayerModel() override = default;

        // Is used when the player reaches the bottom of the screen. Alerts score to save itself to file
        void gameOver();
        // sets Y speed to standard
        void jump() override;

        // Moves the player in X and Y direction
        void movePlayer();

        float getCurrentSpeed() const override;

        void setCurrentSpeedY(float newSpeed) override;

        // set the player direction
        void setPlayerDirection(PlayerMovement::Direction dir);

        float getJumpSpeed() const override;

        void increaseScore(int scoreIncrease) override;
        void decreaseScore(int scoreIncrease) override;

private:
        float jumpSpeed = 1.6f;
        float maxPlayerSpeedX = 1.f;
        float currentSpeedY = 0.f;
        float gravity = 2.f;
        PlayerMovement::Direction direction = PlayerMovement::none;
        std::shared_ptr<EntityModel> lastJumpedOn = nullptr;
};

class BonusModel : public EntityModel
{
public:
        BonusModel(const std::unique_ptr<PlatformModel>& platform, BonusType::Type type);

        /**
         * player collides with a bonus
         * bonus will change behaviour of player
         * @return returns true if it is a valid collision
         */
        bool collide(EntityModel& entityModel) override;

        ~BonusModel() override = default;

        bool deleteOnCollision() override;

        int getType() const override;

        /**
         * updates the bonus position according to the player interaction
         * currently only applies to jetpack
         * return false if jetpack duration is over and it must be deleted by world
         */
        bool updateJetpack(const EntityModel& player) override;

private:
        BonusType::Type btype;
};

class BGTileModel : public EntityModel
{
public:
        BGTileModel(float x, float y);
        ~BGTileModel() override = default;
};
#endif // TESTSFML_ENTITYMODEL_H
