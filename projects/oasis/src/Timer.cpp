#include "Precompiled.h"

Timer::Timer() {
    startTime = std::chrono::system_clock::now();
    elapsedTime = std::chrono::milliseconds::zero(); 
}

Timer::~Timer()
{
}

void Timer::play() {
    startTime = std::chrono::system_clock::now();
}

void Timer::pause() {
    auto delta = std::chrono::system_clock::now() - startTime;
    startTime = std::chrono::system_clock::now();
    elapsedTime += delta;
}

void Timer::stop() {
    pause();
    startTime = std::chrono::system_clock::now();
    elapsedTime = std::chrono::milliseconds::zero(); 
}

void Timer::print(std::string text) {
    std::cout << text << " -> " << std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count() << " microseconds" << std::endl;
}

