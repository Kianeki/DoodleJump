//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_CONCRETEFACTORY_H
#define TESTSFML_CONCRETEFACTORY_H

#include <memory>

#include "AbstractFactory.h"
#include "EntityView.h"
#include "HPView.h"
#include "ScoreView.h"
class ConcreteFactory : public AbstractFactory
{
public:
        explicit ConcreteFactory(std::shared_ptr<sf::RenderWindow> window);
        ~ConcreteFactory() override = default;

        std::unique_ptr<PlayerModel> createPlayer(float x, float y) override;

        std::unique_ptr<PlatformModel> createPlatform(float x, float y, PlatformType::Type ptype) override;

        std::unique_ptr<BGTileModel> createBGTile(float x, float y) override;

        std::unique_ptr<BonusModel> createBonus(const std::unique_ptr<PlatformModel>& platform,
                                                BonusType::Type btype) override;

        std::unique_ptr<BulletModel> createBullet(float x, float y, BulletType::Type bulletType) override;

        std::unique_ptr<EnemyModel> createEnemy(const std::unique_ptr<PlatformModel>& platform,
                                                EnemyType::Type etype) override;

private:
        std::shared_ptr<sf::RenderWindow> gameWindow = nullptr;
        sf::Font scoreFont;
        std::map<std::string, sf::Texture> gameTextures;
};

#endif // TESTSFML_CONCRETEFACTORY_H
