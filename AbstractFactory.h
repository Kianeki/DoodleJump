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
        // Creates PlayerModel at location (x,y) and attaches PlayerView as observer
        virtual std::unique_ptr<PlayerModel> createPlayer(float x, float y) = 0;

        // Creates PlatformModel at location (x,y)  and attaches PlatformView as observer
        virtual std::unique_ptr<PlatformModel> createPlatform(float x, float y, PlatformType::Type ptype) = 0;

        // Creates BonusModel on platform and attaches PlayerView as observer
        virtual std::unique_ptr<BonusModel> createBonus(const std::unique_ptr<PlatformModel>& platform,
                                                        BonusType::Type btype) = 0;

        // Creates BGTileModel at location (x,y)  and attaches BGTileView as observer
        virtual std::unique_ptr<BGTileModel> createBGTile(float x, float y) = 0;

        // Creates BulletModel at location (x,y) (of LivingEntity) and attaches BulletView as observer
        virtual std::unique_ptr<BulletModel> createBullet(float x, float y, BulletType::Type bulletType) = 0;

        // Creates EnemyModel on platform and attaches EnemyView as observer
        virtual std::unique_ptr<EnemyModel> createEnemy(const std::unique_ptr<PlatformModel>& platform,
                                                        EnemyType::Type etype) = 0;

        virtual ~AbstractFactory() = default;
};

#endif // TESTSFML_ABSTRACTFACTORY_H
