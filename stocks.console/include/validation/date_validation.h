#pragma once  

#include <string>  

time_t GetValidDateOrEmpty();  
bool ValidateDate(const std::string& date, time_t& outTime);  
time_t StringToTimeT(const std::string& timeString);