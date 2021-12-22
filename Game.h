//
// Created by Kiani on 4/12/2021.
//

#ifndef TESTSFML_GAME_H
#define TESTSFML_GAME_H
#include <SFML/Graphics.hpp>
#include "Camera.h"
#include <iostream>
#include <list>
#include "ConcreteFactory.h"

#include "World.h"
#include "Stopwatch.h"
class Game {
public:
    Game();
    void run();
private:
    void processEvents();
    void update();
    void render();
    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
private:
    std::shared_ptr<sf::RenderWindow> mWindow;
    World world;
    PlayerMovement::Direction currentPlayerDirection = PlayerMovement::none;
//    PlayerMovement::Direction previousPlayerDirection = PlayerMovement::none;
    bool left=false;
    bool right=false;

};


#endif //TESTSFML_GAME_H
