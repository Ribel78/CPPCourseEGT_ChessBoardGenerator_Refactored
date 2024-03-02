#include "Timer.h"

Timer::Timer(){
	numberOfSimulations = 0;
	totalSimulationTime = 0;
	averageSimulationTime = 0;
	simulationTime = 0;
}

void Timer::markStart(){
startTime = std::chrono::steady_clock::now();

}
void Timer::markEnd(){
endTime = std::chrono::steady_clock::now();
}

void Timer::setDurationInNanoseconds(){
simTime = std::chrono::duration_cast<std::chrono::nanoseconds> (endTime - startTime);
numberOfSimulations += 1;
}

void Timer::updateStats(){
    simulationTime = simTime.count();
    numberOfSimulations += 1;
    totalSimulationTime += simulationTime;
    averageSimulationTime = totalSimulationTime / numberOfSimulations;
}

/*
Format time statistics for dynamic text display 
Returns a string */

std::string Timer::simulationTimeToString(){
	std::string timeStatsString = "";
	timeStatsString.append("Number of simulations: ");
	timeStatsString.append(std::to_string(numberOfSimulations));
	timeStatsString.append("\n");
	timeStatsString.append("Current Simulation Time: ");
	timeStatsString.append(std::to_string(simulationTime));	
	timeStatsString.append(" ns\n");
	timeStatsString.append("Total Simulation Time: ");
	timeStatsString.append(std::to_string(totalSimulationTime));
	timeStatsString.append(" ns\n");
	timeStatsString.append("Average Simulation Time: ");
	timeStatsString.append(std::to_string(averageSimulationTime));
	timeStatsString.append(" ns");	
	
	return 	timeStatsString;
}