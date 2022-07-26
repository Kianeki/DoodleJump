//
// Created by Kiani on 7/12/2021.
//

#include "Camera.h"

Camera::Camera(unsigned int windowWidth, unsigned int windowHeight)
    : windowWidth(windowWidth), windowHeight(windowHeight)
{
}
std::pair<float, float> Camera::scaledPosition(float positionX, float positionY) const
{
        float wCentrex = windowWidth / 2.f;
        float wCentrey = windowHeight / 2.f;
        positionX = wCentrex + (wCentrex * positionX);                      // scaled pixelwaarde X
        positionY = wCentrey - ((positionY - currentMaxHeight) * wCentrey); // scaled pixelwaarde Y

        return {positionX, positionY};
}
std::pair<bool, int> Camera::updateMaxHeight(std::pair<float, float> playerPos)
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
bool Camera::checkView(std::pair<float, float> position) const
{
        if (position.second > lowerBound - 0.2 &&
            position.second < upperBound + 0.1) { // if pos is in cameraview (with a little margin)
                return true;
        } else {
                return false;
        }
}
bool Camera::screenFilled(std::pair<float, float> entityPosition) const
{
        if (entityPosition.second > upperBound) {
                return true;
        } else {
                return false;
        }
}
