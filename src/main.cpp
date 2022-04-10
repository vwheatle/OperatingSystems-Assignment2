#include <iostream>

#include <vector>
#include <string>

#include "data.hpp"

int main() {
	auto dataOpt = BankerData::load("../src/data.txt");
	if (!dataOpt.has_value()) {
		std::cout << "Failed to load data file!" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	auto data = dataOpt.value();
	if (data.empty()) {
		std::cout << "Something went wrong during parsing!" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	std::cout << "Loaded data, here's information about each resource:" << std::endl;
	for (const auto& item : data) {
		std::cout << "Resource: { " << std::endl;
		std::cout << '\t' << item.total << std::endl;
		std::cout << '\t'; printVector(item.allocated);
		std::cout << '\t'; printVector(item.maximum);
		std::cout << "}" << std::endl;
	}
	
	return EXIT_SUCCESS;
}
