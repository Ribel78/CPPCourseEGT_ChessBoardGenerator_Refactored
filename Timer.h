#pragma once

#include <string>
#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

class Timer{

public:
    Timer();
    void markStart();
    void markEnd();
    auto getDuration() const -> double;

    void updateStatistics();
    auto simulationStatistics(std::string chess_pieces) const -> std::string;
    void resetSimulationStatistics();

private:
    TimePoint m_startTime{};
    TimePoint m_endTime{};
    double m_duration{};

    int m_numberOfSimulations{};
    double m_totalSimulationTime{};
    double m_averageSimulationTime{};


};
