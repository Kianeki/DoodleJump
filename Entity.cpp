//
// Created by Kiani on 8/12/2021.
//

#include "Entity.h"

Entity::Entity() {

}

std::pair<float, float> Entity::getPosition() {
    return position;
}

void Entity::setScaledPosition(std::pair<int, int> scaledPos) {
    scaledPosition=scaledPos;
}

Entity::Entity(float x, float y) {
    position.first=x;
    position.second=y;
}

std::pair<float, float> Entity::getScaledPosition() {
    return scaledPosition;
}
