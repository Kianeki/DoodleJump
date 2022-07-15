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
        explicit ConcreteFactory(std::shared_ptr<sf::RenderWindow> window) : AbstractFactory()
        {
                gameWindow = std::move(window);
                if (!backgroundTexture.loadFromFile("Textures/background2.png")) {
                        throw std::runtime_error("Failed  to load texture : Textures/background2.png");
                }
                if (!scoreFont.loadFromFile("arial.ttf")) {
                        throw std::runtime_error("Failed  to load font : arial.ttf");
                }
                if (!playerTexture.loadFromFile("Textures/doodle-pow.png")) {
                        throw std::runtime_error("Failed  to load texture : Textures/doodle-pow.png");
                }
                if (!springTexture.loadFromFile("Textures/spring.png")) {
                        throw std::runtime_error("Failed  to load texture : Textures/spring.png");
                }
        }
        virtual ~ConcreteFactory() override = default;
        std::unique_ptr<PlayerModel> createPlayer(float x, float y) override
        {

                std::unique_ptr<PlayerModel> playerModel = std::make_unique<PlayerModel>(x, y);
                std::unique_ptr<EntityView> playerView = std::make_unique<PlayerView>(
                    playerTexture, playerModel->getWidth(), playerModel->getHeight(), gameWindow);

                std::unique_ptr<ScoreView> scoreView = std::make_unique<ScoreView>(
                    std::pair<int, int>(gameWindow->getSize().x / 2, 30), gameWindow, scoreFont);
                std::unique_ptr<HPView> HP_view = std::make_unique<HPView>(
                                                     std::pair<int, int>(playerModel->getPosition().first, playerModel->getPosition().second+playerModel->getHeight()*2), gameWindow, scoreFont, playerModel->getMaxHp());
                // attach PlayerView to Playermodel (add as observer)
                playerModel->addObserver(std::move(playerView));
                playerModel->addObserver(std::move(scoreView));
                playerModel->addObserver(std::move(HP_view));
                return playerModel;
        }
        std::unique_ptr<PlatformModel> createPlatform(float x, float y, PlatformType::Type ptype) override
        {
                std::unique_ptr<PlatformModel> platformModel = std::make_unique<PlatformModel>(x, y, ptype);
                std::unique_ptr<EntityView> platformView = std::make_unique<PlatformView>(
                    ptype, platformModel->getWidth(), platformModel->getHeight(), gameWindow);

                platformModel->addObserver(std::move(platformView));
                return platformModel;
        }
        std::unique_ptr<BGTileModel> createBGTile(float x, float y) override
        {
                std::unique_ptr<BGTileModel> bgtileModel = std::make_unique<BGTileModel>(x, y);
                std::unique_ptr<BGTileView> bgtileView = std::make_unique<BGTileView>(
                    backgroundTexture, bgtileModel->getWidth(), bgtileModel->getHeight(), gameWindow);
                bgtileModel->addObserver(std::move(bgtileView));
                return bgtileModel;
        }
        std::unique_ptr<BonusModel> createBonus(const std::unique_ptr<PlatformModel>& platform,
                                                BonusType::Type btype) override
        {
                std::unique_ptr<BonusModel> bonusModel = std::make_unique<BonusModel>(platform, btype);
                std::unique_ptr<BonusView> bonusView = std::make_unique<BonusView>(
                    springTexture, btype, bonusModel->getWidth(), bonusModel->getHeight(), gameWindow);
                bonusModel->addObserver(std::move(bonusView));
                return bonusModel;
        }

private:
        std::shared_ptr<sf::RenderWindow> gameWindow = nullptr;
        sf::Texture backgroundTexture;
        sf::Font scoreFont;
        sf::Texture springTexture;
        sf::Texture playerTexture;
};

#endif // TESTSFML_CONCRETEFACTORY_H
