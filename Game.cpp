//
// Created by Kiani on 7/12/2021.
//

#include "Game.h"
#include "Stopwatch.h"

Game::Game(): mainWindow(sf::VideoMode(600,900),"DoodleJump"), visualPlayer() { //600,900
    float playerHeight = world.player->getHeight(); //measurements of player char
    float playerWidth = world.player->getWidth(); //measurements of player char
    visualPlayer.setFillColor(sf::Color::Cyan);
//    sf::FloatRect bounds = visualPlayer.getLocalBounds();
//    visualPlayer.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    auto windowSize = mainWindow.getSize();
    visualPlayer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f); //set visualPlayer at middle of window, is later updated in gameloop
    world.setWindowSize(windowSize.x,windowSize.y);
    auto playerSize = visualPlayer.getSize();
    playerSize.x= windowSize.x*playerWidth;
    playerSize.y = windowSize.y * playerHeight;
    visualPlayer.setSize(playerSize); //scale player according to window size
    sf::FloatRect bounds = visualPlayer.getLocalBounds();
    visualPlayer.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Game::run() {
    std::shared_ptr<Stopwatch> klok = Stopwatch::getInstance();
    float timeSinceLastTick = klok->getElapsedTime();
    while (mainWindow.isOpen())
    {
        processEvents();
        timeSinceLastTick +=klok->getElapsedTime();
        float timePerFrame = klok->getTimePerFrame();
        while(timeSinceLastTick > timePerFrame){
            timeSinceLastTick -= timePerFrame;
            processEvents();
            update(timePerFrame);
        }
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mainWindow.pollEvent(event)){
        switch (event.type) {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code,true);
                break;

            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;

            case sf::Event::Closed:
                mainWindow.close();
                break;
        }
    }
}

void Game::update(float timePerFrame) {
    world.generatePlatforms();
    world.checkCollision();
    world.movePlayer(timePerFrame);
    world.updateMaxHeight(); // update the max height according to the Player, should always be centered
    world.movePlatforms(timePerFrame);
    world.updateCameraView(); // check if platforms are in view, delete if out of view
}

void Game::render() {
    mainWindow.clear();
//    camera.checkView(world.entities);
    drawEntities(world.getEntities()); //draw all platforms
    std::pair<float,float> playerPos = world.player->getScaledPosition(); //actual Player in the logic world
    visualPlayer.setPosition(playerPos.first, playerPos.second);
    mainWindow.draw(visualPlayer);
    mainWindow.display();
//    std::cout<<world.getEntities().size()<<std::endl;
}

void Game::drawEntities(const std::list<std::unique_ptr<Entity>>& entities) {
    auto windowSize= mainWindow.getSize();
    for(const auto& entity : entities){
        sf::RectangleShape visualPlatform;
        platformType::Type type = entity->getPlatformType();
        switch(type){
            case platformType::staticP:
                visualPlatform.setFillColor(sf::Color::Green);
                break;

            case platformType::temporaryP:
                visualPlatform.setFillColor(sf::Color::White);
                break;

            case platformType::horizontalP:
                visualPlatform.setFillColor(sf::Color::Blue);
                break;
            case platformType::verticalP:
                visualPlatform.setFillColor(sf::Color::Yellow);
        }
        float width = entity->getWidth();
        float height = entity->getHeight();
        width= windowSize.x*width;
        height= windowSize.y*height;
        visualPlatform.setSize(sf::Vector2f(width, height));
        sf::FloatRect bounds = visualPlatform.getLocalBounds();
        visualPlatform.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        std::pair<float,float> position= entity->getScaledPosition();
        visualPlatform.setPosition(position.first, position.second);
        mainWindow.draw(visualPlatform);
    }
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if(key == sf::Keyboard::Left ||key ==sf::Keyboard::A ){
        world.movePlayerLeft(isPressed);
    }
    else if(key == sf::Keyboard::Right ||key ==sf::Keyboard::D ){
        world.movePlayerRight(isPressed);
    }
}
