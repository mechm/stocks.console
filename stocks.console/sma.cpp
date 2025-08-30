#include <string>
#include <iostream>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "../stocks.console.indicator/sma.h"
#include "date_validation.h"
#include "include/sma.h"

void ShowSMA(Alpacha& alpacha, const std::string& symbol)
{
    // Validate date and period
    time_t validatedTime = GetValidDateOrEmpty();
    const int period = GetValidPeriod();

    // If no date is set, get the trading date N days ago
    if (validatedTime == 0) {
        validatedTime = alpacha.GetTradingDateNDaysAgo(period);
    }

    // Retrieve historical prices
    const HistoricalClosedPrices closingPrices =
        alpacha.GetHistoricalClosedPrices(symbol, "1D", validatedTime);

    // Error handling for data retrieval
    if (!closingPrices.success) {
        std::cout << "Failed to retrieve historical data for " << symbol;
        if (!closingPrices.error_message.empty()) {
            std::cout << ": " << closingPrices.error_message;
        }
        std::cout << std::endl;
        return;
    }
    if (closingPrices.prices.empty()) {
        std::cout << "No bar data found for " << symbol << std::endl;
        return;
    }
    if (closingPrices.prices.size() < period) {
        std::cout << "Not enough historical data for SMA calculation. Need at least "
            << period << " data points." << std::endl;
        return;
    }

    // Calculate SMA
    const double sma = calculateSMA(closingPrices.prices, period);
    std::cout << "\nSMA (" << period << "-period) for " << symbol << ": "
        << std::fixed << std::setprecision(2) << sma << "\n" << std::endl;

    // Analyze SMA and get signal
    const double currentPrice = closingPrices.prices.back();
    constexpr double threshold = 1.0;
    const int signal = getSMASignal(currentPrice, sma, threshold);

    printSMAAnalysis(currentPrice, sma, signal);
}

static int GetValidPeriod() {
    int period;

    while (true) {
        std::cout << "Enter SMA period (number of days, e.g. 3, 8, 10, 20, 50, 200): ";
        std::cin >> period;

        if (std::cin.fail()) {
            std::cout << "Please enter a valid number." << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (period <= 0) {
            std::cout << "Period must be greater than 0." << std::endl;
            continue;
        }

        if (period > 200) {
            std::cout << "Period seems too large (max 200). Please enter a reasonable value." << std::endl;
            continue;
        }

        return period; // Valid period found
    }
}
