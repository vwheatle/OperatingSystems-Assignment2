#pragma once

#include <cerrno>

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <algorithm>
#include <numeric>

#include <optional> // -> Option<T> -- or i mean.. std::optional<T>

// Utility function to print out the contents of a vector.
template <typename T>
void printVector(const std::vector<T>& a) {
	std::cout << "{ ";
	for (const auto& s : a)
		std::cout << s << ", ";
	std::cout << "}" << std::endl;
}
// Now with a special case for string vectors!
void printVector(const std::vector<std::string>& a) {
	std::cout << '{' << std::endl;
	for (const auto& s : a)
		std::cout << "\t\"" << s << "\"," << std::endl;
	std::cout << '}' << std::endl;
}

// Splits a string with the given delimiter,
// limiting the resulting vector to at most maxSplits elements.
auto splitString(
	const std::string& s,
	const std::string& delim,
	int maxSplits = 64
) -> std::vector<std::string> {
	std::vector<std::string> result;
	
	std::size_t prevHitEnd = 0;
	std::size_t nextHit;
	while (
		(nextHit = s.find(delim, prevHitEnd)) != std::string::npos
		&& --maxSplits > 0
	) {
		result.push_back(s.substr(prevHitEnd, nextHit - prevHitEnd));
		prevHitEnd = nextHit + delim.length();
	}
	
	result.push_back(s.substr(prevHitEnd));
	
	return result;
}

// Takes a space-separated string with numbers and returns the numbers in it.
auto spaceSeparatedNumbers(const std::string& s) -> std::vector<int> {
	std::vector<std::string> sep = splitString(s, " ");
	std::vector<int> res; res.reserve(sep.size());
	for (const auto& v : sep) {
		res.push_back(std::atoi(v.c_str()));
	}
	return res;
}

namespace BankerData {
	struct Resource {
		int available;
		int total;
	};
	
	struct Process {
		std::vector<int> allocated;
		std::vector<int> maximum;
	};
	
	struct ProcessResourceMatrix {
		std::vector<Resource> resources;
		std::vector<Process> processes;
		
		// Sets the `available` field on all resources to their correct values,
		// according to the `total` field.
		void updateResourceAvailables() {
			auto i = 0;
			for (auto& rsrc : resources) {
				rsrc.available = rsrc.total;
				for (const auto& proc : processes)
					rsrc.available -= proc.allocated[i];
				i++;
			}
		}
		
		// Sets the `total` field on all resources to their correct values,
		// according to the `available` field.
		void updateResourceTotals() {
			auto i = 0;
			for (auto& rsrc : resources) {
				rsrc.total = rsrc.available;
				for (const auto& proc : processes)
					rsrc.total += proc.allocated[i];
				i++;
			}
		}
	};
	
	auto load(const std::string& filename) -> std::optional<ProcessResourceMatrix> {
		#define opt_assert(x, failMsg)\
			do if (!(x)) {\
				std::cout << failMsg << std::endl;\
				return std::nullopt;\
			} while (0)
		
		#define opt_assert_p(x, failMsg)\
			do if (!(x)) {\
				perror(failMsg);\
				return std::nullopt;\
			} while (0)
		
		// Open the file, and return None if it fails.
		std::fstream file; file.open(filename, std::ios::in);
		opt_assert_p(!file.fail(), "File failed to open");
		
		// Here's the buffer we'll repeatedly write to, for reading lines!
		std::string buff; buff.reserve(16);
		auto nextLine = [&file, &buff]() -> const std::string& {
			std::getline(file, buff); return buff;
		};
		// getline skips over the \n.
		
		opt_assert(nextLine() == "Resources", "\"Resources\" header missing or not in order");
		
		// Too lazy to do tagged unions, so this'll do.
		bool rsrcIsTotal;
		if (nextLine() == "Total") {
			rsrcIsTotal = true;
		} else if (buff == "Available") {
			rsrcIsTotal = false;
		} else {
			opt_assert(false, "Neither \"Total\" or \"Available\" subheaders found in Resources section");
		}
		auto rsrcInfo = spaceSeparatedNumbers(nextLine());
		
		// assert that there are at least some resources
		auto rsrcCount = rsrcInfo.size();
		opt_assert(rsrcCount > 0, "No resources defined");
		
		// assert that there's an empty line between these two sections
		opt_assert(nextLine().empty(), "Empty line expected (between Resources and Allocated sections)");
		
		// "Allocated" section -- each line representing a process
		opt_assert(nextLine() == "Allocated", "\"Allocated\" header missing or not in order");
		std::vector<std::vector<int>> procAllocatedRsrc;
		while (std::getline(file, buff), !buff.empty()) {
			auto line = spaceSeparatedNumbers(buff);
			opt_assert(line.size() == rsrcCount, "Line in \"Allocated\" section has invalid amount of resources");
			procAllocatedRsrc.push_back(std::move(line));
		}
		
		// empty line was consumed by prev fn, so we shouldn't check for it.
		
		// "Maximum" section -- each line representing a process
		opt_assert(nextLine() == "Maximum", "\"Maximum\" header missing or not in order");
		std::vector<std::vector<int>> procMaximumRsrc;
		while (std::getline(file, buff), !buff.empty()) {
			auto line = spaceSeparatedNumbers(buff);
			opt_assert(line.size() == rsrcCount, "Line in \"Maximum\" section has invalid amount of resources");
			procMaximumRsrc.push_back(std::move(line));
		}
		
		// empty line was consumed by prev fn, so we shouldn't check for it.
		
		// assert that both allocated and maximum are the same length.
		opt_assert(procAllocatedRsrc.size() == procMaximumRsrc.size(), "\"Allocated\" and \"Maximum\" do not define same amount of processes");
		
		// assert that there are at least some processes
		auto procCount = procMaximumRsrc.size();
		opt_assert(procCount > 0, "No processes defined");
		
		// Finally, format everything correctly...
		
		std::vector<Resource> resources; resources.reserve(rsrcCount);
		if (rsrcIsTotal) {
			for (const auto& item : rsrcInfo)
				resources.push_back({ .available = 0, .total = item, });
		} else {
			for (const auto& item : rsrcInfo)
				resources.push_back({ .available = item, .total = 0, });
		}
		
		std::vector<Process> processes;
		for (size_t i = 0; i < procCount; i++) {
			processes.push_back({
				.allocated = std::move(procAllocatedRsrc[i]),
				.maximum = std::move(procMaximumRsrc[i]),
			});
		}
		
		ProcessResourceMatrix result {
			.resources = resources,
			.processes = processes,
		};
		if (rsrcIsTotal) result.updateResourceAvailables();
		else             result.updateResourceTotals();
		
		// and return it!
		return std::make_optional(result);
		
		#undef opt_assert
		#undef opt_assert_p
	}
};
