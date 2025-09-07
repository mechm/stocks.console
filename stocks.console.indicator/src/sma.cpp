#include <iostream>
#include <vector>

#include "../pch.h"

/// @brief Prints help information about the Simple Moving Average (SMA) indicator.
void printSMAHelp()
{
    std::cout << R"(
====================[ SMA Help ]====================
SMA (Simple Moving Average) is a technical indicator that calculates the
average price of a security over a specified number of periods.

- It smooths out price data to identify trends over time.
- Common periods are 10, 20, 50, 100, or 200 days.
- SMA is used to determine support and resistance levels and generate buy/sell signals.

Usage:
- When the price crosses above the SMA, it may indicate a BUY signal.
- When the price crosses below the SMA, it may indicate a SELL signal.

====================================================
)" << std::endl;
}

/// @brief Calculate Simple Moving Average
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

/// @brief Determine buy/sell signal based on current price vs SMA
/// @param currentPrice the current stock price
/// @param smaValue the calculated SMA value
/// @param threshold percentage threshold for signal generation (default 0.0 for any crossover)
/// @return 1 for BUY, -1 for SELL, 0 for HOLD
int getSMASignal(const double currentPrice, const double smaValue, const double threshold)
{
    if (smaValue == 0.0) {
        return 0; // Invalid SMA, hold
    }

    if (const double percentageDiff = (currentPrice - smaValue) / smaValue * 100.0; percentageDiff > threshold) {
        return 1; // BUY signal - price above SMA
    }
    else {
        if (percentageDiff < -threshold) {
            return -1; // SELL signal - price below SMA
        }
        return 0; // HOLD - price near SMA within threshold
    }
}

/// @brief Print SMA analysis
/// @param currentPrice current stock price
/// @param smaValue calculated SMA
/// @param signal the signal value
void printSMAAnalysis(const double currentPrice, const double smaValue, const int signal)
{
    std::cout << "=== SMA Analysis ===" << std::endl;
    std::cout << "Current Price: $" << currentPrice << std::endl;
    std::cout << "SMA Value: $" << smaValue << std::endl;
    std::cout << "Price vs SMA: " << ((currentPrice - smaValue) / smaValue * 100.0) << "%" << std::endl;

    switch (signal) {
    case 1:
        std::cout << "Signal: BUY (Price above SMA)" << std::endl;
        break;
    case -1:
        std::cout << "Signal: SELL (Price below SMA)" << std::endl;
        break;
    case 0:
        std::cout << "Signal: HOLD (Price near SMA)" << std::endl;
        break;
    default: ;
    }
    std::cout << "==================\n" << std::endl;
}