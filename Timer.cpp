//Timer.cpp

#include "Timer.h"

Timer::Timer()
{
}

void Timer::MarkStart()
{
    m_startTime = std::chrono::steady_clock::now();
}
void Timer::MarkEnd()
{
    m_endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> tempDuration = std::chrono::duration_cast<std::chrono::nanoseconds> (m_endTime - m_startTime);
    m_duration = tempDuration.count();

}

auto Timer::GetDuration() const -> double
{
    return m_duration;
}
