//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_GAME_H
#define DOODLEJUMP_GAME_H
#include "SFML/Graphics.hpp"
#include "World.h"
#include "Camera.h"

class Game {
private:
    sf::RenderWindow mainWindow;
    sf::CircleShape player;
    World world;
    Camera camera;
public:
    Game();
    void run();

private:
    void processEvents();
    void update();
    void render();
    void drawEntities(std::list<std::shared_ptr<Entity>> entities);
};


#endif //DOODLEJUMP_GAME_H
