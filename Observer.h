//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_OBSERVER_H
#define TESTSFML_OBSERVER_H
#include "utility"
namespace Alert {
    enum Type {
        drawRequest, updatePosition
    };
}
class Observer {

public:
    virtual void onNotify(Alert::Type alert, std::pair<float,float> scaledPos) =0;
};


#endif //TESTSFML_OBSERVER_H
