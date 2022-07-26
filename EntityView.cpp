//
// Created by Kiani on 17/12/2021.
//

#include "EntityView.h"
EntityView::EntityView(float width, float height, std::shared_ptr<sf::RenderWindow> window)
    : gameWindow(std::move(window))
{
        width *= gameWindow->getSize().x;
        height *= gameWindow->getSize().y;
        entityVisual.setSize(sf::Vector2f(width, height));
        // set origin in centre
        sf::FloatRect bounds = entityVisual.getLocalBounds();
        entityVisual.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}
void EntityView::onNotify(Alert::Type alert, std::pair<float, float> scaledPos)
{
        switch (alert) {
                case Alert::drawRequest:
                        updatePosition(scaledPos);
                drawOnScreen();
                break;
        }
}
void EntityView::updatePosition(std::pair<float, float> scaledPosition)
{
        entityVisual.setPosition(scaledPosition.first, scaledPosition.second);
}
void EntityView::drawOnScreen() { gameWindow->draw(entityVisual); }
BulletView::BulletView(float width, float height, std::shared_ptr<sf::RenderWindow> window, BulletType::Type bulletType)
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
PlayerView::PlayerView(const sf::Texture& playerTexture, float width, float height,
                       std::shared_ptr<sf::RenderWindow> window)
    : EntityView(width, height, std::move(window))
{
        entityVisual.setTexture(&playerTexture);
}
EnemyView::EnemyView(const sf::Texture& enemyTexture, float width, float height, EnemyType::Type etype,
                     std::shared_ptr<sf::RenderWindow> window)
    : EntityView(width, height, window){
        entityVisual.setTexture(&enemyTexture);
}
PlatformView::PlatformView(PlatformType::Type randomType, float width, float height,
                           std::shared_ptr<sf::RenderWindow> window)
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
BonusView::BonusView(const sf::Texture& bonusTexture, float width, float height,
                     std::shared_ptr<sf::RenderWindow> window)
    : EntityView(width, height, std::move(window))
{
        entityVisual.setTexture(&bonusTexture);

}
BGTileView::BGTileView(const sf::Texture& background, float width, float height,
                       std::shared_ptr<sf::RenderWindow> window)
    : EntityView(width, height, std::move(window))
{
        entityVisual.setTexture(&background);
}
