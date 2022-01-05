//
// Created by Kiani on 4/12/2021.
//

#ifndef TESTSFML_GAME_H
#define TESTSFML_GAME_H
#include "Camera.h"
#include "ConcreteFactory.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>

#include "Stopwatch.h"
#include "World.h"
class Game
{
public:
        Game();
        // main game loop
        void run();

private:
        // notices player input
        void processEvents();
        // updates game logic
        void update();
        // draws entities on screen
        void render();
        // handle player input
        void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
        // draws gameOver screen with scores
        void gameOver();
        // reads scores from file
        std::string readScoreFromFile(std::string fileName);

private:
        std::shared_ptr<sf::RenderWindow> mWindow;
        World world;
        PlayerMovement::Direction currentPlayerDirection = PlayerMovement::none;
        bool left = false;
        bool right = false;
};

#endif // TESTSFML_GAME_H
