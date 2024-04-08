#pragma once

#include <chrono>

using TimePoint = std::chrono::time_point<std::chrono::steady_clock>;

class Timer{

public:
    Timer();
    void MarkStart();
    void MarkEnd();
    auto GetDuration() const -> double;

private:
    TimePoint m_startTime{};
    TimePoint m_endTime{};
    double m_duration{};
};
