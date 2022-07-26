//
// Created by Kiani on 7/12/2021.
//

#ifndef TESTSFML_CAMERA_H
#define TESTSFML_CAMERA_H

#include "Random.h"
#include <math.h>
#include <utility>

/** We will be using a coordinate system with (0,0) as origin,
(-1,0) as left bound
(1,0) as right bound
(0,-1) lowest bound
and (0,x) as upper bound where x can go to infinity
(0,x-1) as lower bound at any given time
*/
class Camera
{

public:
        Camera(unsigned int windowWidth, unsigned int windowHeight);

        // returns the scaled position(for sfml)
        std::pair<float, float> scaledPosition(float positionX, float positionY) const;

        // updates the camera height if the player has reached half the screen
        std::pair<bool, int> updateMaxHeight(std::pair<float, float> playerPos);

        // returns true  if the coordinates are in view of the camera
        bool checkView(std::pair<float, float> position) const;

        /**
         * returns true if the coordinates are above the camera view
         * is used to check if screen is filled with platforms
         */
        bool screenFilled(std::pair<float, float> entityPosition) const;

private:
        float currentMaxHeight{0}; // Y-value
        float upperBound{1};       // upper and lower bounds, left and right bounds are always the same {-1, 1}
        float lowerBound{-1};
        unsigned int windowWidth = 0;
        unsigned int windowHeight = 0;
};

#endif // TESTSFML_CAMERA_H
