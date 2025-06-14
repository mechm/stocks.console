#include <iostream>
#include <vector>

#include "pch.h"
#include "framework.h"


/// @brief 
/// @param prices for a given period
/// @param period the period for the prices
/// @return moving average of prices over period
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