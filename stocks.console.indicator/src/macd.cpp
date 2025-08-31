#include <iostream>
#include <vector>
#include <algorithm>

#include "../pch.h"

/// @brief Prints help information about the Exponential Moving Average (EMA) indicator.
void printEMAHelp()
{
    std::cout << R"(
====================[ EMA Help ]====================
EMA (Exponential Moving Average) is a type of moving average that places a
greater weight and significance on the most recent data points.

- EMA reacts more quickly to recent price changes than a simple moving average (SMA).
- Common periods for EMA are 12 and 26 days.
- EMA is widely used in trend-following strategies and as a component of other indicators (e.g., MACD).

Usage:
- Use EMA to identify trend direction and potential support/resistance levels.
- Compare short-term and long-term EMAs to generate trading signals.

====================================================
)" << std::endl;
}

/// @brief Prints help information about the Moving Average Convergence Divergence (MACD) indicator.
void printMACDHelp()
{
    std::cout << R"(
====================[ MACD Help ]====================
MACD (Moving Average Convergence Divergence) is a trend-following momentum
indicator that shows the relationship between two EMAs of a security's price.

- MACD Line: Difference between the 12-period EMA and the 26-period EMA.
- Signal Line: 9-period EMA of the MACD Line.
- Histogram: Difference between the MACD Line and the Signal Line.

Interpretation:
- When the MACD Line crosses above the Signal Line, it may indicate a BUY signal.
- When the MACD Line crosses below the Signal Line, it may indicate a SELL signal.
- The histogram visualizes the distance between the MACD and Signal lines.

Usage:
- Use MACD to identify potential buy/sell opportunities and trend reversals.
- Combine MACD with other indicators for more robust analysis.

====================================================
)" << std::endl;
}

/// @brief Calculate Exponential Moving Average
/// @param prices vector of price data
/// @param period the period for EMA calculation
/// @return exponential moving average
double calculateEMA(const std::vector<double>& prices, const int period) {
    if (prices.empty() || prices.size() < period || period <= 0) {
        return 0.0;
    }

    const double multiplier = 2.0 / (period + 1);
    double ema = prices[0]; // Start with first price

    for (size_t i = 1; i < prices.size(); i++) {
        ema = (prices[i] * multiplier) + (ema * (1 - multiplier));       
        
        #ifdef DEBUG   
            std::cout << "Day " << i + 1 << ": "" Close Price: " << prices[i] << " EMA: " << ema << std::endl;	
        #endif
    }

   return ema;
}

/// @brief Calculate Moving Average Convergence Divergence (MACD)
/// @param prices vector of closing prices
/// @param fastPeriod fast EMA period (typically 12)
/// @param slowPeriod slow EMA period (typically 26)
/// @param signalPeriod signal line EMA period (typically 9)
/// @return MACD line value
double calculateMACD(const std::vector<double>& prices, const int fastPeriod, const int slowPeriod, int signalPeriod) {
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
double calculateMACDSignal(const std::vector<double>& macdValues, const int signalPeriod) {
    return calculateEMA(macdValues, signalPeriod);
}

/// @brief Calculate MACD Histogram
/// @param macdLine MACD line value
/// @param signalLine signal line value
/// @return histogram value (MACD - Signal)
double calculateMACDHistogram(const double macdLine, const double signalLine) {
    return macdLine - signalLine;
}