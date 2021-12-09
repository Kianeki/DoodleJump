//
// Created by Kiani on 7/12/2021.
//

#include "Camera.h"
#include "cmath"
void Camera::checkView(std::list<std::shared_ptr<Entity>>& entities) {
    entitiesInView.clear();
    float wCentrex = windowSize.first/2.f;
    float wCentrey = windowSize.second/2.f;
    for(auto& entity: entities){
        std:: pair<float,float> position = entity->getPosition();
        if(position.second>lowerBound && position.second<upperBound){ //if it is in view
            position.first= floor(wCentrex+(wCentrex*position.first)); //floor for exact pixel value
            position.second= floor(wCentrey-((position.second-currentMaxHeight)*wCentrey));
            entity->setScaledPosition(position);
            entitiesInView.push_back(entity);
        }
//        else{
//            entities.pop_front(); //we arrange the platforms from bottom to top, so we can alway pop front if platform is out of view
//        }
    }
}

void Camera::setWindowSize(int windowWidth, int windowheight) {
    windowSize.first=windowWidth;
    windowSize.second=windowheight;
}

std::list<std::shared_ptr<Entity>> Camera::getEntitiesinView(std::list<std::shared_ptr<Entity>>& entities) {
    checkView(entities);
    return entitiesInView;
}

void Camera::updateMaxHeight(std::unique_ptr<Entity>& player) { //update camera according to player Height
    float wCentrex = windowSize.first/2.f;
    float wCentrey = windowSize.second/2.f;
    std::pair<float,float> playerPos = player->getPosition();
    float playerHeight = playerPos.second;
    if(playerHeight>currentMaxHeight){
        currentMaxHeight=playerHeight;
        upperBound=currentMaxHeight+1;
        lowerBound=currentMaxHeight-1;
    }
    playerPos.first= floor(wCentrex+(wCentrex*playerPos.first)); //floor for exact pixel value
    playerPos.second= floor(wCentrey-((playerPos.second-currentMaxHeight)*wCentrey));
    player->setScaledPosition(playerPos);

}



