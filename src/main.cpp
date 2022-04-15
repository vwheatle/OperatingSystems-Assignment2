#include <iostream>

#include <vector>
#include <string>

#include "data.hpp"

int main(int argc, char* argv[]) {
	if (argc < 2) {
		std::cout << "Please specify a path to a data file." << std::endl;
		return EXIT_FAILURE;
	}
	
	auto dataOpt = BankerData::load(argv[1]);
	if (!dataOpt.has_value()) {
		std::cout << "Failed to load data file!" << std::endl;
		return EXIT_FAILURE;
	}
	
	// Retrieve data from within the optional.
	auto data = dataOpt.value();
	
	// Store a list of which processes terminated when.
	std::vector<int> terminationOrder;
	auto procsToTerminate = data.processes.size();
	terminationOrder.reserve(procsToTerminate);
	
	bool terminatedOne;
	do {
		// Was the algorithm able to terminate
		// at least one process in this recent loop?
		terminatedOne = false;
		
		// Loop through each process, terminating it
		// if we have the proper resources to do so.
		int i = 0;
		for (auto& proc : data.processes) {
			if (!proc.terminated && data.canTerminate(proc)) {
				terminationOrder.push_back(i);
				
				data.terminate(proc);
				terminatedOne = true;
				procsToTerminate--;
			}
			i++;
		}
		
		// Continue looping while there's still processes to terminate
		// and while you're still making *some* progress every time.
	} while (terminatedOne && procsToTerminate > 0);
	
	if (procsToTerminate > 0) {
		// If there's still processes left to terminate, then...
		std::cout << "This system is not in a safe state." << std::endl;
		std::cout << procsToTerminate << " processes still must terminate, but they are deadlocked." << std::endl;
	} else {
		// Otherwise, we succeeded! Plus, we can show
		// what order the processes terminated in.
		std::cout << "This system is in a safe state. " << std::endl;
		
		std::cout << "Termination order:";
		for (const auto& procNo : terminationOrder) {
			std::cout << " P" << procNo;
		}
		std::cout << std::endl;
	}
	
	return EXIT_SUCCESS;
}
