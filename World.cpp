//
// Created by Kiani on 7/12/2021.
//

#include "World.h"

World::World() {
//    entities.push_back(std::unique_ptr<Platform>(new Platform(0,-0.98, platformType::staticP)));
//    entities.push_back(std::unique_ptr<Platform>(new Platform(0.5,0.5,platformType::staticP)));
    entities.push_back(std::unique_ptr<Platform>(new Platform(0,-0.8,platformType::staticP)));
    player = std::unique_ptr<Player>(new Player(0,-0.5));
}
void World::generatePlatforms() {
    float lastPlatformY = entities.back()->getPosition().second;
    float platformX = 0.0;
    std::shared_ptr<Random> random = Random::getInstance();
    while(lastPlatformY< 1.0){ //while the last platform generated is lower than the top of the camera
        lastPlatformY+= random->randomPlatformY(); //lastPlatformY will represent Y value of new platform
        platformX = random->randomPlatformX(); //platformX represents X value of new platform
        entities.push_back(std::unique_ptr<Platform>(new Platform(platformX,lastPlatformY,platformType::staticP)));
    }
}

void World::movePlayer(std::pair<float, float> pos) {
    std::pair<float,float> currentPos= player->getPosition();
    currentPos.first+=pos.first;
    currentPos.second+=pos.second;
    player->changeLocation(currentPos.first,currentPos.second);
//    visualPlayer->setPosition(currentPos);
}

std::list<std::unique_ptr<Entity>>& World::getEntities() {
    return entities;
}


