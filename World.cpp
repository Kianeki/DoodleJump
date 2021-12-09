//
// Created by Kiani on 7/12/2021.
//

#include "World.h"

World::World() {
    entities.push_back(std::unique_ptr<Entity>(new Entity(0,-0.98)));
    entities.push_back(std::unique_ptr<Entity>(new Entity(0.5,0.5)));
    entities.push_back(std::unique_ptr<Entity>(new Entity(0,1.2)));
    player = std::unique_ptr<Entity>(new Entity(0,0));
}
void World::generatePlatforms() {

}

void World::movePlayer(std::pair<float, float> pos) {
    std::pair<float,float> currentPos= player->getPosition();
    currentPos.first+=pos.first;
    currentPos.second+=pos.second;
    player->setPosition(currentPos);
}

std::list<std::unique_ptr<Entity>>& World::getEntities() {
    return entities;
}

