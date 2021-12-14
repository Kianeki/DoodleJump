//
// Created by Kiani on 7/12/2021.
//

#include "Camera.h"
#include "cmath"

void Camera::updateView(std::list<std::unique_ptr<Entity>>& entities) {
    float wCentrex = windowSize.first/2.f;
    float wCentrey = windowSize.second/2.f;
    for(auto it=entities.begin(); it!=entities.end();){
        //************************************************************************
//        if((*it)->getPlatformType()== platformType::horizontalP){ //this section controls horizon/vertical platform movement
//            if((*it)->checkDirection()){
//                (*it)->setXPosition((*it)->getPosition().first+0.01);
//            }
//            else{
//                (*it)->setXPosition((*it)->getPosition().first-0.01);
//            }
//        }
//        else if((*it)->getPlatformType()== platformType::verticalP){
//            if((*it)->checkDirection()){
//                (*it)->setYPosition((*it)->getPosition().second+0.01);
//            }
//            else{
//                (*it)->setYPosition((*it)->getPosition().second-0.01);
//            }
//        }
        //********************************************************
        std:: pair<float,float> position = (*it)->getPosition();
        if(position.second>lowerBound /*&& position.second<upperBound*/){ //if it is in view
            position.first = wCentrex+(wCentrex*position.first); //scaled pixelwaarde X
            position.second = wCentrey-((position.second-currentMaxHeight)*wCentrey); //scaled pixelwaarde Y
            (*it)->setScaledPosition(position);
            ++it;
        }
        else{
            it = entities.erase(it);
        }
    }

}

void Camera::setWindowSize(int windowWidth, int windowheight) {
    windowSize.first=windowWidth;
    windowSize.second=windowheight;
}


void Camera::updateMaxHeight(std::unique_ptr<Player>& player) { //update camera according to visualPlayer Height
    float wCentrex = windowSize.first/2.f;
    float wCentrey = windowSize.second/2.f;
    std::pair<float,float> playerPos = player->getPosition();
    float playerHeight = playerPos.second;
    if(playerHeight>currentMaxHeight){
        currentMaxHeight=playerHeight;
        upperBound=currentMaxHeight+1;
        lowerBound=currentMaxHeight-1;
    }
    playerPos.first = floor(wCentrex+(wCentrex*playerPos.first)); //floor for exact pixel value
    playerPos.second = floor(wCentrey-((playerPos.second-currentMaxHeight)*wCentrey));
    player->setScaledPosition(playerPos);

}



