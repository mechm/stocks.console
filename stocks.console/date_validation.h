#pragma once
#include <string>

void GetValidDateOrEmpty(time_t& outTime);
bool ValidateDate(const std::string& date, time_t& outTime);
time_t StringToTimeT(const std::string& timeString);