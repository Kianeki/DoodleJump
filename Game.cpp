//
// Created by Kiani on 4/12/2021.
//

#include "Game.h"

void Game::run() {

    std::shared_ptr<Stopwatch> klok = Stopwatch::getInstance();
    float timeSinceLastTick = klok->getElapsedTime();
    while (mWindow->isOpen())
    {
        processEvents();
        timeSinceLastTick +=klok->getElapsedTime();
        float timePerFrame = klok->getTimePerFrame();
        while(timeSinceLastTick > timePerFrame){ //if pc is slow, logic calc will be prioritized over rendering
            timeSinceLastTick -= timePerFrame;
            processEvents();
            update();
        }
        render();
    }
}

Game::Game(): mWindow(std::shared_ptr<sf::RenderWindow>(std::make_shared<sf::RenderWindow>(sf::VideoMode(600, 900), "Doodle Jump"))),
    world(std::unique_ptr<AbstractFactory>(new ConcreteFactory(mWindow)),600, 900){


}
void Game::processEvents() {
    sf::Event event;
    while (mWindow->pollEvent(event)){
        switch (event.type) {
            case sf::Event::KeyPressed:
                handlePlayerInput(event.key.code,true);
                break;

            case sf::Event::KeyReleased:
                handlePlayerInput(event.key.code, false);
                break;

            case sf::Event::Closed:
                mWindow->close();
                break;
        }
    }
}
void Game::update() {
    world.generateRandomEntities();
    world.moveEntities();
    world.movePlayer();
    world.checkCollision();

}

void Game::render() {
    mWindow->clear();
    world.drawEntities();
    world.drawPlayer();
    //    mWindow.draw(mPlayer);
//    sf::Font font;
//    sf::Text score;
//    if(!font.loadFromFile("arial.ttf")){
//        std::cout<<"error"<<std::endl;
//    }
//    score.setFont(font);
//    score.setString("Hello Ik ben string");
//    score.setPosition(320,240);
//    score.setCharacterSize(20);
//    mWindow.draw(score);
//    score.scale(3,3);
    mWindow->display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if(isPressed){
        if(key == sf::Keyboard::Left ||key ==sf::Keyboard::A ){
            left = true;
            currentPlayerDirection = PlayerMovement::left;
//            world.setPlayerDirection(PlayerMovement::left)
        }
        else if(key == sf::Keyboard::Right ||key ==sf::Keyboard::D ){
            right = true;
            currentPlayerDirection = PlayerMovement::right;
        }
    }
    else{
        if(key == sf::Keyboard::Left ||key ==sf::Keyboard::A ){
            left = false;
            if(right){
                currentPlayerDirection = PlayerMovement::right;
            }
            else{
                currentPlayerDirection = PlayerMovement::none;
            }
        }
        else if(key == sf::Keyboard::Right ||key ==sf::Keyboard::D ){
            right = false;
            if(left){
                currentPlayerDirection = PlayerMovement::left;
            }
            else{
                currentPlayerDirection = PlayerMovement::none;
            }
        }
    }
    world.setPlayerDirection(currentPlayerDirection);

}
