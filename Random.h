//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_RANDOM_H
#define DOODLEJUMP_RANDOM_H
#include "memory"
#include <random>
#include "ctime"
class Random {
public:
    static std::shared_ptr<Random> getInstance(){
        static std::shared_ptr<Random> instance = std::shared_ptr<Random>(new Random());
        return instance;
    }
    float randomPlatformX(){
        std::uniform_real_distribution<double> distribution(-0.5,0.5);
        return distribution(generator);
    }
    float randomPlatformY(){
        std::uniform_real_distribution<double> distribution(0.2,0.6);
        return distribution(generator);
    }
    int randomPlatformType(){
        std::uniform_int_distribution<int> distribution(0,4);
        return distribution(generator);
    }
private:
    Random(){
        generator.seed(time(nullptr));
    }
    std::default_random_engine generator;

};


#endif //DOODLEJUMP_RANDOM_H
