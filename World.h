//
// Created by Kiani on 16/12/2021.
//

#ifndef TESTSFML_WORLD_H
#define TESTSFML_WORLD_H

#include <list>

#include "memory"
#include "EntityModel.h"
#include "AbstractFactory.h"
#include "Camera.h"
class World {
public:
    World(){}
    World(std::unique_ptr<AbstractFactory> concreteFactory, float windowWidth, float windowHeight): factory(std::move(concreteFactory)){
        camera = Camera(windowWidth, windowHeight);
        player = std::move(factory->createPlayer(0,-0.5)); //don't spawn too high or stuff gets wonky
//        player->setGravity(gravity);
        platforms.push_back(std::move(factory->createPlatform(0, -0.95, PlatformType::verticalP)));
        platforms.push_back(std::move(factory->createPlatform(0, -0.8, PlatformType::staticP)));
        generateRandomPlatforms();
        drawEntities();
    }
    void generateRandomPlatforms();
    void drawEntities();
    void drawPlayer();
    void movePlayer();
    void setPlayerDirection(PlayerMovement::Direction direction);
    void checkEntitiesInView();
    void movePlatforms();
    void checkCollision();
    bool checkPlayerInView();
private:
    void generateBonuses();
    std::list<std::unique_ptr<PlatformModel>> platforms;
    std::list<std::unique_ptr<PlatformModel>> bonuses;
    std::unique_ptr<PlayerModel> player = nullptr;
    std::unique_ptr<AbstractFactory> factory;
    Camera camera;
    float minPlatformDistance = 0.4f;
//    float gravity = 2.f;
};



#endif //TESTSFML_WORLD_H
