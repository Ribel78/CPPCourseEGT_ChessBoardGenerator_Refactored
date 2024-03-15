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
    m_numberOfSimulations += 1;
}

void Timer::updateStats()
{
    m_simulationTime = m_simTime.count();
    m_numberOfSimulations += 1;
    m_totalSimulationTime += m_simulationTime;
    m_averageSimulationTime = m_totalSimulationTime / m_numberOfSimulations;
}

/*
Format time statistics for dynamic text display 
Returns a string */

std::string Timer::simulationTimeToString() const
{
	std::string timeStatsString = "";
	timeStatsString.append("Number of simulations: ");
    timeStatsString.append(std::to_string(m_numberOfSimulations));
	timeStatsString.append("\n");
	timeStatsString.append("Current Simulation Time: ");
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
