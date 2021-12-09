//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_WORLD_H
#define DOODLEJUMP_WORLD_H

#include <list>
#include "Entity.h"
#include <memory>
class World {
public:
    World();
    std::list<std::unique_ptr<Entity>> entities;
    std::unique_ptr<Player> player;
public:
    void generatePlatforms();
    void movePlayer(std::pair<float,float> pos); //moves x and y coordinate by an amount
    std::list<std::unique_ptr<Entity>>& getEntities();
};


#endif //DOODLEJUMP_WORLD_H
