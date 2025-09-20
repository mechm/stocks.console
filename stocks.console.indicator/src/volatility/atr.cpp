#include "./volatility/atr.h"

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

/**
 * @brief Prints a help message explaining the Average True Range (ATR) indicator.
 */
void helpATR() {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Average True Range (ATR): The Volatility Indicator" << std::endl;
    std::cout << "The ATR measures how much an asset moves on average over a given period." << std::endl;
    std::cout << "It is a smoothed moving average of the True Range (TR)." << std::endl;
    std::cout << "The True Range for a day is the greatest of the following three values:" << std::endl;
    std::cout << "1. Current High minus the current Low." << std::endl;
    std::cout << "2. The absolute value of the current High minus the previous Close." << std::endl;
    std::cout << "3. The absolute value of the current Low minus the previous Close." << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}

/**
* @brief Calculates the Average True Range (ATR) indicator.
 *
 * @param data A vector of PriceData (high, low, close).
 * @param period The lookback period (N).
 * @return A vector of ATR values.
 */
std::vector<double> calculateATR(const std::vector<PriceData>& data, const int period) {
    if (data.empty() || period <= 0 || period >= data.size()) {
        return {};
    }

    std::vector<double> atr_values;
    atr_values.reserve(data.size());

    // Step 1: Calculate the True Range for each day
    std::vector<double> tr_values;
    tr_values.reserve(data.size());
    tr_values.push_back(data[0].high - data[0].low); // First TR is just High-Low

    for (size_t i = 1; i < data.size(); ++i) {
        const double prev_close = data[i - 1].close;
        double h_l = data[i].high - data[i].low;
        double h_pc = std::abs(data[i].high - prev_close);
        double l_pc = std::abs(data[i].low - prev_close);
        tr_values.push_back(std::max({h_l, h_pc, l_pc}));
    }

    // Step 2: Calculate the smoothed average of the TR values
    // The first ATR is a simple moving average of the first 'period' TR values
    const double sum = std::accumulate(tr_values.begin(), tr_values.begin() + period, 0.0);
    double current_atr = sum / period;

    // Add 0.0 placeholders for the first period-1 days
    for (int i = 0; i < period - 1; ++i) {
        atr_values.push_back(0.0);
    }
    atr_values.push_back(current_atr);

    // Subsequent ATR values use Wilder's smoothing (similar to EMA)
    for (size_t i = period; i < tr_values.size(); ++i) {
        current_atr = (current_atr * (period - 1) + tr_values[i]) / period;
        atr_values.push_back(current_atr);
    }

    return atr_values;
}