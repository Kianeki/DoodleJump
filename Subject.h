//
// Created by Kiani on 17/12/2021.
//

#ifndef TESTSFML_SUBJECT_H
#define TESTSFML_SUBJECT_H
#include "Observer.h"
#include "list"
#include "memory"
#include "EntityView.h"
class Subject {

public:
    void addObserver(std::unique_ptr<Observer> observer){

        observers.push_back(std::move(observer));
    }
    void removeObserver(){
//        observers.remove(observer);
    }
protected:
    void notify(Alert::Type alert, std::pair<float,float> scaledPos){
        for(auto& observer: observers){
            observer->onNotify(alert, scaledPos);
        }
    }

private:
    std::list<std::unique_ptr<Observer>> observers;
};


#endif //TESTSFML_SUBJECT_H
