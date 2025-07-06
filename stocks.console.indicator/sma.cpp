#include <iostream>
#include <vector>

#include "pch.h"

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
int getSMASignal(double currentPrice, double smaValue, double threshold)
{
    if (smaValue == 0.0) {
        return 0; // Invalid SMA, hold
    }

    double percentageDiff = ((currentPrice - smaValue) / smaValue) * 100.0;

    if (percentageDiff > threshold) {
        return 1; // BUY signal - price above SMA
    }
    else if (percentageDiff < -threshold) {
        return -1; // SELL signal - price below SMA
    }
    else {
        return 0; // HOLD - price near SMA within threshold
    }
}

/// @brief Get SMA signal with price history for trend analysis
/// @param prices vector of historical prices (most recent first)
/// @param period SMA period
/// @param threshold percentage threshold for signal
/// @return 1 for BUY, -1 for SELL, 0 for HOLD
static int getSMASignalWithHistory(const std::vector<double>& prices, const int period, const double threshold)
{
    if (prices.size() < period) {
        return 0; // Not enough data
    }

    double currentSMA = calculateSMA(prices, period);
    double currentPrice = prices[0]; // Assuming most recent price is first

    return getSMASignal(currentPrice, currentSMA, threshold);
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
    }
    std::cout << "===================" << std::endl;
}