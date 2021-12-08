//
// Created by Kiani on 7/12/2021.
//

#include "World.h"

World::World() {
    entities.push_back(std::shared_ptr<Entity>(new Entity(0.5,0.5)));
    entities.push_back(std::shared_ptr<Entity>(new Entity(0,-0.99)));
}
void World::generatePlatforms() {

}


