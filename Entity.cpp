//
// Created by Kiani on 8/12/2021.
//

#include "Entity.h"

Entity::Entity() {

}

std::pair<float, float> Entity::getPosition() {
    return position;
}
//void Entity::setPosition(std::pair<float,float> pos){
//    position= pos;
//}
void Entity::setScaledPosition(std::pair<float,float> scaledPos) {
    scaledPosition=scaledPos;
}

Entity::Entity(float x, float y) {
    position.first=x;
    position.second=y;
}

std::pair<float, float> Entity::getScaledPosition() {
    return scaledPosition;
}

float Entity::getWidth() const {
    return 0;
}

float Entity::getHeight() const {
    return 0;
}

platformType::Type Entity::getPlatformType() const {
    return platformType::Type();
}

void Entity::setXPosition(float pos) {
    position.first=pos;
}

void Entity::setYPosition(float pos) {
    position.second=pos;
}

float Platform::getWidth() const {
    return width;
}

float Platform::getHeight() const {
    return height;
}

platformType::Type Platform::getPlatformType() const {
    return type;
}

float Player::getWidth() const {
    return width;
}

float Player::getHeight() const {
    return height;
}

float Player::getJumpHeight() const {
    return jumpHeight;
}
