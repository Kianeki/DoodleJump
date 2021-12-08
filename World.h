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
    std::list<std::shared_ptr<Entity>> entities;
    std::unique_ptr<Entity> player;
public:
    void generatePlatforms();
};


#endif //DOODLEJUMP_WORLD_H
