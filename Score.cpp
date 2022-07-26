//
// Created by Kiani on 30/12/2021.
//

#include "Score.h"
void Score::onNotify(Alert::Type alert, std::pair<float, float> increase)
{
        switch (alert) {
        case Alert::increaseScore:
                score += increase.first;
                break;
        case Alert::decreaseScore:
                score -= increase.first;
                break;
        case Alert::gameOver:
                std::ofstream scoreFile("Scores/CurrentScore.txt");
                scoreFile << std::to_string(score);
                scoreFile.close();
        }
}
