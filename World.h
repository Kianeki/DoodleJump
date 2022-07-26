//
// Created by Kiani on 16/12/2021.
//

#ifndef TESTSFML_WORLD_H
#define TESTSFML_WORLD_H

#include <list>

#include "AbstractFactory.h"
#include "Camera.h"
#include "EntityModel.h"
#include "memory"

class World
{
public:
        World(std::unique_ptr<AbstractFactory> concreteFactory, unsigned int windowWidth, unsigned int windowHeight);
        // creates platforms and bonuses to fill screen
        void generateRandomEntities();

        // draws platforms and bonuses on screen
        void drawEntities();

        // draws player on screen
        void drawPlayer();

        // moves player in X and Y direction and updates camera height if necessary
        bool movePlayer();

        // changes player direction according to input
        void setPlayerDirection(PlayerMovement::Direction direction);

        // check if platforms are still in view
        void checkEntitiesInView();

        // move platforms and bonuses
        void moveEntities();

        // check if player is in view (if player hits bottom of screen the game will end)
        bool checkPlayerInView();

        // check collision between player and platforms/bonuses
        void checkCollision();

        // updateJetpack (if necessary) the active bonus that is applied to the player
        void updateActiveBonus();

        // creates the backgrounds to fill the screen
        void generateBackground();

        // draws background onto screen
        void drawBackground();

        // shoots a bullet from player that can hit enemies
        void shootFriendlyBullet();

        // allows player to shoot again
        void reload();

        // shoots a bullet from an enemy at the player
        void shootEnemyBullet(const EntityModel& enemy);

private:

        std::list<std::unique_ptr<BGTileModel>> makeBackgroundRow(float currentRowY);

        std::list<std::pair<std::unique_ptr<PlatformModel>, std::unique_ptr<EntityModel>>> entities;

        bool globalCollision(EntityModel& livingEntity, EntityModel& staticEntity);
private:
        std::unique_ptr<PlayerModel> player = nullptr;
        std::unique_ptr<AbstractFactory> factory;
        Camera camera;
        float minPlatformDistance = 0.4f;
        std::unique_ptr<EntityModel> activeBonus = nullptr;
        std::list<std::list<std::unique_ptr<BGTileModel>>> backgroundTiles; // list[row][column]
        std::list<std::unique_ptr<BulletModel>> friendlyBullets;
        std::list<std::unique_ptr<BulletModel>> enemyBullets;
        bool reloaded = true;
};

#endif // TESTSFML_WORLD_H
