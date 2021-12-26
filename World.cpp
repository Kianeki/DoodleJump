//
// Created by Kiani on 16/12/2021.
//

#include <iostream>
#include "World.h"




void World::generateRandomEntities() {
    ////exception if entities is empty
    float platformWidth = entities.back().first->getWidth();
    float platformHeight = entities.back().first->getHeight();
    std::shared_ptr<Random> random = Random::getInstance();
    std::pair<float,float> lastPlatformPosition = entities.back().first->getPosition();
    while(!camera.screenFilled(lastPlatformPosition)){
        lastPlatformPosition = entities.back().first->getPosition();
        float newPlatformY = lastPlatformPosition.second + random->randomPlatformY(platformHeight, minPlatformDistance);
        float newPlatformX = random->randomPlatformX(platformWidth);
        std::unique_ptr<PlatformModel> newPlatform = factory->createPlatform(newPlatformX, newPlatformY, random->randomPlatformType());
//        platforms.push_back(factory->createPlatform(newPlatformX, newPlatformY, random->randomPlatformType()));
        BonusType::Type bonustype = random->randomBonusType(); //we will also have a chance of generating a bonus after the platform has been created
        if((bonustype!=BonusType::none) && (newPlatform->getType()!=PlatformType::temporaryP)){
            std::unique_ptr<BonusModel> newBonus = factory->createBonus(newPlatform, bonustype);
            entities.emplace_back(std::move(newPlatform),std::move(newBonus));
        }
        else{
            entities.emplace_back(std::move(newPlatform),nullptr);
        }
    }

}

void World::drawEntities() {
    for(auto& entity: entities){
        entity.first->drawEntity(camera); //draw platforms
        if(entity.second!= nullptr){ //draw bonuses
            entity.second->drawEntity(camera);
        }
    }
    if(activeBonus!=nullptr){ //needs to be drawn after platforms/bonuses
        activeBonus->drawEntity(camera);
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
    for(auto it=entities.begin(); it != entities.end();){
        if(camera.checkView((*it).first->getPosition())){ //we check if the platform is still in view
            it++;
        }
        else{
            it = entities.erase(it); //this will delete both the platform and the attached bonus
        }
    }
//    std::cout<<entities.size()<<std::endl;
}

void World::moveEntities() {
    updateActiveBonus(); // this will update the jetpack position in our case
    for(auto& entity: entities){
        entity.first->movePlatform(); //first we move the platform
        if(entity.second !=nullptr){
            entity.second->movePlatformBonus(entity.first); //will update bonus position according to platformpos
        }
    }
}

bool World::checkPlatformCollision(std::pair<float,float> entityPosition, float entityWidth, float entityHeight) {
    if(player->getCurrentSpeed()>0) { // if player is jumping we do not need to check platform collision
        return false;
    }
    std::pair<float,float> platformUpperLeftCorner{0,0};
    std::pair<float,float> platformUpperRightCorner{0,0};
    std::pair<float,float> playerLowerLeftCorner{0,0};
    std::pair<float,float> playerLowerRightCorner{0,0};
    //this is to calc LOWER corners of rectanglebox of player
    std::pair<float,float> playerPosition = player->getPosition(); //centre of the player
    float playerWidth = player->getWidth();
    float playerHeight = player->getHeight();
    playerLowerLeftCorner.first = playerPosition.first - playerWidth;
    playerLowerLeftCorner.second = playerPosition.second - playerHeight;
    playerLowerRightCorner.first = playerPosition.first + playerWidth;
    playerLowerRightCorner.second = playerLowerLeftCorner.second;
        //this is to calc UPPER corners of rectanglebox of platforms/bonusses
//        std::pair<float,float> entityPosition = entity->getPosition();
//        float entityWidth = entity->getWidth();
//        float entityHeight = entity->getHeight();
        platformUpperLeftCorner.first = entityPosition.first - entityWidth; //entityPosition is  the centre of the entity
        platformUpperLeftCorner.second = entityPosition.second + entityHeight; // need to add/subtract width and height to get rectangle corners
        platformUpperRightCorner.first = entityPosition.first + entityWidth;
        platformUpperRightCorner.second = platformUpperLeftCorner.second;
        //now we check collision between player and this entity
        if(entityPosition.second < playerPosition.second){ //platform is below player
            if((playerLowerLeftCorner.second - platformUpperLeftCorner.second) <= 0 && (playerLowerLeftCorner.second - platformUpperLeftCorner.second) > -0.01){ //player is inside platform on Y-axis ; check if distance is negative (with margin of 0.01)
                if(playerLowerRightCorner.first < platformUpperLeftCorner.first || playerLowerLeftCorner.first > platformUpperRightCorner.first){

                }
                else{
                    // player is not on left or right of the entity
                    //collision
//                    player->collide();
//                    if(entityPair.first->getType() == PlatformType::temporaryP){ //we delete temporary platforms on collision
//                        entities.remove(entityPair);
//                    }
                    return true;
                }
            }
        }
    return false;
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



bool World::checkBonusCollision(const std::unique_ptr<BonusModel>& bonus) {
    std::pair<float,float> bonusUpperLeftCorner{0,0};
    std::pair<float,float> bonusUpperRightCorner{0,0};
    std::pair<float,float> bonusLowerLeftCorner{0,0};
    std::pair<float,float> bonusLowerRightCorner{0,0};
    std::pair<float,float> playerLowerLeftCorner{0,0};
    std::pair<float,float> playerLowerRightCorner{0,0};
    std::pair<float,float> playerUpperLeftCorner{0,0};
    std::pair<float,float> playerUpperRightCorner{0,0};
    //this is to calc LOWER corners of rectanglebox of player
    std::pair<float,float> playerPosition = player->getPosition();
    float playerWidth = player->getWidth();
    float playerHeight = player->getHeight();
    playerLowerLeftCorner.first = playerPosition.first - playerWidth;
    playerLowerLeftCorner.second = playerPosition.second - playerHeight;
    playerLowerRightCorner.first = playerPosition.first + playerWidth;
    playerLowerRightCorner.second = playerLowerLeftCorner.second;
    playerUpperLeftCorner.first = playerLowerLeftCorner.first;
    playerUpperRightCorner.first = playerLowerRightCorner.first;
    playerUpperLeftCorner.second = playerPosition.second + playerHeight;
    playerUpperRightCorner.second = playerUpperLeftCorner.second;
    //we now have the full rectangleCollision box of the player
    //now we calc the rectanglebox of bonusses
        std::pair<float,float> bonusPosition = bonus->getPosition();
        float bonusWidth = bonus->getWidth();
        float bonusHeight = bonus->getHeight();
    bonusUpperLeftCorner.first = bonusPosition.first - bonusWidth; //entityPosition is  the centre of the entity
    bonusUpperLeftCorner.second = bonusPosition.second + bonusHeight; // need to add/subtract width and height to get rectangle corners
    bonusUpperRightCorner.first = bonusPosition.first + bonusWidth;
    bonusUpperRightCorner.second = bonusUpperLeftCorner.second;
    bonusLowerLeftCorner.first = bonusUpperLeftCorner.first;
    bonusLowerRightCorner.first = bonusUpperRightCorner.first;
    bonusLowerRightCorner.second = bonusPosition.second - bonusHeight;
    bonusLowerLeftCorner.second = bonusLowerRightCorner.second;
    if(!(playerLowerLeftCorner.second > bonusUpperRightCorner.second) && !(playerUpperLeftCorner.second < bonusLowerRightCorner.second)){// if this is true then the player is NOT above/below the bonus entirely
        if(!(playerLowerLeftCorner.first> bonusUpperRightCorner.first) && !(playerLowerRightCorner.first< bonusUpperLeftCorner.first)){ // if this is true then the player is inside of the bonus
            return true;
        }
    }
    else{
        return false;
    }
}

void World::checkCollision() {

    for(auto it= entities.begin(); it!= entities.end();){
        std::pair<float,float> platformPos = (*it).first->getPosition();
        float platformHeight = (*it).first->getHeight();
        float platformWidth = (*it).first->getWidth();
        if((*it).second!= nullptr){ //if there is a bonus, check bonuscollision first
            if((*it).second->getType() == BonusType::spring){
                std::pair<float,float> bonusPos = (*it).second->getPosition();
                float bonusHeight = (*it).second->getHeight();
                float bonusWidth = (*it).second->getWidth();
                if(checkPlatformCollision(bonusPos, bonusWidth, bonusHeight)){ //spring bonus collision is the same as platform collision
                    player->platformCollide();//this will give player the normal jump speed
                    (*it).second->applyToPlayer(player); //this will apply the (spring)bonus
                }
            }
            else{ //its not a spring bonus
                if(checkBonusCollision((*it).second) && activeBonus==nullptr){ //player picks up bonus and there is no bonus active
                    player->platformCollide();//this will give player the normal jump speed
                    (*it).second->applyToPlayer(player); //this will apply the bonus
                    activeBonus = std::move((*it).second); //the world will keep track of the activeBonus
                    (*it).second = nullptr; //safety
                }
            }
        }
        if(checkPlatformCollision(platformPos, platformWidth, platformHeight)){ // if there is collision between platform and player
            player->platformCollide();
            if((*it).first->getType() == PlatformType::temporaryP){
                it = entities.erase(it);
            }
            else{

            }
        }
        it++;
    }
}

void World::updateActiveBonus() { // is used to update the position of the active bonus and destruct it once it is expired
    if(activeBonus == nullptr){
        return;
    }
    else{
        if(activeBonus->update(player)){
            //bonus has not yet expired
        }
        else{
            activeBonus.reset();
            activeBonus = nullptr;
        }
    }
}




