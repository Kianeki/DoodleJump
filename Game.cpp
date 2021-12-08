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

}

void Game::render() {
    mainWindow.clear();
    drawEntities(camera.getEntitiesinView(world.entities));
    mainWindow.draw(player);
    mainWindow.display();
}

void Game::drawEntities(std::list<std::shared_ptr<Entity>> entities) {
    for(auto entity : entities){
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
