#pragma once

#include <string>
#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;
using TimeDuration = std::chrono::duration<double>;

class Timer{

public:
    Timer();
    void markStart();
    void markEnd();
    void setDurationInNanoseconds();
    void updateStats();
    auto simulationTimeStatistics(std::string chess_pieces) const -> std::string;
    auto getSimulationTime() const -> double;
    void reset();

private:
    TimePoint m_startTime;
    TimePoint m_endTime;
    TimeDuration m_simTime;
    int m_numberOfSimulations;
    double m_totalSimulationTime;
    double m_averageSimulationTime;
    double m_simulationTime;

};
