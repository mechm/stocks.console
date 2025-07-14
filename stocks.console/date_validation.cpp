#include <ctime>
#include <string>
#include <iostream>

#include "date_validation.h"

void GetValidDateOrEmpty(time_t& outTime) {
    
    std::string date;

    while (true) {
        std::cout << "Choose date today or in the past "
            "(YYYY-MM-DD, YYYY-MM-DDTHH:MM:SSZ format), or leave blank : ";
        
        std::getline(std::cin, date); // Use getline to capture empty input                

        // If empty, return immediately
        if (date.empty()) {
            return;
        }

        // Validate the date
        if (ValidateDate(date, outTime)) {
            return;
        }

        // Invalid date, ask again
        std::cout << "Please enter a valid date and try again." << std::endl;
    }
}

bool ValidateDate(const std::string& date, time_t& outTime) {
   
    try 
    {
        outTime = StringToTimeT(date);

        // Additional validation: check if the converted time is reasonable
        if (outTime > 0) {
            // Check if date is not in the future (markets don't have future data)
            time_t currentTime = std::time(nullptr);
            if (outTime <= currentTime) {
                return true;
            }
            else {
                std::cout << "Error: Date cannot be in the future." << std::endl;
                return false;
            }
        }
        else {
            std::cout << "Error: Invalid date format. Please use ISO 8601 format (YYYY-MM-DD, YYYY-MM-DDTHH:MM:SSZ, etc.)." << std::endl;
            return false;
        }
    }
    catch (const std::exception) {
        std::cout << "Error: Invalid date format. Please use ISO 8601 format (YYYY-MM-DD, YYYY-MM-DDTHH:MM:SSZ, etc.)." << std::endl;
        return false;
    }
}

time_t StringToTimeT(const std::string& timeString)
{
    struct tm tm = {};

    // Initialize time components to default values
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    tm.tm_isdst = 0;     // UTC time, no DST

    // Try to parse full ISO 8601 format: "YYYY-MM-DDTHH:MM:SSZ"
    if (sscanf_s(timeString.c_str(), "%d-%d-%dT%d:%d:%dZ",
        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
        &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {

        tm.tm_year -= 1900;  // tm_year is years since 1900
        tm.tm_mon -= 1;      // tm_mon is 0-11
        return _mkgmtime(&tm);
    }

    // Try to parse with hour and minute only: "YYYY-MM-DDTHH:MMZ"
    if (sscanf_s(timeString.c_str(), "%d-%d-%dT%d:%dZ",
        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
        &tm.tm_hour, &tm.tm_min) == 5) {

        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        tm.tm_sec = 0;  // Default seconds to 0
        return _mkgmtime(&tm);
    }

    // Try to parse with hour only: "YYYY-MM-DDTHH Z"
    if (sscanf_s(timeString.c_str(), "%d-%d-%dT%dZ",
        &tm.tm_year, &tm.tm_mon, &tm.tm_mday, &tm.tm_hour) == 4) {

        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        tm.tm_min = 0;  // Default minutes to 0
        tm.tm_sec = 0;  // Default seconds to 0
        return _mkgmtime(&tm);
    }

    // Try to parse date only: "YYYY-MM-DD"
    if (sscanf_s(timeString.c_str(), "%d-%d-%d",
        &tm.tm_year, &tm.tm_mon, &tm.tm_mday) == 3) {

        tm.tm_year -= 1900;
        tm.tm_mon -= 1;
        tm.tm_hour = 0;  // Default hour to 0 (midnight)
        tm.tm_min = 0;   // Default minutes to 0
        tm.tm_sec = 0;   // Default seconds to 0
        return _mkgmtime(&tm);
    }

    return -1; // Error parsing
}
