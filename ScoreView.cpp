//
// Created by Kiani on 30/12/2021.
//

#include "ScoreView.h"
ScoreView::ScoreView(std::pair<int, int> pos, std::shared_ptr<sf::RenderWindow>& window, sf::Font& scoreFont)
{
    gameWindow = window;
    scoreDisplay.setFont(scoreFont);
    scoreDisplay.setCharacterSize(26);
    scoreDisplay.setOutlineColor(sf::Color::Black);
    scoreDisplay.setOutlineThickness(1);
    scoreDisplay.setPosition(pos.first, pos.second);
    // set origin in the centre
    sf::FloatRect bounds = scoreDisplay.getLocalBounds();
    scoreDisplay.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}
void ScoreView::onNotify(Alert::Type alert, std::pair<float, float> increase)
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
                break;
        case Alert::gameOver:
                std::ofstream scoreFile("Scores/CurrentScore.txt");
                scoreFile << std::to_string(score);
                scoreFile.close();
        }
}
