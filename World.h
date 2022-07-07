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
        World(std::unique_ptr<AbstractFactory> concreteFactory, unsigned int windowWidth, unsigned int windowHeight)
            : factory(std::move(concreteFactory)), camera(Camera(windowWidth, windowHeight))
        {
                player = std::move(factory->createPlayer(0, -0.5)); // don't spawn too high or stuff gets wonky
                // place starter platform
                entities.emplace_back(std::move(factory->createPlatform(0, -0.7, PlatformType::staticP)), nullptr);
                // make bottom row of background
                backgroundTiles.push_back(makeBackgroundRow(-1));
                generateBackground();
                generateRandomEntities();
                drawEntities();
        }
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

        // update (if necessary) the active bonus that is applied to the player
        void updateActiveBonus();

        // creates the backgrounds to fill the screen
        void generateBackground();

        // draws background onto screen
        void drawBackground();

private:
        bool checkPlatformCollision(std::pair<float, float> entityPos, float width, float height);

        bool checkBonusCollision(const std::unique_ptr<BonusModel>& bonus);

        std::list<std::unique_ptr<BGTileModel>> makeBackgroundRow(float currentRowY);

        std::list<std::pair<std::shared_ptr<PlatformModel>, std::unique_ptr<BonusModel>>> entities;

private:
        std::unique_ptr<PlayerModel> player = nullptr;
        std::unique_ptr<AbstractFactory> factory;
        Camera camera;
        float minPlatformDistance = 0.4f;
        std::unique_ptr<BonusModel> activeBonus = nullptr;
        std::list<std::list<std::unique_ptr<BGTileModel>>> backgroundTiles; // list[row][column]
};

#endif // TESTSFML_WORLD_H
