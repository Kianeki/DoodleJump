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
//    void setPosition(std::pair<float,float> Pos);
    void setScaledPosition(std::pair<float,float> scaledPos);
    virtual float getWidth();
    virtual float getHeight();
protected:
    std::pair<float,float> position{0,0};
    std::pair<float,float> scaledPosition{0,0};
};

class Player : public Entity {
public:
    Player(float x, float y) : Entity(x,y){};
    float getWidth() override;
    float getHeight() override;
    void changeLocation(float x, float y){
        position.first=x;
        position.second=y;
    }
private:
    float width = 0.10;
    float height = 0.15;

};

class Platform : public Entity {
public:
    Platform(float x, float y): Entity(x,y){};
    float getWidth() override;
    float getHeight() override;
protected:
    float width = 0.25;
    float height = 0.02;
};
#endif //DOODLEJUMP_ENTITY_H
