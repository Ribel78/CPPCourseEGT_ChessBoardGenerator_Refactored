#pragma once
#include <string>
#include <chrono>

class Timer{

public:
    Timer();
    void markStart();
    void markEnd();
    void setDurationInNanoseconds();
    void updateStats();
    std::string simulationTimeToString();

private:

std::chrono::time_point<std::chrono::steady_clock> startTime;
std::chrono::time_point<std::chrono::steady_clock> endTime;
std::chrono::duration<double> simTime;
int numberOfSimulations;
double totalSimulationTime;
double averageSimulationTime;
double simulationTime;

};