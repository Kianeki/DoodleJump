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
        static std::shared_ptr<Stopwatch> getInstance()
        {
                static std::shared_ptr<Stopwatch> instance = std::shared_ptr<Stopwatch>(new Stopwatch());
                return instance;
        }

        float getElapsedTime()
        { // returns elapsed time in seconds since last call
                std::chrono::high_resolution_clock::time_point currentTimePoint =
                    std::chrono::high_resolution_clock::now();
                elapsedTime =
                    std::chrono::duration_cast<std::chrono::microseconds>(currentTimePoint - previousTimePoint)
                        .count() /
                    1000000.f;
                previousTimePoint = currentTimePoint;
                return elapsedTime;
        }

        float getTimePerFrame() const { return timePerFrame; }

private:
        Stopwatch() { previousTimePoint = std::chrono::high_resolution_clock::now(); }

        std::chrono::high_resolution_clock::time_point previousTimePoint;
        float elapsedTime = 0.f;
        float timePerFrame = 1 / 60.f; // 1/framerate
};

#endif // DOODLEJUMP_STOPWATCH_H
