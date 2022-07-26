//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_SUBJECT_H
#define TESTSFML_SUBJECT_H

#include "EntityView.h"
#include "Observer.h"
#include "list"
#include "memory"

class Subject
{

public:
        void addObserver(std::unique_ptr<Observer> observer);

protected:
        void notify(Alert::Type alert, std::pair<float, float> scaledPos);

private:
        std::list<std::unique_ptr<Observer>> observers;
};

#endif // TESTSFML_SUBJECT_H
