//
// Created by Kiani on 17/12/2021.
//

#include "ConcreteFactory.h"
ConcreteFactory::ConcreteFactory(std::shared_ptr<sf::RenderWindow> window) : AbstractFactory()
{
    gameWindow = std::move(window);
    //load in the textures
    if (!scoreFont.loadFromFile("arial.ttf")) {
        throw std::runtime_error("Failed  to load font : arial.ttf");
    }

    sf::Texture gameTexture;
    if (!gameTexture.loadFromFile("Textures/background2.png")) {
        throw std::runtime_error("Failed  to load texture : Textures/background2.png");
    }
    else{
        gameTextures["bgTexture"] = gameTexture;
    }

    if (!gameTexture.loadFromFile("Textures/doodle-pow.png")) {
        throw std::runtime_error("Failed  to load texture : Textures/doodle-pow.png");
    }
    else{
        gameTextures["playerTexture"] = gameTexture;
    }
    if(!gameTexture.loadFromFile("Textures/spikes.png")){
        throw std::runtime_error("Failed  to load texture : Textures/spikes.png");
    }
    else{
        gameTextures["spikeTexture"] = gameTexture;
    }
    if (!gameTexture.loadFromFile("Textures/spring.png")) {
        throw std::runtime_error("Failed  to load texture : Textures/spring.png");
    }
    else{
        gameTextures["springTexture"] = gameTexture;
    }
    if (!gameTexture.loadFromFile("Textures/heart.png")) {
        throw std::runtime_error("Failed  to load texture : Textures/heart.png");
    }
    else{
        gameTextures["heartTexture"] = gameTexture;
    }
    if (!gameTexture.loadFromFile("Textures/jetpack.png")) {
        throw std::runtime_error("Failed  to load texture : Textures/jetpack.png");
    }
    else{
        gameTextures["jetpackTexture"] = gameTexture;
    }
    if (!gameTexture.loadFromFile("Textures/monster1.png")) {
        throw std::runtime_error("Failed  to load texture : Textures/monster1.png");
    }
    else{
        gameTextures["monster1Texture"] = gameTexture;
    }
    if (!gameTexture.loadFromFile("Textures/monster2.png")) {
        throw std::runtime_error("Failed  to load texture : Textures/monster2.png");
    }
    else{
        gameTextures["monster2Texture"] = gameTexture;
    }

}
std::unique_ptr<PlayerModel> ConcreteFactory::createPlayer(float x, float y)
{
        std::unique_ptr<PlayerModel> playerModel = std::make_unique<PlayerModel>(x, y);
        std::unique_ptr<EntityView> playerView = std::make_unique<PlayerView>(
            gameTextures["playerTexture"], playerModel->getWidth(), playerModel->getHeight(), gameWindow);

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
std::unique_ptr<PlatformModel> ConcreteFactory::createPlatform(float x, float y, PlatformType::Type ptype)
{
        std::unique_ptr<PlatformModel> platformModel = std::make_unique<PlatformModel>(x, y, ptype);
        std::unique_ptr<EntityView> platformView = std::make_unique<PlatformView>(
            ptype, platformModel->getWidth(), platformModel->getHeight(), gameWindow);

        platformModel->addObserver(std::move(platformView));
        return platformModel;
}
std::unique_ptr<BGTileModel> ConcreteFactory::createBGTile(float x, float y)
{
        std::unique_ptr<BGTileModel> bgtileModel = std::make_unique<BGTileModel>(x, y);
        std::unique_ptr<BGTileView> bgtileView = std::make_unique<BGTileView>(
            gameTextures["bgTexture"], bgtileModel->getWidth(), bgtileModel->getHeight(), gameWindow);
        bgtileModel->addObserver(std::move(bgtileView));
        return bgtileModel;
}
std::unique_ptr<BonusModel> ConcreteFactory::createBonus(const std::unique_ptr<PlatformModel>& platform,
                                                         BonusType::Type btype)
{
        std::unique_ptr<BonusModel> bonusModel = std::make_unique<BonusModel>(platform, btype);
        std::unique_ptr<BonusView> bonusView;
        if(btype == BonusType::spring){
                bonusView = std::make_unique<BonusView>(
                    gameTextures["springTexture"], bonusModel->getWidth(), bonusModel->getHeight(), gameWindow);
        }
        else if(btype == BonusType::spike){
                bonusView = std::make_unique<BonusView>(
                    gameTextures["spikeTexture"], bonusModel->getWidth(), bonusModel->getHeight(), gameWindow);
        }
        else if(btype == BonusType::heart){
                bonusView = std::make_unique<BonusView>(
                    gameTextures["heartTexture"], bonusModel->getWidth(), bonusModel->getHeight(), gameWindow);
        }
        else if(btype == BonusType::jetpack){
                bonusView = std::make_unique<BonusView>(
                    gameTextures["jetpackTexture"], bonusModel->getWidth(), bonusModel->getHeight(), gameWindow);
        }
        bonusModel->addObserver(std::move(bonusView));
        return bonusModel;
}
std::unique_ptr<BulletModel> ConcreteFactory::createBullet(float x, float y, BulletType::Type bulletType)
{

        std::unique_ptr<BulletModel> bulletModel = std::make_unique<BulletModel>(x,y, bulletType);
        std::unique_ptr<BulletView> bulletView = std::make_unique<BulletView>(bulletModel->getWidth(),bulletModel->getHeight(), gameWindow, bulletType);
        bulletModel->addObserver(std::move(bulletView));
        return bulletModel;
}
std::unique_ptr<EnemyModel> ConcreteFactory::createEnemy(const std::unique_ptr<PlatformModel>& platform,
                                                         EnemyType::Type etype)
{
        std::unique_ptr<EnemyModel> enemyModel = std::make_unique<EnemyModel>(platform, etype);
        std::unique_ptr<EnemyView> enemyView;
        if(etype == EnemyType::weak){
                enemyView = std::make_unique<EnemyView>(gameTextures["monster2Texture"], enemyModel->getWidth(),enemyModel->getHeight(), etype, gameWindow);
        }
        else if(etype == EnemyType::strong){
                enemyView = std::make_unique<EnemyView>(gameTextures["monster1Texture"], enemyModel->getWidth(),enemyModel->getHeight(), etype, gameWindow);
        }
        std::unique_ptr<HPView> HP_view = std::make_unique<HPView>(
            std::pair<int, int>(enemyModel->getPosition().first, enemyModel->getPosition().second+enemyModel->getHeight()*2), gameWindow, scoreFont, enemyModel->getMaxHp());
        enemyModel->addObserver(std::move(enemyView));
        enemyModel->addObserver(std::move(HP_view));
        return enemyModel;
}
