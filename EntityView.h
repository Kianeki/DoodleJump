//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_ENTITYVIEW_H
#define TESTSFML_ENTITYVIEW_H

#include "Observer.h"
#include "Random.h"
#include "SFML/Graphics.hpp"
#include "memory"
#include <utility>

class EntityView : public Observer
{
public:
        EntityView(float width, float height, std::shared_ptr<sf::RenderWindow> window);

        ~EntityView() override = default;

        // is called by the entityModel class and tells EntityView what function it should call
        void onNotify(Alert::Type alert, std::pair<float, float> scaledPos) override;

        void updatePosition(std::pair<float, float> scaledPosition);
        void drawOnScreen();

protected:
        std::shared_ptr<sf::RenderWindow> gameWindow;
        sf::RectangleShape entityVisual;
};

class BulletView : public EntityView
{
public:
        BulletView(float width, float height, std::shared_ptr<sf::RenderWindow> window, BulletType::Type bulletType);
        ~BulletView() override = default;
};
class PlayerView : public EntityView
{
public:
        PlayerView(const sf::Texture& playerTexture, float width, float height,
                   std::shared_ptr<sf::RenderWindow> window);
        ~PlayerView() override = default;

private:
};

class EnemyView : public EntityView
{
public:
        EnemyView(const sf::Texture& enemyTexture, float width, float height, EnemyType::Type etype,
                  std::shared_ptr<sf::RenderWindow> window);
        ~EnemyView() override = default;
};
class PlatformView : public EntityView
{
public:
        PlatformView(PlatformType::Type randomType, float width, float height,
                     std::shared_ptr<sf::RenderWindow> window);
        ~PlatformView() override = default;

private:
};

class BonusView : public EntityView
{
public:
        BonusView(const sf::Texture& bonusTexture, float width, float height, std::shared_ptr<sf::RenderWindow> window);
        ~BonusView() override = default;
};

class BGTileView : public EntityView
{
public:
        BGTileView(const sf::Texture& background, float width, float height, std::shared_ptr<sf::RenderWindow> window);

        ~BGTileView() override = default;
};
#endif // TESTSFML_ENTITYVIEW_H
