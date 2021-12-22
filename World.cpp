//
// Created by Kiani on 16/12/2021.
//

#include <iostream>
#include "World.h"




void World::generateRandomPlatforms() {
    ////exception if entities is empty
    float platformWidth = platforms.back()->getWidth();
    float platformHeight = platforms.back()->getHeight();
    std::shared_ptr<Random> random = Random::getInstance();
    std::pair<float,float> lastPlatformPosition = platforms.back()->getPosition();
    while(!camera.screenFilled(lastPlatformPosition)){
        lastPlatformPosition = platforms.back()->getPosition();
        float newPlatformY = lastPlatformPosition.second + random->randomPlatformY(platformHeight, minPlatformDistance);
        float newPlatformX = random->randomPlatformX(platformWidth);
        platforms.push_back(factory->createPlatform(newPlatformX, newPlatformY, random->randomPlatformType()));
    }

}

void World::drawEntities() {
    for(auto& entity: platforms){
        entity->drawEntity(camera);
    }
}

void World::drawPlayer() {
    player->drawEntity(camera);
}

void World::movePlayer() {
    player->movePlayer();
    if(!checkPlayerInView()){
//       player.reset();
        // game over
    }
    if(camera.updateMaxHeight(player->getPosition())){ //returns true if the camera has moved (player passed half screen)
        checkEntitiesInView();
    };


}

void World::checkEntitiesInView() {//delete entities if they are out of view
    for(auto it=platforms.begin(); it != platforms.end();){
        if(camera.checkView((*it)->getPosition())){
            it++;
        }
        else{
            it = platforms.erase(it);
        }
    }
//    std::cout<<entities.size()<<std::endl;
}

void World::movePlatforms() {
    for(auto& entity: platforms){
        entity->movePlatform();
    }
}

void World::checkCollision() {
    if(player->getCurrentSpeed()>0) { // if player is jumping we do not need to check collision
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
    for(auto& platform: platforms){
        //this is to calc UPPER corners of rectanglebox of platforms/bonusses
        std::pair<float,float> entityPosition = platform->getPosition();
        float entityWidth = platform->getWidth();
        float entityHeight = platform->getHeight();
        entityUpperLeftCorner.first = entityPosition.first - entityWidth; //entityPosition is  the centre of the entity
        entityUpperLeftCorner.second = entityPosition.second + entityHeight; // need to add/subtract width and height to get rectangle corners
        entityUpperRightCorner.first = entityPosition.first + entityWidth;
        entityUpperRightCorner.second = entityUpperLeftCorner.second;
        //now we check collision between player and this entity
        if(entityPosition.second < playerPosition.second){ //platform is below player
            if((playerLowerLeftCorner.second - entityUpperLeftCorner.second) <= 0 && (playerLowerLeftCorner.second - entityUpperLeftCorner.second)>-0.01){ //player is inside platform on Y-axis ; check if distance is negative (with margin of 0.01)
                if(playerLowerRightCorner.first < entityUpperLeftCorner.first || playerLowerLeftCorner.first > entityUpperRightCorner.first){

                }
                else{
                    // player is not on left or right of the entity
                    //collision
                    player->collide();
                    if(platform->getType() == PlatformType::temporaryP){
                        platforms.remove(platform);
                    }
                    return;
                }
            }
        }

    }
}

bool World::checkPlayerInView() {
    if(camera.checkView(player->getPosition())){ //player is in view
        return true;
    }
    else{ //player has reached the bottom of the screen
        return false;
    }
}

void World::setPlayerDirection(PlayerMovement::Direction direction) {
    player->setPlayerDirection(direction);
}


