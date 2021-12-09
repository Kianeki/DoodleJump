//
// Created by Kiani on 7/12/2021.
//

#include "Game.h"

Game::Game(): mainWindow(sf::VideoMode(600,900),"DoodleJump"), player(100.f) {
    player.setFillColor(sf::Color::Green);
    sf::FloatRect bounds = player.getLocalBounds();
    player.setOrigin(bounds.width/ 2.f, bounds.height /2.f);
    auto size = mainWindow.getSize();
    player.setPosition(size.x/2.f, size.y/2.f); //set player at middle of window
    camera.setWindowSize(600,900);
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
    playerMovement.second+=0.005;
    world.movePlayer(playerMovement);
    camera.updateMaxHeight(world.player); // update the max height according to the player, should always be centered
    camera.checkView(world.entities); // check if platforms are in view, delete if out of view
}

void Game::render() {
    mainWindow.clear();
//    camera.checkView(world.entities);
    drawEntities(world.entities);
    std::pair<float,float> playerPos = world.player->getScaledPosition(); //actual player in the logic world
    player.setPosition(playerPos.first,playerPos.second);
    mainWindow.draw(player);
    mainWindow.display();
}

void Game::drawEntities(const std::list<std::unique_ptr<Entity>>& entities) {
    for(const auto& entity : entities){
        sf::RectangleShape platform;
        platform.setFillColor(sf::Color::White);
        platform.setSize(sf::Vector2f(150.f,25.f));
        sf::FloatRect bounds = platform.getLocalBounds();
        platform.setOrigin(bounds.width/ 2.f, bounds.height /2.f);
        std::pair<float,float> position= entity->getScaledPosition();
        platform.setPosition(position.first,position.second);
        mainWindow.draw(platform);
    }
}
