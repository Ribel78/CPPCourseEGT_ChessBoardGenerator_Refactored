//Timer.cpp

#include "Timer.h"

Timer::Timer()
{
    m_numberOfSimulations = 0;
    m_totalSimulationTime = 0;
    m_averageSimulationTime = 0;
    m_simulationTime = 0;
}

void Timer::markStart()
{
    m_startTime = std::chrono::steady_clock::now();
}
void Timer::markEnd()
{
    m_endTime = std::chrono::steady_clock::now();
}

void Timer::setDurationInNanoseconds()
{
    m_simTime = std::chrono::duration_cast<std::chrono::nanoseconds> (m_endTime - m_startTime);
}

void Timer::updateStats()
{
    m_simulationTime = m_simTime.count();
    m_numberOfSimulations += 1;
    m_totalSimulationTime += m_simulationTime;
    m_averageSimulationTime = m_totalSimulationTime / m_numberOfSimulations;
}

/*
* Statistics for dynamic text display in Simulator Mode
*/
auto Timer::simulationTimeStatistics(std::string chess_pieces) const -> std::string
{
    std::string timeStatsString = "";
    timeStatsString.append("Chess pieces: ");
    timeStatsString.append(chess_pieces);
    timeStatsString.append("\n");
	timeStatsString.append("Number of simulations: ");
    timeStatsString.append(std::to_string(m_numberOfSimulations));
	timeStatsString.append("\n");
    timeStatsString.append("Last Simulation Time: ");
    timeStatsString.append(std::to_string(m_simulationTime));
	timeStatsString.append(" ns\n");
	timeStatsString.append("Total Simulation Time: ");
    timeStatsString.append(std::to_string(m_totalSimulationTime));
	timeStatsString.append(" ns\n");
	timeStatsString.append("Average Simulation Time: ");
    timeStatsString.append(std::to_string(m_averageSimulationTime));
	timeStatsString.append(" ns");	
	
	return 	timeStatsString;
}

auto Timer::getSimulationTime() const -> double
{
    return m_simulationTime;
}

void Timer::reset()
{
    m_numberOfSimulations = 0;
    m_totalSimulationTime = 0;
    m_averageSimulationTime = 0;
    m_simulationTime = 0;
}
