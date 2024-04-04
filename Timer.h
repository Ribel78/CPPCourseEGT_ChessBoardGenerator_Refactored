#pragma once

#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

class Timer{

public:
    Timer();
    void markStart();
    void markEnd();
    auto getDuration() const -> double;

private:
    TimePoint m_startTime{};
    TimePoint m_endTime{};
    double m_duration{};
};
