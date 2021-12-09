//
// Created by Kiani on 7/12/2021.
//

#include "Game.h"

Game::Game(): mainWindow(sf::VideoMode(600,900),"DoodleJump"), visualPlayer() {
    float playerHeight = world.player->getHeight(); //measurements of player char
    float playerWidth = world.player->getWidth(); //measurements of player char
    visualPlayer.setFillColor(sf::Color::Green);
//    sf::FloatRect bounds = visualPlayer.getLocalBounds();
//    visualPlayer.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    auto windowSize = mainWindow.getSize();
    visualPlayer.setPosition(windowSize.x / 2.f, windowSize.y / 2.f); //set visualPlayer at middle of window, is later updated in gameloop
    camera.setWindowSize(windowSize.x,windowSize.y);
    auto playerSize = visualPlayer.getSize();
    playerSize.x= windowSize.x*playerWidth;
    playerSize.y = windowSize.y * playerHeight;
    visualPlayer.setSize(playerSize); //scale player according to window size
    sf::FloatRect bounds = visualPlayer.getLocalBounds();
    visualPlayer.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void Game::run() {
    while (mainWindow.isOpen())
    {
        processEvents();
        update();
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mainWindow.pollEvent(event)){
        switch (event.type) {
//            case sf::Event::KeyPressed:
//                handlePlayerInput(event.key.code,true);
//                break;
//
//            case sf::Event::KeyReleased:
//                handlePlayerInput(event.key.code, false);
//                break;

            case sf::Event::Closed:
                mainWindow.close();
                break;
        }
    }
}

void Game::update() {
    std::pair<float,float> playerMovement{0,0};
    playerMovement.second+=0.001;
    world.movePlayer(playerMovement);
    camera.updateMaxHeight(world.player); // update the max height according to the Player, should always be centered
    camera.checkView(world.entities); // check if platforms are in view, delete if out of view
}

void Game::render() {
    mainWindow.clear();
//    camera.checkView(world.entities);
    drawEntities(world.entities); //draw all platforms
    std::pair<float,float> playerPos = world.player->getScaledPosition(); //actual Player in the logic world
    visualPlayer.setPosition(playerPos.first, playerPos.second);
    mainWindow.draw(visualPlayer);
    mainWindow.display();
}

void Game::drawEntities(const std::list<std::unique_ptr<Entity>>& entities) {
    auto windowSize= mainWindow.getSize();
    for(const auto& entity : entities){
        sf::RectangleShape visualPlatform;
        visualPlatform.setFillColor(sf::Color::White);
        float width = entity->getWidth();
        float height = entity->getHeight();
        width= windowSize.x*width;
        height= windowSize.y*height;
        visualPlatform.setSize(sf::Vector2f(width, height));
//        visualPlatform.setSize(sf::Vector2f(150.f,25.f));
        sf::FloatRect bounds = visualPlatform.getLocalBounds();
        visualPlatform.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        std::pair<float,float> position= entity->getScaledPosition();
        visualPlatform.setPosition(position.first, position.second);
        mainWindow.draw(visualPlatform);
    }
}
