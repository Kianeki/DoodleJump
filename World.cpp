//
// Created by Kiani on 7/12/2021.
//

#include "World.h"

World::World() {

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
        lastPlatformY+= random->randomPlatformY(platformHeight, minPlatformDistance, difficulty); //lastPlatformY will represent Y value of new platform
        platformX = random->randomPlatformX(platformWidth); //platformX represents X value of new platform
        if(platformType==platformType::horizontalP){
            entities.push_back(std::unique_ptr<Platform>(new HorizontalPlatform(platformX,lastPlatformY)));
        }
        else if(platformType==platformType::verticalP){
            entities.push_back(std::unique_ptr<Platform>(new VerticalPlatform(platformX,lastPlatformY)));
        }
        else{
            entities.push_back(std::unique_ptr<Platform>(new Platform(platformX,lastPlatformY, platformType)));
        }
    }
}

void World::movePlayer(float timePerFrame) {
    std::pair<float,float> currentPos= player->getPosition();
    currentPos.first-=playerMovingLeft*1*timePerFrame;
    currentPos.first+=playerMovingRight*1*timePerFrame;
    player->currentSpeed -= gravity*timePerFrame;
    currentPos.second += player->currentSpeed*timePerFrame;
//    if(player->checkDirection()){ //false = jumping / true = falling
//        if(player->currentSpeed == 0){
//
//        }
//        else{
//            if(player->currentSpeed > player->maxFallSpeed){ //maximum falling speed hasnt been reached yet
//                player->currentSpeed = player->currentSpeed*2;
//            }
//        }
//        currentPos.second+= player->currentSpeed*timePerFrame; //falling
//    }
//    else{ // jumping
//        if(player->currentSpeed==0){
//            player->currentSpeed= player->getJumpHeight()/2;
//        }
//        else{
//            player->currentSpeed = player->currentSpeed/2;
//        }
//        if(player->currentSpeed < player->getJumpHeight()/10){
//            player->changeDirection();
//
//        }
//        currentPos.second += player->currentSpeed*timePerFrame;
//    }
//    currentPos.second+=pos.second;
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

void World::movePlayerLeft(bool keyPressed) {
    playerMovingLeft=keyPressed;
//    player->setXPosition(player->getPosition().first-0.01);
}

void World::movePlayerRight(bool keyPressed) {
    playerMovingRight=keyPressed;
//    player->setXPosition(player->getPosition().first+0.01);
}

void World::movePlatforms(float timePerFrame) {
    for(auto it=entities.begin(); it!=entities.end();++it) {
        //************************************************************************
        if ((*it)->getPlatformType() ==
            platformType::horizontalP) { //this section controls horizon/vertical platform movement
            if ((*it)->checkDirection()) {
                (*it)->setXPosition((*it)->getPosition().first + 0.5*timePerFrame);
            } else {
                (*it)->setXPosition((*it)->getPosition().first - 0.5*timePerFrame);
            }
        } else if ((*it)->getPlatformType() == platformType::verticalP) {
            if ((*it)->checkDirection()) {
                (*it)->setYPosition((*it)->getPosition().second + 0.1*timePerFrame);
            } else {
                (*it)->setYPosition((*it)->getPosition().second - 0.1*timePerFrame);
            }
        }
    }
}

void World::checkCollision() {
    if(player->checkDirection()) { // if player is jumping we do not need to check collision
        return;
    }
    std::pair<float,float> entityUpperLeftCorner;
    std::pair<float,float> entityUpperRightCorner;
    std::pair<float,float> playerLowerLeftCorner;
    std::pair<float,float> playerLowerRightCorner;
    //this is to calc LOWER corners of rectanglebox of player
    std::pair<float,float> playerPosition = player->getPosition();
    float playerWidth = player->getWidth();
    float playerHeight = player->getHeight();
    playerLowerLeftCorner.first = playerPosition.first - playerWidth;
    playerLowerLeftCorner.second = playerPosition.second - playerHeight;
    playerLowerRightCorner.first = playerPosition.first + playerWidth;
    playerLowerRightCorner.second = playerLowerLeftCorner.second;
    for(auto& entity: entities){
        //this is to calc UPPER corners of rectanglebox of platforms/bonusses
        std::pair<float,float> entityPosition = entity->getPosition();
        float entityWidth = entity->getWidth();
        float entityHeight = entity->getHeight();
        entityUpperLeftCorner.first = entityPosition.first - entityWidth; //entityPosition is  the centre of the entity
        entityUpperLeftCorner.second = entityPosition.second + entityHeight; // need to add/subtract width and height to get rectangle corners
        entityUpperRightCorner.first = entityPosition.first + entityWidth;
        entityUpperRightCorner.second = entityUpperLeftCorner.second;
        //now we check collision between player and this entity
        if(entityPosition.second < playerPosition.second){ //platform is below player
            if((playerLowerLeftCorner.second - entityUpperLeftCorner.second) <= 0 && (playerLowerLeftCorner.second - entityUpperLeftCorner.second)>-0.04){ //player is inside platform on Y-axis
                //player is inside platform on Y-axis ; check if distance is negative (with margin of 0.02)
                if(playerLowerRightCorner.first < entityUpperLeftCorner.first || playerLowerLeftCorner.first > entityUpperRightCorner.first){

                }
                else{
                    // player is not on left or right of the entity
                    //collision
                    player->changeDirection();
                    if(entity->getPlatformType() == platformType::temporaryP){
                        entities.remove(entity);
                    }
                    return;
                }
            }
        }

    }
}

float World::getGravity() const {
    return gravity;
}


