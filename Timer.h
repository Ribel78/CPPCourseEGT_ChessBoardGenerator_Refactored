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
    std::string simulationTimeStatistics(std::string chess_pieces) const;
    double getSimulationTime() const;
    void reset();

private:
    std::chrono::time_point<std::chrono::steady_clock> m_startTime;
    std::chrono::time_point<std::chrono::steady_clock> m_endTime;
    std::chrono::duration<double> m_simTime;
    int m_numberOfSimulations;
    double m_totalSimulationTime;
    double m_averageSimulationTime;
    double m_simulationTime;

};
