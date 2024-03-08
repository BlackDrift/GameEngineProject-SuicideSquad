#pragma once
#include <ctime>


class Time
{
private:
    clock_t startClock;
    clock_t lastFrameClock;
    float deltaTime;  // Time elapsed since the last frame
    float totalTime;  // Total time elapsed since the start
    unsigned int frameCount;  // Number of frames rendered since the start
    float fps;  // Frames per second

public:
    Time() : deltaTime(0.0f), totalTime(0.0f) {
        reset();
    }

    void reset();
    void update();

    float getDeltaTime() const;
    float getTotalTime() const;
    float getFPS() const;

    
};
