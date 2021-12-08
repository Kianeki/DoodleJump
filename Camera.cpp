//
// Created by Kiani on 7/12/2021.
//

#include "Camera.h"
#include "cmath"
void Camera::checkView(std::list<std::shared_ptr<Entity>> entities) {
    entitiesInView.clear();
    float wCentrex = windowSize.first/2.f;
    float wCentrey = windowSize.second/2.f;
    for(auto& entity: entities){
        std:: pair<float,float> position = entity->getPosition();
        if(position.second>lowerBound && position.second<upperBound){ //if it is in view
            position.first= floor(wCentrex+(wCentrex*position.first)); //floor for exact pixel value
            position.second= floor(wCentrey-(wCentrey*position.second));
            entity->setScaledPosition(position);
            entitiesInView.push_back(entity);
        }
    }
}

void Camera::setWindowSize(int windowWidth, int windowheight) {
    windowSize.first=windowWidth;
    windowSize.second=windowheight;
}

std::list<std::shared_ptr<Entity>> Camera::getEntitiesinView(std::list<std::shared_ptr<Entity>> entities) {
    checkView(entities);
    return entitiesInView;
}


