//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_ABSTRACTFACTORY_H
#define TESTSFML_ABSTRACTFACTORY_H
#include "EntityModel.h"
#include "memory"
#include "Camera.h"
#include "Random.h"
class AbstractFactory {
public:
    virtual std::unique_ptr<PlayerModel> createPlayer(float x, float y)=0;
    virtual std::unique_ptr<PlatformModel> createPlatform(float x, float y, PlatformType::Type ptype)=0;
    virtual std::unique_ptr<BonusModel> createBonus()=0;
    virtual std::unique_ptr<BGTileModel> createBGTile()=0;
};


#endif //TESTSFML_ABSTRACTFACTORY_H
