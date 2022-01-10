//
// Created by Kiani on 4/12/2021.
//

#include "Game.h"

void Game::run()
{

        std::shared_ptr<Stopwatch> klok = Stopwatch::getInstance();
        float timeSinceLastTick = klok->getElapsedTime();
        while (mWindow->isOpen()) {
                processEvents();
                timeSinceLastTick += klok->getElapsedTime();
                float timePerFrame = klok->getTimePerFrame();
                while (timeSinceLastTick >
                       timePerFrame) { // if pc is slow, logic calc will be prioritized over rendering
                        timeSinceLastTick -= timePerFrame;
                        processEvents();
                        update();
                }
                render();
        }
}

Game::Game()
    : mWindow(std::shared_ptr<sf::RenderWindow>(
          std::make_shared<sf::RenderWindow>(sf::VideoMode(600, 900), "Doodle Jump"))),
      world(std::unique_ptr<AbstractFactory>(new ConcreteFactory(mWindow)), mWindow->getSize().x, mWindow->getSize().y)
{
}
void Game::processEvents()
{
        sf::Event event;
        while (mWindow->pollEvent(event)) {
                switch (event.type) {
                case sf::Event::KeyPressed:
                        handlePlayerInput(event.key.code, true);
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
void Game::update()
{
        world.generateBackground();
        world.generateRandomEntities();
        if (!world.movePlayer()) {
                // if the player reaches the bottom of the screen, it will return false and the game will end
                gameOver();
        };
        world.checkCollision();
        world.moveEntities();
}

void Game::render()
{
        mWindow->clear();
        world.drawBackground();
        world.drawEntities();
        world.drawPlayer();
        mWindow->display();
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
        if (isPressed) {
                if (key == sf::Keyboard::Left || key == sf::Keyboard::A) {
                        left = true;
                        currentPlayerDirection = PlayerMovement::left;
                        //            world.setPlayerDirection(PlayerMovement::left)
                } else if (key == sf::Keyboard::Right || key == sf::Keyboard::D) {
                        right = true;
                        currentPlayerDirection = PlayerMovement::right;
                }
        } else {
                if (key == sf::Keyboard::Left || key == sf::Keyboard::A) {
                        left = false;
                        if (right) {
                                currentPlayerDirection = PlayerMovement::right;
                        } else {
                                currentPlayerDirection = PlayerMovement::none;
                        }
                } else if (key == sf::Keyboard::Right || key == sf::Keyboard::D) {
                        right = false;
                        if (left) {
                                currentPlayerDirection = PlayerMovement::left;
                        } else {
                                currentPlayerDirection = PlayerMovement::none;
                        }
                }
        }
        world.setPlayerDirection(currentPlayerDirection); // left, right or none
}

void Game::gameOver()
{
        sf::Font font; // could've used resource holder class instead
        font.loadFromFile("arial.ttf");
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(30);
        text.setString("GAME OVER");
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        text.setPosition(mWindow->getSize().x / 2.f, mWindow->getSize().y / 3.f);
        sf::Text currentscore;
        currentscore.setFont(font);
        currentscore.setCharacterSize(20);
        std::string scoreValue = readScoreFromFile("Scores/CurrentScore.txt");
        currentscore.setString("SCORE: " + scoreValue);
        bounds = currentscore.getLocalBounds();
        currentscore.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        currentscore.setPosition(mWindow->getSize().x / 2.f, mWindow->getSize().y / 2.f);
        std::string highscoreValue = readScoreFromFile("Scores/HighScore.txt");
        if (highscoreValue.empty()) {
                highscoreValue = "0";
        }
        if (std::stoi(scoreValue) > std::stoi(highscoreValue)) {
                std::ofstream MyFile("Scores/HighScore.txt");
                MyFile << scoreValue;
                MyFile.close();
        }
        sf::Text highScore;
        highScore.setFont(font);
        highScore.setCharacterSize(20);
        highScore.setString("HIGHSCORE: " + readScoreFromFile("Scores/HighScore.txt"));
        bounds = highScore.getLocalBounds();
        highScore.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        highScore.setPosition(currentscore.getPosition().x,
                              currentscore.getPosition().y - 2 * currentscore.getLocalBounds().height);

        while (mWindow->isOpen()) {
                mWindow->clear();
                mWindow->draw(text);
                mWindow->draw(currentscore);
                mWindow->draw(highScore);
                mWindow->display();
                processEvents();
        }
        exit(0);
}

std::string Game::readScoreFromFile(const std::string& fileName)
{
        std::string score;
        std::ifstream file(fileName);
        std::getline(file, score);
        file.close();
        return score;
}
