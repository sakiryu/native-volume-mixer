#pragma once

#include <regex>
#include <vector>

namespace utils {
	std::vector<std::string> split(std::string&& str, const std::regex& delimiter = std::regex{ "\\s+" })
	{
		std::sregex_token_iterator it{ str.begin(), str.end(), delimiter, -1 };
		std::sregex_token_iterator end{};
		return { it, end };
	}
}