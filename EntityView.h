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
        EntityView(float width, float height, std::shared_ptr<sf::RenderWindow> window) : gameWindow(std::move(window))
        {
                width *= gameWindow->getSize().x;
                height *= gameWindow->getSize().y;
                entityVisual.setSize(sf::Vector2f(width, height));
                // set origin in centre
                sf::FloatRect bounds = entityVisual.getLocalBounds();
                entityVisual.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        }

        virtual ~EntityView() override = default;

        void onNotify(Alert::Type alert, std::pair<float, float> scaledPos) override
        {
                switch (alert) {
                case Alert::drawRequest:
                        updatePosition(scaledPos);
                        drawOnScreen();
                        break;
                }
        }
        void updatePosition(std::pair<float, float> scaledPosition)
        {
                entityVisual.setPosition(scaledPosition.first, scaledPosition.second);
        }
        void drawOnScreen() { gameWindow->draw(entityVisual); }

protected:
        std::shared_ptr<sf::RenderWindow> gameWindow;
        sf::RectangleShape entityVisual;
};

class BulletView : public EntityView
{
public:
        BulletView(float width, float height,std::shared_ptr<sf::RenderWindow> window, BulletType::Type bulletType)
            : EntityView(width, height, std::move(window)){
                if(bulletType == BulletType::friendly){
                        entityVisual.setFillColor(sf::Color(100,100,100));
                }
                else if( bulletType == BulletType::enemy){
                        entityVisual.setFillColor(sf::Color(255,100,100));
                }
                entityVisual.setOutlineThickness(2);
                entityVisual.setOutlineColor(sf::Color::Black);
        }
        ~BulletView() override = default;

};
class PlayerView : public EntityView
{
public:
        PlayerView(sf::Texture& playerTexture, float width, float height, std::shared_ptr<sf::RenderWindow> window)
            : EntityView(width, height, std::move(window))
        {
                entityVisual.setTexture(&playerTexture);
        }
        ~PlayerView() override = default;

private:
};

class EnemyView : public EntityView
{
public:
        EnemyView(float width, float height, EnemyType::Type etype,std::shared_ptr<sf::RenderWindow> window) : EntityView(width, height, window){
                if(etype == EnemyType::weak){
                        entityVisual.setFillColor(sf::Color(200,30,30));
                }
                else if (etype == EnemyType::strong){
                        entityVisual.setFillColor(sf::Color(100,30,200));
                }
                entityVisual.setOutlineColor(sf::Color::Black);
                entityVisual.setOutlineThickness(2);
        }
};
class PlatformView : public EntityView
{
public:
        PlatformView(PlatformType::Type randomType, float width, float height, std::shared_ptr<sf::RenderWindow> window)
            : EntityView(width, height, std::move(window))
        {
                entityVisual.setOutlineColor(sf::Color::Black);
                entityVisual.setOutlineThickness(1);
                switch (randomType) {
                case PlatformType::staticP:
                        entityVisual.setFillColor(sf::Color::Green);
                        break;
                case PlatformType::horizontalP:
                        entityVisual.setFillColor(sf::Color::Blue);
                        break;
                case PlatformType::horizontalPteleport:
                        entityVisual.setFillColor(sf::Color::Blue);
                        entityVisual.setOutlineThickness(-3);
                        entityVisual.setOutlineColor(sf::Color::White);
                        break;
                case PlatformType::temporaryP:
                        entityVisual.setFillColor(sf::Color::White);
                        break;
                case PlatformType::verticalP:
                        entityVisual.setFillColor(sf::Color::Yellow);
                        break;
                case PlatformType::verticalPteleport:
                        entityVisual.setFillColor(sf::Color::Yellow);
                        entityVisual.setOutlineThickness(-3);
                        entityVisual.setOutlineColor(sf::Color::White);
                        break;
                }

        }
        virtual ~PlatformView() override = default;

private:
};

class BonusView : public EntityView
{
public:
        BonusView(sf::Texture& bonusTexture, BonusType::Type btype, float width, float height,
                  std::shared_ptr<sf::RenderWindow> window)
            : EntityView(width, height, std::move(window))
        {
                switch (btype) {
                case BonusType::spring:
                        entityVisual.setTexture(&bonusTexture);
                        break;
                case BonusType::jetpack:
                        entityVisual.setFillColor(sf::Color::Magenta);
                        break;
                case BonusType::heart:
                        entityVisual.setFillColor(sf::Color::Red);
                        break;
                case BonusType::spike:
                        entityVisual.setFillColor(sf::Color::Black);
                        break;
                }
        }
        virtual ~BonusView() override = default;
};

class BGTileView : public EntityView
{
public:
        BGTileView(sf::Texture& background, float width, float height, std::shared_ptr<sf::RenderWindow> window)
            : EntityView(width, height, std::move(window))
        {
                entityVisual.setTexture(&background);
        }

        virtual ~BGTileView() override = default;
};
#endif // TESTSFML_ENTITYVIEW_H
