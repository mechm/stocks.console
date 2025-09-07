#include <vector>

#include "../pch.h"
#include "../include/obv.h"

/// @brief Prints help information about the On-Balance Volume (OBV) indicator.
void printOBVHelp()
{
    std::cout << R"(
    ====================[ OBV Help ]====================
    OBV (On-Balance Volume) is a technical indicator that uses volume flow
    to predict changes in stock price.

    - OBV adds volume on up days and subtracts volume on down days.
    - A rising OBV indicates that volume is increasing on up days, suggesting accumulation.
    - A falling OBV indicates that volume is increasing on down days, suggesting distribution.

    Usage:
    - Use OBV to confirm price trends: rising OBV supports an uptrend, falling OBV supports a downtrend.
    - Look for divergences between OBV and price to spot potential reversals.
    - Combine OBV with other indicators for more reliable trading signals.

    ====================================================
    )" << std::endl;
}

/// @brief Calculate On-Balance Volume (OBV) for a stock
/// @param closingPrices vector of closing prices
/// @param volumes vector of volume data (same size as closingPrices)
/// @return vector of OBV values
std::vector<long long> calculateOBV(const std::vector<double>& closingPrices, const std::vector<long long>& volumes)
{
    std::vector<long long> obv;
    if (closingPrices.empty() || volumes.empty() || closingPrices.size() != volumes.size())
        return obv;

    long long currentOBV = 0;
    obv.push_back(currentOBV); // First value is zero

    for (size_t i = 1; i < closingPrices.size(); ++i) {
        if (closingPrices[i] > closingPrices[i - 1]) {
            // Price went up, add current volume
            currentOBV += volumes[i];
        }
        else if (closingPrices[i] < closingPrices[i - 1]) {
            // Price went down, subtract current volume
            currentOBV -= volumes[i];
        }
        // If prices are equal, OBV stays the same
        obv.push_back(currentOBV);
    }
    return obv;
}

/// @brief Generate buy/sell/hold signals for each day based on OBV and price trends.
/// @param prices Vector of closing prices.
/// @param obv_values Vector of OBV values (same size as prices).
/// @return Vector of signals: 1 for BUY (OBV and price ri
std::vector<int> getOBVSignals(const std::vector<double>& prices, const std::vector<long long>& obv_values) 
{
    if (prices.size() != obv_values.size()) {
        throw std::invalid_argument("Price and OBV vectors must have the same size.");
    }
    if (prices.size() < 2 || obv_values.size() < 2) {
        return {}; // Not enough data
    }

    std::vector<int> signals;
    signals.reserve(prices.size());
    signals.push_back(0); // No signal for the first data point

    for (size_t i = 1; i < prices.size(); ++i) {
        const bool price_up = prices[i] > prices[i - 1];
        const bool price_down = prices[i] < prices[i - 1];
        const bool obv_up = obv_values[i] > obv_values[i - 1];
        const bool obv_down = obv_values[i] < obv_values[i - 1];

        if (price_up && obv_up) {
            signals.push_back(1);
        }
        else if (price_down && obv_down) {
            signals.push_back(-1);
        }
        else {
            signals.push_back(0);
        }
    }

    return signals;
}