//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_ABSTRACTFACTORY_H
#define TESTSFML_ABSTRACTFACTORY_H
#include "Camera.h"
#include "EntityModel.h"
#include "Random.h"
#include "memory"
class AbstractFactory
{
public:
        // Creates PlayerModel and attaches PlayerView as observer
        virtual std::unique_ptr<PlayerModel> createPlayer(float x, float y) = 0;
        // Creates PlatformModel and attaches PlatformView as observer
        virtual std::unique_ptr<PlatformModel> createPlatform(float x, float y, PlatformType::Type ptype) = 0;
        // Creates BonusModel and attaches PlayerView as observer
        virtual std::unique_ptr<BonusModel> createBonus(const std::unique_ptr<PlatformModel>& platform,
                                                        BonusType::Type btype) = 0;
        // Creates BGTileModel and attaches BGTileView as observer
        virtual std::unique_ptr<BGTileModel> createBGTile(float x, float y) = 0;
        virtual ~AbstractFactory() = default;
};

#endif // TESTSFML_ABSTRACTFACTORY_H
