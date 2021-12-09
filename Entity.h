//
// Created by Kiani on 8/12/2021.
//

#ifndef DOODLEJUMP_ENTITY_H
#define DOODLEJUMP_ENTITY_H


#include <utility>

class Entity {
public:
    Entity();
    Entity(float x, float y);
    std::pair<float,float> getPosition();

    std::pair<float,float> getScaledPosition();
    void setPosition(std::pair<float,float> Pos);
    void setScaledPosition(std::pair<float,float> scaledPos);
protected:
    std::pair<float,float> position{0,0};
    std::pair<float,float> scaledPosition{0,0};
};

class Player : public Entity {
public:
    void changeLocation(float x, float y){
        position.first=x;
        position.second=y;
    }
};

#endif //DOODLEJUMP_ENTITY_H
