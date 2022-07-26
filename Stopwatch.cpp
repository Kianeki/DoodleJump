//
// Created by Kiani on 7/12/2021.
//

#include "Stopwatch.h"
std::shared_ptr<Stopwatch> Stopwatch::getInstance()
{
    static std::shared_ptr<Stopwatch> instance = std::shared_ptr<Stopwatch>(new Stopwatch());
    return instance;
}
Stopwatch::Stopwatch() { previousTimePoint = std::chrono::high_resolution_clock::now(); }
float Stopwatch::getElapsedTime()
{
        std::chrono::high_resolution_clock::time_point currentTimePoint =
            std::chrono::high_resolution_clock::now();
        elapsedTime =
            std::chrono::duration_cast<std::chrono::microseconds>(currentTimePoint - previousTimePoint)
                .count() /
            1000000.f;
        previousTimePoint = currentTimePoint;
        return elapsedTime;
}
float Stopwatch::getTimePerFrame() const { return timePerFrame; }
