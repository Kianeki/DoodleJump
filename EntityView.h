//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_ENTITYVIEW_H
#define TESTSFML_ENTITYVIEW_H


#include <utility>
#include "Observer.h"
#include "SFML/Graphics.hpp"
#include "memory"
#include "Random.h"

class EntityView : public Observer {
public:

    EntityView(float width, float height, std::shared_ptr<sf::RenderWindow> window ) : gameWindow(std::move(window)){
//        entityVisual.setPosition(scaledPos.first,scaledPos.second);
            width *= gameWindow->getSize().x;
            height *= gameWindow->getSize().y;
            entityVisual.setSize(sf::Vector2f(width,height));
    }
    virtual void onNotify(Alert::Type alert, std::pair<float,float> scaledPos) override=0;

    void updatePosition(std::pair<float,float> scaledPosition){
        entityVisual.setPosition(scaledPosition.first, scaledPosition.second);
    }
    void drawOnScreen(){
        gameWindow->draw(entityVisual);
    }
protected:
    std::shared_ptr<sf::RenderWindow> gameWindow;
    sf::RectangleShape entityVisual;
};

class PlayerView : public EntityView{
public:
    void onNotify(Alert::Type alert, std::pair<float,float> scaledPos) override{
        switch(alert){
            case Alert::drawRequest:
                updatePosition(scaledPos);
                drawOnScreen();
                break;
//            case Alert::updatePosition:
//                updatePosition(scaledPos);
//                break;
        }

    }
    PlayerView(float width, float height, std::shared_ptr<sf::RenderWindow> window) : EntityView(width,height, std::move(window)){
        entityVisual.setFillColor(sf::Color::Red);
//        entityVisual.setPosition(scaledPos.first,scaledPos.second);
        sf::FloatRect bounds = entityVisual.getLocalBounds();
        entityVisual.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }
//    void drawOnScreen(){
//        gameWindow->draw(entityVisual);
//    }
//    void updatePosition(std::pair<float,float> scaledPosition){
//        entityVisual.setPosition(scaledPosition.first, scaledPosition.second);
//    }
private:
//    sf::RectangleShape player;
};

class PlatformView : public EntityView{
public:
    void onNotify(Alert::Type alert, std::pair<float,float> scaledPos) override{
        switch(alert) {
            case Alert::drawRequest:
                updatePosition(scaledPos);
                drawOnScreen();
                break;
//            case Alert::updatePosition:
//                updatePosition(scaledPos);
//                break;
        }
    }
    PlatformView(PlatformType::Type randomType, float width, float height, std::shared_ptr<sf::RenderWindow> window) : EntityView(width, height, std::move(window)){
        switch(randomType){
            case PlatformType::staticP:
                entityVisual.setFillColor(sf::Color::Green);
                break;
            case PlatformType::horizontalP:
                entityVisual.setFillColor(sf::Color::Blue);
                break;
            case PlatformType::temporaryP:
                entityVisual.setFillColor(sf::Color::White);
                break;
            case PlatformType::verticalP:
                entityVisual.setFillColor(sf::Color::Yellow);
                break;
        }
//        entityVisual.setFillColor(sf::Color::Green);
//        entityVisual.setPosition(scaledPos.first,scaledPos.second);
        sf::FloatRect bounds = entityVisual.getLocalBounds();
        entityVisual.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    }
//    void drawOnScreen(){
//        gameWindow->draw(platform);
//    }
private:

//    sf::RectangleShape platform;
};

class BonusView : public EntityView{
public:
    void onNotify(Alert::Type alert, std::pair<float,float> scaledPos) override{

    }
};

class BGTileView : public EntityView{
public:
    void onNotify(Alert::Type alert, std::pair<float,float> scaledPos) override{

    }
};
#endif //TESTSFML_ENTITYVIEW_H
