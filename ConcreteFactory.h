//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_CONCRETEFACTORY_H
#define TESTSFML_CONCRETEFACTORY_H


#include <memory>

#include "AbstractFactory.h"
#include "EntityView.h"

class ConcreteFactory: public AbstractFactory {
public:
    explicit ConcreteFactory(std::shared_ptr<sf::RenderWindow> window): AbstractFactory(){
        gameWindow = std::move(window);

    }
    std::unique_ptr<PlayerModel> createPlayer(float x, float y) override{
//        std::unique_ptr<Observer> observer = std::unique_ptr<EntityView>(new PlayerView());
        std::unique_ptr<PlayerModel> playerModel = std::make_unique<PlayerModel>(x,y);
        std::unique_ptr<EntityView> playerView = std::make_unique<PlayerView>(playerModel->getWidth(),playerModel->getHeight(),gameWindow);
//        std::unique_ptr<PlayerModel> playerModel = std::make_unique<PlayerModel>(x,y);
        //attach PlayerView to Playermodel (add as observer)
        playerModel->addObserver(std::move(playerView));
        return playerModel;
    }
    std::unique_ptr<PlatformModel> createPlatform(float x, float y, PlatformType::Type ptype) override{
//        std::shared_ptr<Random> random = Random::getInstance();
//        PlatformType::Type type = random->randomPlatformType();
        std::unique_ptr<PlatformModel> platformModel = std::make_unique<PlatformModel>(x,y, ptype);
        std::unique_ptr<EntityView> platformView = std::make_unique<PlatformView>(ptype,platformModel->getWidth(),platformModel->getHeight(), gameWindow);
//        std::unique_ptr<PlatformModel> platformModel = std::make_unique<PlatformModel>(x,y);
        platformModel->addObserver(std::move(platformView));
        return platformModel;
    }
    std::unique_ptr<BGTileModel> createBGTile() override{

    }
    std::unique_ptr<BonusModel> createBonus(float x, float y, BonusType::Type btype) override{

    }

private:
    std::shared_ptr<sf::RenderWindow> gameWindow = nullptr;

};


#endif //TESTSFML_CONCRETEFACTORY_H
