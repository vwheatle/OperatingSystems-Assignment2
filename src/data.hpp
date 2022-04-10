#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <algorithm>

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

// Given an input stream and a buffer, this maybe returns the columns of a
// chunk of the file that looks like:
// 1 2 3\n
// 4 5 6\n
// 7 8 9\n
// \n
// So that'd return { { 1, 4, 7 }, { 2, 5, 8 }, { 3, 6, 9 } }
// Plus, you ~~can~~ have to specify a required size for each of these columns.
// Also yes, it does eat the empty line.
// Nigh-useless for anyone who's not me.
auto linesOfSpaceSeparatedNumbersAsColumnVectors(
	std::fstream& stream,
	std::string& buff,
	int requiredSize = 3
) -> std::optional<std::vector<std::vector<int>>> {
	std::vector<std::vector<int>> result;
	result.resize(requiredSize);
	// resize will initialize every vector inside with the correct value.
	// reserve just makes space for them. not good! leads to segfault.
	
	while (std::getline(stream, buff), !buff.empty()) {
		auto lineNums = spaceSeparatedNumbers(buff);
		if (requiredSize != lineNums.size()) return std::nullopt;
		
		for (auto i = 0; i < requiredSize; i++) {
			result[i].push_back(lineNums[i]);
		}
	}
	
	return std::optional(result);
}

namespace BankerData {
	// Yeah, I'm slicing the data column-wise, because I think I'll do column
	// operations more often than row operations. If I'm wrong, I'll fix it.
	struct Resource {
		int total;
		std::vector<int> allocated;
		std::vector<int> maximum;
	};
	
	auto load(const std::string& filename) -> std::optional<std::vector<Resource>> {
		// Open the file, and return None if it fails.
		std::fstream file; file.open(filename, std::ios::in);
		if (file.fail()) return std::nullopt;
		
		// Here's the buffer we'll repeatedly write to, for reading lines!
		std::string buff; buff.reserve(16);
		auto nextLine = [&file, &buff]() -> const std::string& {
			std::getline(file, buff); return buff;
		};
		// getline skips over the \n.
		
		// "Resources" section -- one line that determines both how many
		//             resources exist and each resources' maximum value
		if (nextLine() != "Resources") return std::nullopt;
		auto rsrcTotals = spaceSeparatedNumbers(nextLine());
		int expectedWidth = rsrcTotals.size();
		
		// assert that there's an empty line between these two sections
		if (!nextLine().empty()) return std::nullopt;
		
		// "Allocated" section -- each line representing a process
		if (nextLine() != "Allocated") return std::nullopt;
		auto rsrcsAllocatedOpt = linesOfSpaceSeparatedNumbersAsColumnVectors(file, buff, expectedWidth);
		if (!rsrcsAllocatedOpt.has_value()) return std::nullopt;
		auto rsrcsAllocated = *rsrcsAllocatedOpt;
		
		// empty line was consumed by prev fn, so we shouldn't check for it.
		
		// "Maximum" section -- each line representing a process
		if (nextLine() != "Maximum") return std::nullopt;
		auto rsrcsMaximumOpt = linesOfSpaceSeparatedNumbersAsColumnVectors(file, buff, expectedWidth);
		if (!rsrcsMaximumOpt.has_value()) return std::nullopt;
		auto rsrcsMaximum = *rsrcsMaximumOpt;
		
		// empty line was consumed by prev fn, so we shouldn't check for it.
		
		// Finally, format the vector correctly...
		std::vector<Resource> result;
		for (auto i = 0; i < rsrcTotals.size(); i++) {
			result.push_back({
				rsrcTotals[i],
				std::move(rsrcsAllocated[i]),
				std::move(rsrcsMaximum[i]),
			});
		}
		// and return it!
		return std::make_optional(result);
	}
};
