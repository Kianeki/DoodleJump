//
// Created by Kiani on 8/12/2021.
//

#ifndef DOODLEJUMP_ENTITY_H
#define DOODLEJUMP_ENTITY_H


#include <utility>
namespace platformType {
    enum Type {
        staticP = 'S', temporaryP = 'T', horizontalP = 'H', verticalP = 'V'
    };
}
class Entity {
public:
    Entity();
    Entity(float x, float y);

    std::pair<float,float> getPosition();
    std::pair<float,float> getScaledPosition();
    void setXPosition(float pos);
    void setYPosition(float pos);
    void setScaledPosition(std::pair<float,float> scaledPos);
    virtual float getWidth() const;
    virtual float getHeight() const;
    virtual platformType::Type getPlatformType() const;

protected:
    std::pair<float,float> position{0,0};
    std::pair<float,float> scaledPosition{0,0};
};

class Player : public Entity {
public:
    Player(float x, float y) : Entity(x,y){};
    float getWidth() const override;
    float  getHeight() const override;
    float  getJumpHeight() const;
    void changeLocation(float x, float y){
        position.first=x;
        position.second=y;
    }
private:
    float width = 0.10;
    float height = 0.10;
    float jumpHeight = 0.4;
};

class Platform : public Entity {
public:

    Platform(float x, float y, platformType::Type pType): Entity(x,y){
        type = pType;
    };
    float getWidth() const override;
    float getHeight() const override;
    platformType::Type getPlatformType() const override;
protected:
    float width = 0.25;
    float height = 0.02;
    platformType::Type type;
};


#endif //DOODLEJUMP_ENTITY_H
