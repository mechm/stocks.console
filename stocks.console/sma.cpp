#include <iostream>
#include <vector>

#include "../stocks.console/sma.h"

/// @brief 
/// @param prices 
/// @param period 
/// @return 
double calculateSMA(const std::vector<double>& prices, const int period)
{
    double sum = 0;

    // Calculate the sum of prices over the specified period
    for (int i = 0; i < period; i++) {
        sum += prices[i];
    }

    // Calculate the SMA
    const double sma = sum / period;

    return sma;
}