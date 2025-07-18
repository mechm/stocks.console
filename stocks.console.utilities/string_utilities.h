#pragma once

#include <string>

class StringUtilities {

public:
	static std::string TimeToUtcIso8601(const time_t& start);
};