#pragma once

#include <string>

class DateUtilities {

public:
 static time_t Iso8601ToTime(const std::string& iso8601);
};