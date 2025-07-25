﻿#include <iostream>
#include <vector>
#include <cmath>

#include "pch.h"
#include <iomanip>

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
        double change = prices[i] - prices[static_cast<std::vector<double, std::allocator<double>>::size_type>(i) - 1];
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

    for (const double gain : gains) {
        avgGain += gain;
    }
    avgGain /= period;

    for (const double loss : losses) {
        avgLoss += loss;
    }
    avgLoss /= period;

    // Avoid division by zero
    if (avgLoss == 0.0) {
        return 100.0;
    }

    // Calculate RS (Relative Strength)
    const double rs = avgGain / avgLoss;

    // Calculate RSI
    const double rsi = 100.0 - (100.0 / (1.0 + rs));

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
        const double change = prices[i] - prices[static_cast<std::vector<double, std::allocator<double>>::size_type>(i) - 1];
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
        const double change = prices[i] - prices[static_cast<std::vector<double, std::allocator<double>>::size_type>(i) - 1];
        const double gain = (change > 0) ? change : 0.0;
        const double loss = (change < 0) ? std::abs(change) : 0.0;

        avgGain = (avgGain * (period - 1) + gain) / period;
        avgLoss = (avgLoss * (period - 1) + loss) / period;
    }

    // Avoid division by zero
    if (avgLoss == 0.0) {
        return 100.0;
    }

    // Calculate RS and RSI
    const double rs = avgGain / avgLoss;
    const double rsi = 100.0 - (100.0 / (1.0 + rs));

    return rsi;
}

static int getRSISignal(double rsi, double oversoldThreshold, double overboughtThreshold) {
    if (rsi <= oversoldThreshold) {
        return 1;  // Buy signal (oversold)
    }
    else if (rsi >= overboughtThreshold) {
        return -1; // Sell signal (overbought)
    }
    else {
        return 0;  // Hold signal (neutral)
    }
}

static void printRSIAnalysis(double currentPrice, double rsi, int signal) {
    std::cout << "\n=== RSI Analysis ===" << std::endl;
    std::cout << "Current Price: $" << std::fixed << std::setprecision(2) << currentPrice << std::endl;
    std::cout << "RSI: " << std::fixed << std::setprecision(2) << rsi << std::endl;

    std::cout << "Market Condition: ";
    if (rsi <= 30) {
        std::cout << "OVERSOLD (RSI <= 30)" << std::endl;
    }
    else if (rsi >= 70) {
        std::cout << "OVERBOUGHT (RSI >= 70)" << std::endl;
    }
    else {
        std::cout << "NEUTRAL (30 < RSI < 70)" << std::endl;
    }

    std::cout << "Signal: ";
    switch (signal) {
    case 1:
        std::cout << "BUY - Stock may be oversold, potential upward movement" << std::endl;
        break;
    case -1:
        std::cout << "SELL - Stock may be overbought, potential downward movement" << std::endl;
        break;
    case 0:
        std::cout << "HOLD - No clear signal, monitor for changes" << std::endl;
        break;
    }
    std::cout << "==================\n" << std::endl;
}