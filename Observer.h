//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_OBSERVER_H
#define TESTSFML_OBSERVER_H
#include "utility"
namespace Alert {
    enum Type {
        drawRequest,
        increaseScore,
        decreaseScore
    };
}
class Observer {

public:
    virtual void onNotify(Alert::Type alert, std::pair<float,float> scaledPos) =0;
    virtual ~Observer()=default;
};


#endif //TESTSFML_OBSERVER_H
