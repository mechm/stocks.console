#include <string>
#include <iostream>
#include "date_validation.h"
#include "../stocks.console.api/alpacha.h"
#include "../stocks.console.indicator/sma.h"
#include <iomanip>
#include "sma.h"

void ShowSMA(Alpacha& alpacha, std::string symbol) 
{
    // Validate date and period
    time_t validatedTime = GetValidDateOrEmpty();
    const int period = GetValidPeriod();

    // If no date is set, get the trading date N days ago
    if (validatedTime == 0) {
        validatedTime = alpacha.GetTradingDateNDaysAgo(period);
    }

    // Retrieve historical prices
    const HistoricalBarsResult historicPrices =
        alpacha.GetHistoricalBarsAsObjects(symbol, "1D", validatedTime);

    if (!historicPrices.success || historicPrices.bars.empty()) {
        std::cout << "Failed to retrieve historical data for " << symbol << std::endl;
        return;
    }

    if (historicPrices.bars.size() < period) {
        std::cout << "Not enough historical data for SMA calculation. Need at least "
            << period << " data points." << std::endl;
        return;
    }

    // Extract closing prices
    std::vector<double> closingPrices;
    closingPrices.reserve(historicPrices.bars.size());
    for (const auto& bar : historicPrices.bars) {
        closingPrices.push_back(bar.close);
    }

    // Calculate SMA
    const double sma = calculateSMA(closingPrices, period);
    std::cout << "\nSMA (" << period << "-period) for " << symbol << ": "
        << std::fixed << std::setprecision(2) << sma << "\n" << std::endl;

    // Analyze SMA and get signal
    const double currentPrice = closingPrices.back();
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
