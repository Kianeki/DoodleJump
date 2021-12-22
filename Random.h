//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_RANDOM_H
#define DOODLEJUMP_RANDOM_H
#include "memory"
#include <random>
#include <iostream>
#include "ctime"
namespace PlatformType{
    enum Type{staticP,temporaryP,horizontalP,verticalP};
}

class Random {
public:
    static std::shared_ptr<Random> getInstance(){
        static std::shared_ptr<Random> instance = std::shared_ptr<Random>(new Random());
        return instance;
    }
    float randomPlatformX(float platformWidth){//-1 is the left bound, +1 is the right bound of the screen
        std::uniform_real_distribution<double> distribution(-1.0+platformWidth,1.0-platformWidth);//adding/subtracting platformwidth ensures platforms are within view
        return distribution(generator);
    }
    float randomPlatformY(float platformHeight, float minPlatformDistance){ // platformHeight is the thickness of the platform
        std::uniform_real_distribution<double> distribution(3*platformHeight*(1.0+ difficulty), (minPlatformDistance / 2) + (minPlatformDistance * difficulty / 9));
        return distribution(generator);
    }
    PlatformType::Type randomPlatformType(){
        std::uniform_int_distribution<int> distribution(0+difficulty,4+difficulty); //0+difficulty <= x <= 4+difficulty
        int randomNumber = distribution(generator);
        if(randomNumber<4){
            return PlatformType::staticP;
        }
        else if (randomNumber==4){
            return PlatformType::temporaryP;
        }
        else if(randomNumber==5 || randomNumber==6){
            return PlatformType::horizontalP;
        }
        else if(randomNumber==7){
            return PlatformType::verticalP;
        }
        return PlatformType::Type(); //Need to throw exception here
    }
private:
    Random(){
        generator.seed(time(nullptr));
    }
    std::default_random_engine generator;
    int difficulty=3; //max 3
};


#endif //DOODLEJUMP_RANDOM_H
