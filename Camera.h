//
// Created by Kiani on 7/12/2021.
//

#ifndef TESTSFML_CAMERA_H
#define TESTSFML_CAMERA_H

#include "Random.h"
#include <math.h>
#include <utility>

// We will be using a coordinate system with (0,0) as origin,
//(-1,0) as left bound
//(1,0) as right bound
//(0,-1) lowewst bound
// and (0,x) as upper bound where x can go to infty
// (0,x-1) as lower bound
class Camera
{

public:
        Camera(unsigned int windowWidth, unsigned int windowHeight)
            : windowWidth(windowWidth), windowHeight(windowHeight)
        {
        }

//        Camera(){};

        // returns the scaled position(for sfml)
        std::pair<float, float> scaledPosition(float positionX, float positionY) const
        {
                float wCentrex = windowWidth / 2.f;
                float wCentrey = windowHeight / 2.f;
                positionX = wCentrex + (wCentrex * positionX);                      // scaled pixelwaarde X
                positionY = wCentrey - ((positionY - currentMaxHeight) * wCentrey); // scaled pixelwaarde Y

                return {positionX, positionY};
        }

        // updates the camera height if the player has reached half the screen
        std::pair<bool, int> updateMaxHeight(std::pair<float, float> playerPos)
        {
                if (playerPos.second > currentMaxHeight) {
                        int scoreIncrease = ceil((playerPos.second - currentMaxHeight) * 100);
                        currentMaxHeight = playerPos.second;
                        upperBound = currentMaxHeight + 1;
                        lowerBound = currentMaxHeight - 1;
                        Random::getInstance()->calcDifficulty(
                            currentMaxHeight); // updates the difficulty based on camera height
                        return {true, scoreIncrease};
                }
                return {false, 0};
        }

        // returns true  if the coordinates are in view of the camera
        bool checkView(std::pair<float, float> position) const
        {
                if (position.second > lowerBound - 0.1) { // if it is in cameraview (with a little margin)
                        return true;
                } else {
                        return false;
                }
        }

        // returns true if the coordinates are above the camera view
        bool screenFilled(std::pair<float, float> entityPosition) const
        {
                // is used to check if screen is filled with platforms
                if (entityPosition.second > upperBound) {
                        return true;
                } else {
                        return false;
                }
        }

private:
        float currentMaxHeight{0}; // Y-value
        float upperBound{1};       // upper and lower bounds, left and right bounds are always the same
        float lowerBound{-1};
        unsigned int windowWidth = 0;
        unsigned int windowHeight = 0;
};

#endif // TESTSFML_CAMERA_H
