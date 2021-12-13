//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_GAME_H
#define DOODLEJUMP_GAME_H
#include "SFML/Graphics.hpp"
#include "World.h"
#include "Camera.h"
#include "iostream"
class Game {
private:
    sf::RenderWindow mainWindow;
    sf::RectangleShape visualPlayer;
    World world;
//    Camera camera;
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float timePerFrame);
    void render();
    void drawEntities(const std::list<std::unique_ptr<Entity>>& entities);
};


#endif //DOODLEJUMP_GAME_H
