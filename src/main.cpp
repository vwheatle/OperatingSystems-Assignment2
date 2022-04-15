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
	
	auto data = dataOpt.value();
	std::cout << "Resources:" << std::endl;
	for (const auto& rsrc : data.resources) {
		std::cout << "Resource: { " << rsrc.available << " / " << rsrc.total << " }" << std::endl;
	}
	std::cout << "Processes:" << std::endl;
	for (const auto& proc : data.processes) {
		std::cout << "Process: { " << std::endl;
		std::cout << "\t Allocated: "; printVector(proc.allocated);
		std::cout << "\t Maximum:   "; printVector(proc.maximum);
		std::cout << "}" << std::endl;
	}
	
	return EXIT_SUCCESS;
}
