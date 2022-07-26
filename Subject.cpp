//
// Created by Kiani on 17/12/2021.
//

#include "Subject.h"
void Subject::notify(Alert::Type alert, std::pair<float, float> scaledPos)
{
    for (auto& observer : observers) {
        observer->onNotify(alert, scaledPos);
    }
}
void Subject::addObserver(std::unique_ptr<Observer> observer) { observers.push_back(std::move(observer)); }
