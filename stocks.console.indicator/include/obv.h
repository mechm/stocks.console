#pragma once

#include <vector>

void printOBVHelp();
std::vector<long long> calculateOBV(const std::vector<double>& closingPrices, const std::vector<long long>& volumes);
std::vector<int> getOBVSignals(const std::vector<double>& prices, const std::vector<long long>& obv_values);