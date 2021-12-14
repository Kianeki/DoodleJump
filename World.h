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
    void movePlayer(float timePerFrame); //moves x and y coordinate by an amount
    const std::list<std::unique_ptr<Entity>>& getEntities() const;
    void updateMaxHeight(); // update the max height according to the Player, should always be centered
    void updateCameraView(); // check if platforms are in view, delete if out of view
    void setWindowSize(float x, float y);
    void movePlayerLeft(bool keyPressed);
    void movePlayerRight(bool keyPressed);
    void movePlatforms(float timePerFrame);
    void checkCollision();

    float getGravity() const;
//    void handlePlayerInput()
private:
    Camera camera;
    std::list<std::unique_ptr<Entity>> entities;
    int score=0;
    int difficulty=3; //max 3
    bool playerMovingLeft=false;
    bool playerMovingRight=false;
    float gravity = 2.f;
    float minPlatformDistance = 0.4f;
};


#endif //DOODLEJUMP_WORLD_H
