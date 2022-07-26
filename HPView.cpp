//
// Created by kiani on 14.07.22.
//

#include "HPView.h"
HPView::HPView(std::pair<int, int> pos, std::shared_ptr<sf::RenderWindow>& window, sf::Font& HP_font, int playerHealth)
{
        HP_value = playerHealth;
        gameWindow = window;
        HP_display.setFont(HP_font);
        HP_display.setCharacterSize(26);
        HP_display.setOutlineColor(sf::Color::Black);
        HP_display.setOutlineThickness(1);
        HP_display.setPosition(pos.first, pos.second);
        // set origin in the centre
        sf::FloatRect bounds = HP_display.getLocalBounds();
        HP_display.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}
void HPView::onNotify(Alert::Type alert, std::pair<float, float> increase)
{
        switch (alert) {
        case Alert::increaseHP:
                HP_value += increase.first;
                break;
        case Alert::decreaseHP:
                HP_value -= increase.first;
                break;
        case Alert::drawRequest:
                HP_display.setString(std::to_string(HP_value));
                sf::FloatRect bounds = HP_display.getLocalBounds();
                HP_display.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
                HP_display.setPosition(increase.first, increase.second - 75);
                gameWindow->draw(HP_display);
                break;
        }
}
