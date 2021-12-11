//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_WORLD_H
#define DOODLEJUMP_WORLD_H

#include <list>
#include "Entity.h"
#include <memory>
#include "Random.h"
#include "Camera.h"
class World {
public:

    std::unique_ptr<Player> player;
public:
    World();
    void generatePlatforms();
    void movePlayer(std::pair<float,float> pos); //moves x and y coordinate by an amount
    const std::list<std::unique_ptr<Entity>>& getEntities() const;
    void updateMaxHeight(); // update the max height according to the Player, should always be centered
    void updateCameraView(); // check if platforms are in view, delete if out of view
    void setWindowSize(float x, float y);

private:
    Camera camera;
    std::list<std::unique_ptr<Entity>> entities;
    int score=0;
    int difficulty=3; //max 3
};


#endif //DOODLEJUMP_WORLD_H
