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
        horizontalPteleport = 50,
        verticalP = 75,
        verticalPteleport = 25

}; // these int values are for score calc
}
namespace BonusType {
enum Type
{
        none = 0,
        spring = 500,
        jetpack = 2000,
        heart = 250,
        spike = -1000,
}; // these int values are for score calc
}
namespace EnemyType {
enum Type
{
        none = 0,
        weak = 5000,
        strong = 8000
};
}
namespace BulletType {
enum Type
{
        friendly = 1,
        enemy = 2
};
}
class Random
{
public:
        static std::shared_ptr<Random> getInstance();

        // returns X value between -1 and 1
        float randomPlatformX(float platformWidth);

        // returns Y value depending on difficulty
        float randomPlatformY(float platformHeight, float minPlatformDistance);
        // return random teleport Y-value for vertical platform
        float randomTeleportPlatformY(const float lowerBound, const float upperBound);
        // returns a random PlatformType depending on difficulty
        PlatformType::Type randomPlatformType();

        // returns a random BonusType
        BonusType::Type randomBonusType();

        // return a random enemyType
        EnemyType::Type randomEnemyType();
        // scales difficulty(max 3) depending on camera height
        void calcDifficulty(float currentMaxHeight);

private:
        Random();
        std::default_random_engine generator;
        int difficulty = 0; // max 4
};

#endif // DOODLEJUMP_RANDOM_H
