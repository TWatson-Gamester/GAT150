#include "Utilities.h"
#include <algorithm>

namespace gn {
	std::string string_tolower(const std::string& str) {
		std::string lower = str;

		std::transform(str.begin(), str.end(), lower.begin(), [](char c) { return std::tolower(c); });

		return lower;
	}

	bool iString_Compate(const std::string& str1, const std::string& str2){
		return std::equal(str1.begin(), str1.end(), str2.begin(), [](char c1, char c2) {return (std::tolower(c1) == std::tolower(c2)); });
	}
}