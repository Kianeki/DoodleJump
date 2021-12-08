//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_CAMERA_H
#define DOODLEJUMP_CAMERA_H

#include "Entity.h"
#include <memory>
#include <list>
//We will be using (0,0) as origin
//(-1,q) left bound, (1,q) right bound (q can take all y values)
//(v,-1) as lower bound (v can go from -1 to 1
// and infinitely extending up

class Camera {
public:
    float currentMaxHeight=0; //start in the centre
    float upperBound=1;
    float lowerBound=-1;
    std::pair<int,int> windowSize;
public:
    void updateMaxHeight(float playerHeight){ //update camera according to player Height
        if(playerHeight>currentMaxHeight){
            currentMaxHeight=playerHeight;
            upperBound=currentMaxHeight+1;
            lowerBound=currentMaxHeight-1;
        }
    }
    void setWindowSize(int windowWidth, int windowheight);

    std::list<std::shared_ptr<Entity>> getEntitiesinView(std::list<std::shared_ptr<Entity>> entities);
private:
    std::list<std::shared_ptr<Entity>> entitiesInView;

private:
    void checkView(std::list<std::shared_ptr<Entity>> entities); //puts all entities in view in "entitiesInView" (except for player)
};


#endif //DOODLEJUMP_CAMERA_H
