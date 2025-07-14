#pragma once

#include <string>

class StringUtilities {

public:	
	static time_t StringToTimeT(const std::string& timeString);
	static bool ValidateDate(const std::string& date, time_t& outTime);
};