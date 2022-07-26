//
// Created by kiani on 14.07.22.
//

#ifndef DOODLEJUMP_HPVIEW_H
#define DOODLEJUMP_HPVIEW_H

#include "Observer.h"
#include "SFML/Graphics.hpp"
#include <memory>
class HPView : public Observer
{
private:
        int HP_value = 0;
        std::shared_ptr<sf::RenderWindow> gameWindow = nullptr;
        sf::Text HP_display;
public:
        HPView(std::pair<int, int> pos, std::shared_ptr<sf::RenderWindow>& window, sf::Font& HP_font, int playerHealth);
        ~HPView() override = default;
        void onNotify(Alert::Type alert, std::pair<float, float> increase) override;
};

#endif // DOODLEJUMP_HPVIEW_H
