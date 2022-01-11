//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_RANDOM_H
#define DOODLEJUMP_RANDOM_H

#include "memory"
#include <random>

#include "ctime"

namespace PlatformType {
enum Type
{
        staticP = 200,
        temporaryP = 0,
        horizontalP = 100,
        verticalP = 75
}; // these int values are for score calc
}
namespace BonusType {
enum Type
{
        none = 0,
        spring = 500,
        jetpack = 2000
}; // these int values are for score calc
}
class Random
{
public:
        static std::shared_ptr<Random> getInstance()
        {
                static std::shared_ptr<Random> instance = std::shared_ptr<Random>(new Random());
                return instance;
        }

        // returns X value between -1 and 1
        float randomPlatformX(float platformWidth)
        { //-1 is the left bound, +1 is the right bound of the screen
                std::uniform_real_distribution<double> distribution(-1.0 + platformWidth, 1.0 - platformWidth);
                // adding/subtracting platformwidth ensures platforms don't clip out of view
                return distribution(generator);
        }

        // returns Y value depending on difficulty
        float randomPlatformY(float platformHeight, float minPlatformDistance)
        { // platformHeight is the thickness of the platform
                std::uniform_real_distribution<double> distribution(3 * platformHeight * (1.0 + difficulty),
                                                                    (minPlatformDistance / 2) +
                                                                        (minPlatformDistance * difficulty / 9.f));
                return distribution(generator);
        }

        // returns a random PlatformType depending on difficulty
        PlatformType::Type randomPlatformType()
        {
                std::uniform_int_distribution<int> distribution(0 + difficulty,
                                                                4 + difficulty); // 0+difficulty <= x <= 4+difficulty
                int randomNumber = distribution(generator);
                if (randomNumber < 4) {
                        return PlatformType::staticP;
                } else if (randomNumber == 4) {
                        return PlatformType::temporaryP;
                } else if (randomNumber == 5 || randomNumber == 6) {
                        return PlatformType::horizontalP;
                } else if (randomNumber == 7) {
                        return PlatformType::verticalP;
                }
                return PlatformType::Type(); // Need to throw exception here
        }

        // returns a random BonusType
        BonusType::Type randomBonusType()
        {
                std::uniform_int_distribution<int> distribution(0, 100); // 0 <= x <= 10
                int randomNumber = distribution(generator);
                if (randomNumber < 10 && difficulty > 0) {
                        return BonusType::spring;
                } else if (randomNumber > 95 && difficulty > 1) {
                        return BonusType::jetpack;
                }
                return BonusType::none;
        }

        // scales difficulty(max 3) depending on camera height
        void calcDifficulty(float currentMaxHeight)
        {
                if (currentMaxHeight > 150) {
                        difficulty = 3;
                } else if (currentMaxHeight > 80) {
                        difficulty = 2;
                } else if (currentMaxHeight > 20) {
                        difficulty = 1;
                }
        }

private:
        Random() { generator.seed(time(nullptr)); }

        std::default_random_engine generator;
        int difficulty = 0; // max 3
};

#endif // DOODLEJUMP_RANDOM_H
