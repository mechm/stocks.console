#include <vector>

#include "pch.h"

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

    long currentOBV = 0;
    obv.push_back(currentOBV); // First value is zero

    for (size_t i = 1; i < closingPrices.size(); ++i) {
        if (closingPrices[i] > closingPrices[i - 1]) {
            currentOBV += volumes[i];
        }
        else if (closingPrices[i] < closingPrices[i - 1]) {
            currentOBV -= volumes[i];
        }
        // If prices are equal, OBV stays the same
        obv.push_back(currentOBV);
    }
    return obv;
}