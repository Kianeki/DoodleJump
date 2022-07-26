//
// Created by Kiani on 7/12/2021.
//

#ifndef DOODLEJUMP_STOPWATCH_H
#define DOODLEJUMP_STOPWATCH_H

#include "chrono"
#include "memory"
class Stopwatch
{
public:
        static std::shared_ptr<Stopwatch> getInstance();

        // returns elapsed time in seconds since last call
        float getElapsedTime();
        // returns the amount of time one frame takes
        float getTimePerFrame() const;

private:
        Stopwatch();

        std::chrono::high_resolution_clock::time_point previousTimePoint;
        float elapsedTime = 0.f;
        float timePerFrame = 1 / 60.f; // 1/framerate
};

#endif // DOODLEJUMP_STOPWATCH_H
