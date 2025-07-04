#include <ctime>
#include <string>
#include "string_utilities.h"

// Add this function to convert ISO 8601 string to time_t
time_t StringUtilities::StringToTimeT(const std::string& timeString)
{
    struct tm tm = {};

    // Parse ISO 8601 format: "YYYY-MM-DDTHH:MM:SSZ"
    if (sscanf_s(timeString.c_str(), "%d-%d-%dT%d:%d:%dZ",
        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
        &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {

        tm.tm_year -= 1900;  // tm_year is years since 1900
        tm.tm_mon -= 1;      // tm_mon is 0-11
        tm.tm_isdst = 0;     // UTC time, no DST

        // Convert to time_t (assumes input is UTC)
        return _mkgmtime(&tm);
    }

    return -1; // Error parsing
}