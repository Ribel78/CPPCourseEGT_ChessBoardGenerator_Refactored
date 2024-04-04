//Timer.cpp

#include "Timer.h"

Timer::Timer()
{
}

void Timer::markStart()
{
    m_startTime = std::chrono::steady_clock::now();
}
void Timer::markEnd()
{
    m_endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> tempDuration = std::chrono::duration_cast<std::chrono::nanoseconds> (m_endTime - m_startTime);
    m_duration = tempDuration.count();

}

// void Timer::setDurationInNanoseconds()
// {
//     m_durationNanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds> (m_endTime - m_startTime);
// }

void Timer::updateStatistics()
{
    //m_simulationTime = m_durationNanoseconds.count();
    m_numberOfSimulations += 1;
    m_totalSimulationTime += m_duration;
    m_averageSimulationTime = m_totalSimulationTime / m_numberOfSimulations;
}

/*
* Statistics for dynamic text display in Simulator Mode
*/
auto Timer::simulationStatistics(std::string chess_pieces) const -> std::string
{
    std::string timeStatsString = "";
    timeStatsString.append("Chess pieces: ");
    timeStatsString.append(chess_pieces);
    timeStatsString.append("\n");
	timeStatsString.append("Number of simulations: ");
    timeStatsString.append(std::to_string(m_numberOfSimulations));
	timeStatsString.append("\n");
    timeStatsString.append("Last Simulation Time: ");
    timeStatsString.append(std::to_string(m_duration));
	timeStatsString.append(" ns\n");
	timeStatsString.append("Total Simulation Time: ");
    timeStatsString.append(std::to_string(m_totalSimulationTime));
	timeStatsString.append(" ns\n");
	timeStatsString.append("Average Simulation Time: ");
    timeStatsString.append(std::to_string(m_averageSimulationTime));
	timeStatsString.append(" ns");	
	
	return 	timeStatsString;
}

auto Timer::getDuration() const -> double
{
    return m_duration;
}

void Timer::resetSimulationStatistics()
{
    m_numberOfSimulations = 0;
    m_totalSimulationTime = 0;
    m_averageSimulationTime = 0;
}
