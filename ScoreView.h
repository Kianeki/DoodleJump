//
// Created by Kiani on 30/12/2021.
//

#ifndef DOODLEJUMP_SCOREVIEW_H
#define DOODLEJUMP_SCOREVIEW_H
#include "SFML/Graphics.hpp"
#include "Score.h"
#include "memory"
class ScoreView : public Score
{
public:
        ScoreView(std::pair<int, int> pos, std::shared_ptr<sf::RenderWindow>& window, sf::Font& scoreFont)
        {
                gameWindow = window;
                scoreDisplay.setFont(scoreFont);
                scoreDisplay.setCharacterSize(20);
                scoreDisplay.setPosition(pos.first, pos.second);
                //set origin in the centre
                sf::FloatRect bounds = scoreDisplay.getLocalBounds();
                scoreDisplay.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
        }
        virtual ~ScoreView() override = default;
        virtual void onNotify(Alert::Type alert, std::pair<float, float> increase) override
        {
                switch (alert) {
                case Alert::increaseScore:
                        score += increase.first;
                        break;
                case Alert::decreaseScore:
                        score -= increase.first;
                        break;
                case Alert::drawRequest:
                        scoreDisplay.setString(std::to_string(score));
                        gameWindow->draw(scoreDisplay);
                case Alert::gameOver:
                        std::ofstream scoreFile("Scores/CurrentScore.txt");
                        scoreFile << std::to_string(score);
                        scoreFile.close();
                }
        }

private:
        std::shared_ptr<sf::RenderWindow> gameWindow = nullptr;
        sf::Text scoreDisplay;
};

#endif // DOODLEJUMP_SCOREVIEW_H
