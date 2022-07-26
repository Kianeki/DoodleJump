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
        ~Score() override = default;
        void onNotify(Alert::Type alert, std::pair<float, float> increase) override;

protected:
        int score = 0;
};

#endif // DOODLEJUMP_SCORE_H
