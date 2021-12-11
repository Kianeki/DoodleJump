//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_RANDOM_H
#define DOODLEJUMP_RANDOM_H
#include "memory"
#include <random>
#include "ctime"
#include "Entity.h"
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
    float randomPlatformY(float platformHeight, float jumpheight, int difficulty){ // platformHeight is the thickness of the platform
        std::uniform_real_distribution<double> distribution(3*platformHeight*(1 + difficulty),(jumpheight/2)+(jumpheight*difficulty/9));
        return distribution(generator);
    }
    platformType::Type randomPlatformType(int difficulty){
        std::uniform_int_distribution<int> distribution(0+difficulty,6+difficulty); //0+difficulty <= x <= 4+difficulty
        int randomNumber = distribution(generator);
        if(randomNumber<5){
            return platformType::staticP;
        }
        else if (randomNumber==5){
            return platformType::temporaryP;
        }
        else if(randomNumber==6 || randomNumber==7 || randomNumber==8){
            return platformType::horizontalP;
        }
        else if(randomNumber==9){
            return platformType::verticalP;
        }
        return platformType::Type(); //Need to throw exception here
    }
private:
    Random(){
        generator.seed(time(nullptr));
    }
    std::default_random_engine generator;

};


#endif //DOODLEJUMP_RANDOM_H
