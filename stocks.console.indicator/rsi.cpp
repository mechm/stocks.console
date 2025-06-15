#include <iostream>
#include <vector>
#include <cmath>

#include "pch.h"

/// @brief Calculate RSI (Relative Strength Index) for a given period
/// Basic RSI calculation using simple moving averages
/// Overbought / Oversold : Values above 70 typically indicate overbought conditions with potential pullback.
/// Below 30 indicates oversold, indicating potential price comeback.
/// 
/// @param prices vector of price data
/// @param period the period for RSI calculation (typically 14)
/// @return RSI value between 0 and 100
double calculateRSI(const std::vector<double>& prices, const int period)
{
    if (prices.size() < static_cast<unsigned long long>(period) + 1) {
        return 0.0; // Not enough data
    }

    std::vector<double> gains;
    std::vector<double> losses;

    // Calculate price changes (gains and losses)
    for (int i = 1; i <= period; i++) {
        double change = prices[i] - prices[i - 1];
        if (change > 0) {
            gains.push_back(change);
            losses.push_back(0.0);
        }
        else {
            gains.push_back(0.0);
            losses.push_back(std::abs(change));
        }
    }

    // Calculate average gain and average loss
    double avgGain = 0.0;
    double avgLoss = 0.0;

    for (double gain : gains) {
        avgGain += gain;
    }
    avgGain /= period;

    for (double loss : losses) {
        avgLoss += loss;
    }
    avgLoss /= period;

    // Avoid division by zero
    if (avgLoss == 0.0) {
        return 100.0;
    }

    // Calculate RS (Relative Strength)
    double rs = avgGain / avgLoss;

    // Calculate RSI
    double rsi = 100.0 - (100.0 / (1.0 + rs));

    return rsi;
}

/// @brief Calculate RSI using Wilder's smoothing method (considered - more accurate)
/// @param prices vector of price data
/// @param period the period for RSI calculation (typically 14)
/// @return RSI value between 0 and 100
double calculateRSIWilder(const std::vector<double>& prices, const int period)
{
    if (prices.size() < static_cast<unsigned long long>(period) + 1) {
        return 0.0; // Not enough data
    }

    double avgGain = 0.0;
    double avgLoss = 0.0;

    // Calculate initial average gain and loss for the first period
    for (int i = 1; i <= period; i++) {
        double change = prices[i] - prices[i - 1];
        if (change > 0) {
            avgGain += change;
        }
        else {
            avgLoss += std::abs(change);
        }
    }

    avgGain /= period;
    avgLoss /= period;

    // Apply Wilder's smoothing for subsequent periods
    for (int i = period + 1; i < prices.size(); i++) {
        double change = prices[i] - prices[i - 1];
        double gain = (change > 0) ? change : 0.0;
        double loss = (change < 0) ? std::abs(change) : 0.0;

        avgGain = ((avgGain * (period - 1)) + gain) / period;
        avgLoss = ((avgLoss * (period - 1)) + loss) / period;
    }

    // Avoid division by zero
    if (avgLoss == 0.0) {
        return 100.0;
    }

    // Calculate RS and RSI
    double rs = avgGain / avgLoss;
    double rsi = 100.0 - (100.0 / (1.0 + rs));

    return rsi;
}