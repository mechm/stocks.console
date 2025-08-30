// stocks.console.utilities.cpp : Defines the functions for the static library.
//

#include <ctime>
#include <string>
#include "string_utilities.h"

std::string StringUtilities::TimeToUtcIso8601(const time_t& start) {
	// Convert time_t to tm structure in UTC
	struct tm utc_tm{};
	gmtime_s(&utc_tm, &start);
	char time_buffer[32];
	strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%H:%M:%SZ", &utc_tm);
	const std::string start_time(time_buffer);
	return start_time;
}

