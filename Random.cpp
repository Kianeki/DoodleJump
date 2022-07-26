//
// Created by Kiani on 7/12/2021.
//

#include "Random.h"
std::shared_ptr<Random> Random::getInstance()
{
        static std::shared_ptr<Random> instance = std::shared_ptr<Random>(new Random());
        return instance;
}
float Random::randomPlatformX(float platformWidth)
{ //-1 is the left bound, +1 is the right bound of the screen
        std::uniform_real_distribution<double> distribution(-1.0 + platformWidth, 1.0 - platformWidth);
        // adding/subtracting platformwidth ensures platforms don't clip out of view
        return distribution(generator);
}
float Random::randomPlatformY(float platformHeight, float minPlatformDistance)
{ // platformHeight is the thickness of the platform
        std::uniform_real_distribution<double> distribution(3 * platformHeight * (1.0 + difficulty),
                                                            (minPlatformDistance / 2) +
                                                            (minPlatformDistance * difficulty / 9.f));
        return distribution(generator);
}
float Random::randomTeleportPlatformY(const float lowerBound, const float upperBound)
{
        std::uniform_real_distribution<double> distribution(lowerBound, upperBound);
        return distribution(generator);
}
PlatformType::Type Random::randomPlatformType()
{
        std::uniform_int_distribution<int> distribution(0 + difficulty,
                                                        4 + difficulty); // 0+difficulty <= x <= 4+difficulty
        int randomNumber = distribution(generator);
        if (randomNumber < 4) {
                return PlatformType::staticP;
        } else if (randomNumber == 4) {
                return PlatformType::temporaryP;
        } else if (randomNumber == 5 ) {
                return PlatformType::horizontalP;
        } else if(randomNumber == 6){
                return PlatformType::horizontalPteleport;
        }
        else if (randomNumber == 7) {
                return PlatformType::verticalP;
        }
        else if (randomNumber == 8) {
                return PlatformType::verticalPteleport;
        }
        return PlatformType::Type(); // Need to throw exception here
}
BonusType::Type Random::randomBonusType()
{
        std::uniform_int_distribution<int> distribution(0, 100); // 0 <= x <= 100
        int randomNumber = distribution(generator);
        if (randomNumber < 10 && difficulty > 0) {
                return BonusType::spring;
        }
        else if (randomNumber >=10 && randomNumber <20 && difficulty > 0) {
                return BonusType::heart;
        }
        else if (randomNumber >=20 && randomNumber <30 && difficulty > 0) {
                return BonusType::spike;
        }
        else if (randomNumber > 95 && difficulty > 1) {
                return BonusType::jetpack;
        }
        return BonusType::none;
}
Random::Random() { generator.seed(time(nullptr)); }
EnemyType::Type Random::randomEnemyType()
{
        std::uniform_int_distribution<int> distribution(0, 100); // 0 <= x <= 100
        int randomNumber = distribution(generator);
        if (randomNumber < 10 && difficulty > 0) {
                return EnemyType::weak;
        }
        else if (randomNumber > 95 && difficulty > 1) {
                return EnemyType::strong;
        }
        return EnemyType::none;
}
void Random::calcDifficulty(float currentMaxHeight)
{
        if (currentMaxHeight > 150) {
                difficulty = 4;
        }
        else if (currentMaxHeight > 80) {
                difficulty = 3;
        } else if (currentMaxHeight > 50) {
                difficulty = 2;
        } else if (currentMaxHeight > 20) {
                difficulty = 1;
        }
}
