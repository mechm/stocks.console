#include <iostream>
#include <vector>
#include <algorithm>

#include "pch.h"

/// @brief Calculate Exponential Moving Average
/// @param prices vector of price data
/// @param period the period for EMA calculation
/// @return exponential moving average
double calculateEMA(const std::vector<double>& prices, int period) {
    if (prices.size() < period) {
        return 0.0;
    }

    const double multiplier = 2.0 / (period + 1);
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
double calculateMACD(const std::vector<double>& prices, const int fastPeriod = 12, const int slowPeriod = 26, int signalPeriod = 9) {
    if (prices.size() < slowPeriod) {
        return 0.0;
    }

    // Calculate fast EMA (12-period)
    const double fastEMA = calculateEMA(prices, fastPeriod);

    // Calculate slow EMA (26-period)
    const double slowEMA = calculateEMA(prices, slowPeriod);

    // MACD line = Fast EMA - Slow EMA
    const double macdLine = fastEMA - slowEMA;

    return macdLine;
}

/// @brief Calculate MACD Signal Line
/// @param macdValues vector of MACD line values
/// @param signalPeriod signal line EMA period (typically 9)
/// @return signal line value
double calculateMACDSignal(const std::vector<double>& macdValues, const int signalPeriod = 9) {
    return calculateEMA(macdValues, signalPeriod);
}

/// @brief Calculate MACD Histogram
/// @param macdLine MACD line value
/// @param signalLine signal line value
/// @return histogram value (MACD - Signal)
double calculateMACDHistogram(const double macdLine, const double signalLine) {
    return macdLine - signalLine;
}