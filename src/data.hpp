#pragma once

#include <iostream>
#include <fstream>

#include <vector>
#include <string>

#include <algorithm>

#include <optional> // -> Option<T> -- or i mean.. std::optional<T>

template <typename T>
void printVector(const std::vector<T>& a) {
	std::cout << "{ ";
	for (const auto& s : a)
		std::cout << s << ", ";
	std::cout << "}" << std::endl;
}
void printVector(const std::vector<std::string>& a) {
	std::cout << '{' << std::endl;
	for (const auto& s : a)
		std::cout << "\t\"" << s << "\"," << std::endl;
	std::cout << '}' << std::endl;
}

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

auto spaceSeparatedNumbers(const std::string& s) -> std::vector<int> {
	std::vector<std::string> sep = splitString(s, " ");
	std::vector<int> res; res.reserve(sep.size());
	for (const auto& v : sep) {
		res.push_back(std::atoi(v.c_str()));
	}
	return res;
}

namespace BankerData {
	// Yeah, I'm slicing the data column-wise, because I think I'll do column
	// operations more often than row operations. If I'm wrong, then I'll live
	// with this anyway.
	struct Resource {
		int total;
		std::vector<int> allocated;
		std::vector<int> maximum;
	};
	
	auto load(const std::string& filename) -> std::optional<std::vector<Resource>> {
		std::fstream file; file.open(filename, std::ios::in);
		if (file.fail()) return std::nullopt;
		
		// Here's the buffer we'll repeatedly write to, for reading lines!
		std::string buff; buff.reserve(16);
		auto nextLine = [&file, &buff]() -> const std::string& {
			std::getline(file, buff); return buff;
		};
		// getline skips over the \n.
		
		if (nextLine() != "Resources") return std::nullopt;
		auto rsrcTotals = spaceSeparatedNumbers(nextLine());
		
		if (!nextLine().empty()) return std::nullopt;
		
		if (nextLine() != "Allocated") return std::nullopt;
		
		std::vector<std::vector<int>> rsrcsAllocated;
		rsrcsAllocated.resize(rsrcTotals.size());
		// resize will initialize every vector inside with the correct value.
		// reserve just makes space for them. not good! leads to segfault.
		
		while (std::getline(file, buff), !buff.empty()) {
			auto lineAllocated = spaceSeparatedNumbers(buff);
			if (rsrcTotals.size() != lineAllocated.size()) return std::nullopt;
			
			for (auto i = 0; i < rsrcTotals.size(); i++) {
				rsrcsAllocated[i].push_back(lineAllocated[i]);
			}
		}
		
		if (nextLine() != "Maximum") return std::nullopt;
		
		std::vector<std::vector<int>> rsrcsMaximum;
		rsrcsMaximum.resize(rsrcTotals.size());
		
		while (std::getline(file, buff), !buff.empty()) {
			auto lineMaximum = spaceSeparatedNumbers(buff);
			if (rsrcTotals.size() != lineMaximum.size()) return std::nullopt;
			
			for (auto i = 0; i < rsrcTotals.size(); i++) {
				rsrcsMaximum[i].push_back(lineMaximum[i]);
			}
		}
		
		std::vector<Resource> result;
		for (auto i = 0; i < rsrcTotals.size(); i++) {
			result.push_back({
				rsrcTotals[i],
				std::move(rsrcsAllocated[i]),
				std::move(rsrcsMaximum[i]),
			});
		}
		return std::make_optional(result);
	}
};
