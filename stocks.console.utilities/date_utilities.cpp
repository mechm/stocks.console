#include <ctime>
#include <string>
#include "date_utilities.h"

time_t DateUtilities::Iso8601ToTime(const std::string& iso8601) {
    struct tm tm = {};

    // Parse ISO 8601 date format (YYYY-MM-DD)
    if (sscanf_s(iso8601.c_str(), "%d-%d-%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday) == 3) {
        tm.tm_year -= 1900; // tm_year is years since 1900
        tm.tm_mon -= 1;     // tm_mon is 0-based
        tm.tm_hour = 0;
        tm.tm_min = 0;
        tm.tm_sec = 0;
        tm.tm_isdst = -1;   // Let system determine DST

        return mktime(&tm);
    }

    return 0; // Failed to parse
}