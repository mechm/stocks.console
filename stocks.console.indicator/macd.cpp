#include <iostream>
#include <vector>
#include <algorithm>

#include "pch.h"

//This implementation provides :
//
//calculateEMA() - Helper function to calculate Exponential Moving Average
//calculateMACD() - Main MACD calculation(Fast EMA - Slow EMA)
//calculateMACDSignal() - Signal line calculation(EMA of MACD values)
//calculateMACDHistogram() - Histogram calculation(MACD - Signal)
//The MACD indicator consists of three components :
//
//MACD Line : Difference between 12 - period and 26 - period EMAs
//Signal Line : 9 - period EMA of the MACD line
//Histogram : Difference between MACD line and signal line
//This follows the same coding style and structure as your existing SMA functions in the codebase.



/// @brief Calculate Exponential Moving Average
/// @param prices vector of price data
/// @param period the period for EMA calculation
/// @return exponential moving average
double calculateEMA(const std::vector<double>& prices, int period) {
    if (prices.size() < period) {
        return 0.0;
    }

    double multiplier = 2.0 / (period + 1);
    double ema = prices[0]; // Start with first price

    for (size_t i = 1; i < prices.size(); i++) {
        ema = (prices[i] * multiplier) + (ema * (1 - multiplier));
    }

    return ema;
}

/// @brief Calculate Moving Average Convergence Divergence (MACD)
/// @param prices vector of closing prices
/// @param fastPeriod fast EMA period (typically 12)
/// @param slowPeriod slow EMA period (typically 26)
/// @param signalPeriod signal line EMA period (typically 9)
/// @return MACD line value
double calculateMACD(const std::vector<double>& prices, int fastPeriod = 12, int slowPeriod = 26, int signalPeriod = 9) {
    if (prices.size() < slowPeriod) {
        return 0.0;
    }

    // Calculate fast EMA (12-period)
    double fastEMA = calculateEMA(prices, fastPeriod);

    // Calculate slow EMA (26-period)
    double slowEMA = calculateEMA(prices, slowPeriod);

    // MACD line = Fast EMA - Slow EMA
    double macdLine = fastEMA - slowEMA;

    return macdLine;
}

/// @brief Calculate MACD Signal Line
/// @param macdValues vector of MACD line values
/// @param signalPeriod signal line EMA period (typically 9)
/// @return signal line value
double calculateMACDSignal(const std::vector<double>& macdValues, int signalPeriod = 9) {
    return calculateEMA(macdValues, signalPeriod);
}

/// @brief Calculate MACD Histogram
/// @param macdLine MACD line value
/// @param signalLine signal line value
/// @return histogram value (MACD - Signal)
double calculateMACDHistogram(double macdLine, double signalLine) {
    return macdLine - signalLine;
}