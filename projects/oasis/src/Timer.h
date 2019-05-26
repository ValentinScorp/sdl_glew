#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
    Timer();
    ~Timer();
    
    void play();
    void pause();
    void stop();
    void print(std::string text);
private:
    
     std::chrono::time_point<std::chrono::system_clock> startTime; 
     std::chrono::duration<Sint64, std::ratio<1l, 1000000000l>> elapsedTime;
};

#endif // TIMER_H
