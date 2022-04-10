#include <iostream>

#include <vector>
#include <string>

#include "data.hpp"

int main() {
	// lookAtTheVector(splitString("hello world i am so cool", " "));
	// lookAtTheVector(splitString("hello world i am the opposite :(", " ", 4));
	// lookAtTheVector(splitString("FEARS..EARS..COOL", ".."));
	
	auto dataOpt = CoolData::load("../src/data.txt");
	if (!dataOpt.has_value()) {
		std::cout << "Failed to load data file!" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	auto data = dataOpt.value();
	if (data.empty()) {
		std::cout << "Something went wrong during parsing!" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	for (const auto& item : data) {
		std::cout << "Resource: { " << std::endl;
		std::cout << '\t' << item.total << std::endl;
		std::cout << '\t'; lookAtTheVector(item.allocated);
		std::cout << '\t'; lookAtTheVector(item.maximum);
		std::cout << "}" << std::endl;
	}
	
	exit(EXIT_SUCCESS);
}
