#include "Time.h"

void Time::reset() {
    startClock = clock();
    lastFrameClock = startClock;
    deltaTime = 0.0f;
    totalTime = 0.0f;
    frameCount = 0;
    fps = 0.0f;
}
void Time::update() {
    clock_t current_clock = clock();
    deltaTime = static_cast<float>(current_clock - lastFrameClock) / CLOCKS_PER_SEC;
    lastFrameClock = current_clock;
    totalTime = static_cast<float>(current_clock - startClock) / CLOCKS_PER_SEC;
    frameCount++;
    if (totalTime >= 1.0f) {
        fps = static_cast<float>(frameCount) / totalTime;
        frameCount = 0;
        startClock = current_clock;
    }
}

float Time::getDeltaTime() const {
    return deltaTime;
}

float Time::getTotalTime() const {
    return totalTime;
}

float Time::getFPS() const {
    return fps;
}