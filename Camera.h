//
// Created by Kiani on 7/12/2021.
//

#ifndef TESTSFML_CAMERA_H
#define TESTSFML_CAMERA_H


#include <utility>
#include <math.h>
//We will be using a coordinate system with (0,0) as origin,
//(-1,0) as left bound
//(1,0) as right bound
//(0,-1) lowewst bound
//and (0,x) as upper bound where x can go to infty
// (0,x-1) as lower bound
class Camera {

public:
    Camera(float windowWidth, float windowHeight) : windowWidth(windowWidth), windowHeight(windowHeight){

    }
    Camera();
    std::pair<float,float> scaledPosition(float positionX, float positionY) const{
        float wCentrex = windowWidth/2.f;
        float wCentrey = windowHeight/2.f;
//        std::pair<float,float> scaledPos;
        if(positionY>lowerBound /*&& position.second<upperBound*/){ //if it is in view
            positionX = wCentrex+(wCentrex*positionX); //scaled pixelwaarde X
            positionY = wCentrey-((positionY-currentMaxHeight)*wCentrey); //scaled pixelwaarde Y
        }
        return {positionX,positionY};
    }
    bool updateMaxHeight(std::pair<float,float> playerPos){
        if(playerPos.second>currentMaxHeight){
            currentMaxHeight=playerPos.second;
            upperBound=currentMaxHeight+1;
            lowerBound=currentMaxHeight-1;
            return true;
        }
        return false;
    }
    bool checkView(std::pair<float,float> position) const{
        if(position.second >lowerBound) {//if it is in cameraview
            return true;
        }
        else{
            return false;
        }
    }
    bool screenFilled(std::pair<float,float> entityPosition) const{
        if(entityPosition.second>upperBound){
            return true;
        }
        else{
            return false;
        }
    }
//    void updatePlayerLocation(float x, float y){
//        playerLocation.first+=x;
//        playerLocation.second+=y;
//        if(playerLocation.second>currentMaxHeight){
//            currentMaxHeight=playerLocation.second;
//            cameraBounds.first=currentMaxHeight+1;
//            cameraBounds.second=currentMaxHeight-1;
//        }
//    }
//    std::pair<float,float> getPlayerLocation(){
//        std::pair<float,float> relativeLocation{playerLocation.first,playerLocation.second-currentMaxHeight};
//        return relativeLocation;
//    }
private:
//    std::pair<float,float> playerLocation{0,0};
    float currentMaxHeight{0}; //Y-value
    float upperBound{1};//upper and lower bounds, left and right bounds are always the same
    float lowerBound{-1};
    float windowWidth;
    float windowHeight;

};


#endif //TESTSFML_CAMERA_H
