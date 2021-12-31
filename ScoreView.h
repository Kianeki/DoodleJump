//
// Created by Kiani on 30/12/2021.
//

#ifndef DOODLEJUMP_SCOREVIEW_H
#define DOODLEJUMP_SCOREVIEW_H
#include "Score.h"
#include "SFML/Graphics.hpp"
#include "memory"
class ScoreView: public Score{
public:
    ScoreView(std::pair<int,int> pos, std::shared_ptr<sf::RenderWindow>& window, sf::Font& scoreFont){
        gameWindow = window;
        scoreDisplay.setFont(scoreFont);
        scoreDisplay.setCharacterSize(20);
        scoreDisplay.setPosition(pos.first,pos.second);
    }
    virtual void onNotify(Alert::Type alert, std::pair<float, float> increase) override{
        switch (alert) {
            case Alert::increaseScore:
                score+=increase.first;
                break;
            case Alert::decreaseScore:
                score-=increase.first;
                break;
            case Alert::drawRequest:
                scoreDisplay.setString(std::to_string(score));
                gameWindow->draw(scoreDisplay);
//            case Alert::drawRequest:
//                gameWindow->draw()
        }
    }
private:
    std::shared_ptr<sf::RenderWindow> gameWindow=nullptr;
    sf::Text scoreDisplay;
};


#endif //DOODLEJUMP_SCOREVIEW_H
