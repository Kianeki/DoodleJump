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
    float platformWidth = entities.back()->getWidth();
    float platformHeight = entities.back()->getHeight();
    std::shared_ptr<Random> random = Random::getInstance();
    while(lastPlatformY< camera.upperBound){ //while the last platform generated is lower than the top of the camera
        platformType::Type platformType = random->randomPlatformType(difficulty);
        lastPlatformY+= random->randomPlatformY(platformHeight, player->getJumpHeight(), difficulty); //lastPlatformY will represent Y value of new platform
        platformX = random->randomPlatformX(platformWidth); //platformX represents X value of new platform
        entities.push_back(std::unique_ptr<Platform>(new Platform(platformX,lastPlatformY, platformType)));
    }
}

void World::movePlayer(std::pair<float, float> pos) {
    std::pair<float,float> currentPos= player->getPosition();
    currentPos.first+=pos.first;
    currentPos.second+=pos.second;
    player->changeLocation(currentPos.first,currentPos.second);
//    visualPlayer->setPosition(currentPos);
}

const std::list<std::unique_ptr<Entity>>& World::getEntities() const {
    return entities;
}

void World::updateMaxHeight() {
    camera.updateMaxHeight(player); // update the max height according to the Player, should always be centered

}

void World::updateCameraView() {
    camera.updateView(entities); // check if platforms are in view, delete if out of view
}

void World::setWindowSize(float width, float height) {
    camera.setWindowSize(width, height);
}


