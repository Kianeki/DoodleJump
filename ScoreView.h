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
        ScoreView(std::pair<int, int> pos, std::shared_ptr<sf::RenderWindow>& window, sf::Font& scoreFont);
        ~ScoreView() override = default;
        void onNotify(Alert::Type alert, std::pair<float, float> increase) override;

private:
        std::shared_ptr<sf::RenderWindow> gameWindow = nullptr;
        sf::Text scoreDisplay;
};

#endif // DOODLEJUMP_SCOREVIEW_H
