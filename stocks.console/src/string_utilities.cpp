#include <ctime>
#include <string>
#include <iostream>

#include "../include/string_utilities.h"

std::string StringUtilities::TimeToUtcIso8601(const time_t t)
{  
    struct tm utc_tm;  
    gmtime_s(&utc_tm, &t);  
    char time_buffer[32];  
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%H:%M:%SZ", &utc_tm);  
    return std::string(time_buffer);  
}
