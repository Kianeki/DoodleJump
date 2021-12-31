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
//    World(){}
    World(std::unique_ptr<AbstractFactory> concreteFactory, unsigned int windowWidth, unsigned int windowHeight): factory(std::move(concreteFactory)){
        camera = Camera(windowWidth, windowHeight);
        player = std::move(factory->createPlayer(0,-0.5)); //don't spawn too high or stuff gets wonky
//        player->setGravity(gravity);
//        std::unique_ptr<PlatformModel> platform = std::move(factory->createPlatform(0, -0.6, PlatformType::staticP));
//        entities.emplace_back(std::move(platform),std::move(factory->createBonus(platform,BonusType::jetpack)));
        entities.emplace_back(std::move(factory->createPlatform(0,-0.8,PlatformType::staticP)),nullptr);
        backgroundTiles.push_back(makeBackgroundRow(-1));
        generateBackground();
        generateRandomEntities();
        drawEntities();
    }
    void generateRandomEntities();
    void drawEntities();
    void drawPlayer();
    void movePlayer();
    void setPlayerDirection(PlayerMovement::Direction direction);
    void checkEntitiesInView();
    void moveEntities();

    bool checkPlayerInView();

    void checkCollision();
    void updateActiveBonus();
    void generateBackground();
    void drawBackground();
private:
    bool checkPlatformCollision(std::pair<float,float> entityPos, float width, float height);
    bool checkBonusCollision(const std::unique_ptr<BonusModel>& bonus);
    std::list<std::unique_ptr<BGTileModel>> makeBackgroundRow(float currentRowY);
//    std::list<std::pair<std::unique_ptr<PlatformModel>,std::unique_ptr<BonusModel>>> entities;
    std::list<std::pair<std::shared_ptr<PlatformModel>,std::unique_ptr<BonusModel>>> entities;
//    std::list<std::unique_ptr<PlatformModel>> platforms;
//    std::list<std::unique_ptr<BonusModel>> bonuses;
    std::unique_ptr<PlayerModel> player = nullptr;
    std::unique_ptr<AbstractFactory> factory;
    Camera camera;
    float minPlatformDistance = 0.4f;
    std::unique_ptr<BonusModel> activeBonus = nullptr;
//    float gravity = 2.f;
    std::list<std::list<std::unique_ptr<BGTileModel>>> backgroundTiles; //list[row][column]
};



#endif //TESTSFML_WORLD_H
