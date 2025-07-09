#include <ctime>
#include <string>
#include <iostream>

#include "string_utilities.h"

// Add this function to convert ISO 8601 string to time_t
//time_t StringUtilities::StringToTimeT(const std::string& timeString)
//{
//    struct tm tm = {};
//
//    // Parse ISO 8601 format: "YYYY-MM-DDTHH:MM:SSZ"
//    if (sscanf_s(timeString.c_str(), "%d-%d-%dT%d:%d:%dZ",
//        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
//        &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
//
//        tm.tm_year -= 1900;  // tm_year is years since 1900
//        tm.tm_mon -= 1;      // tm_mon is 0-11
//        tm.tm_isdst = 0;     // UTC time, no DST
//
//        // Convert to time_t (assumes input is UTC)
//        return _mkgmtime(&tm);
//    }
//
//    return -1; // Error parsing
//}



//FormatAsISO8601(dateStr, 0, 0, 0);


//bool StringUtilities::ValidateDate(const std::string& date, time_t& outTime) {
//    try {
//        outTime = StringUtilities::StringToTimeT(date);
//
//        // Additional validation: check if the converted time is reasonable
//        if (outTime > 0) {
//            // Check if date is not in the future (markets don't have future data)
//            time_t currentTime = std::time(nullptr);
//            if (outTime <= currentTime) {
//                return true;
//            }
//            else {
//                std::cout << "Error: Date cannot be in the future." << std::endl;
//                return false;
//            }
//        }
//        else {
//            std::cout << "Error: Invalid date format. Please use YYYY-MM-DD format." << std::endl;
//            return false;
//        }
//    }
//    catch (const std::exception& e) {
//        std::cout << "Error: Invalid date format. Please use YYYY-MM-DD format." << std::endl;
//        return false;
//    }
//}