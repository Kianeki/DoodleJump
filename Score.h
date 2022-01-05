//
// Created by Kiani on 30/12/2021.
//

#ifndef DOODLEJUMP_SCORE_H
#define DOODLEJUMP_SCORE_H

#include "Observer.h"
#include "fstream"
class Score : public Observer
{
public:
        virtual ~Score() override = default;
        virtual void onNotify(Alert::Type alert, std::pair<float, float> increase) override
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

protected:
        int score = 0;
};

#endif // DOODLEJUMP_SCORE_H
