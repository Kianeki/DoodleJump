//
// Created by Kiani on 30/12/2021.
//

#ifndef DOODLEJUMP_SCORE_H
#define DOODLEJUMP_SCORE_H

#include "Observer.h"

class Score: public Observer {
public:
    virtual void onNotify(Alert::Type alert, std::pair<float, float> increase) override{
        switch (alert) {
            case Alert::increaseScore:
                score+=increase.first;
                break;
            case Alert::decreaseScore:
                score-=increase.first;
                break;
        }
    }
protected:
    int score=0;

};


#endif //DOODLEJUMP_SCORE_H
