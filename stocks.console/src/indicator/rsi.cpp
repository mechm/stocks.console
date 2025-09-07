#include <string>
#include <iostream>
#include <iomanip>

#include "../../../stocks.console.api/alpacha.h"
#include "../../../stocks.console.indicator/include/rsi.h"
#include "../../include/indicator/rsi.h"
#include "../../include/validation/date_validation.h"

void ShowRSI(Alpacha& alpacha, const std::string& symbol)
{
    // Validate date and period
    time_t validatedTime = GetValidDateOrEmpty();
    const int period = GetValidPeriod();

    // If no date is set, get the trading date N days ago
    if (validatedTime == 0) {
        validatedTime = alpacha.GetTradingDateNDaysAgo(period + 1);
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
    if (closingPrices.prices.size() < static_cast<size_t>(period + 1)) {
        std::cout << "Not enough historical data for RSI calculation. Need at least "
            << (period + 1) << " data points." << std::endl;
        return;
    }

    // Calculate RSI
    const double rsiValue = calculateRSI(closingPrices.prices, period);
    const int signal = getRSISignal(rsiValue);
    const double currentPrice = closingPrices.prices.back();

    printRSIAnalysis(currentPrice, rsiValue, signal);
}

static int GetValidPeriod() {
    int period;

    while (true) {
        std::cout << "Enter RSI period (number of days, e.g. 14): ";
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

        return period; // Valid period found
    }
}