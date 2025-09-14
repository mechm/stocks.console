#include "momentum/roc.h"

#include <iostream>
#include <vector>
#include <map>
#include <string>

void printROCHelp() {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Rate of Change (ROC): The Momentum Indicator" << std::endl;
    std::cout << "The ROC measures the percentage change in price between the current" << std::endl;
    std::cout << "price and a price from 'N' periods ago. It's a momentum oscillator that" << std::endl;
    std::cout << "shows the strength and direction of a trend." << std::endl;
    std::cout << "Formula: ((Current Price - Price 'N' Periods Ago) / Price 'N' Periods Ago) * 100" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}

/**
 * @brief Calculates the Rate of Change (ROC) indicator.
 *
 * @param prices A vector of stock prices.
 * @param period The lookback period (N).
 * @return A vector of ROC values. The first 'period' values will be 0.0.
 */
std::vector<double> calculateROC(const std::vector<double>& prices, const int period) {
    if (prices.empty() || period <= 0 || period >= prices.size()) {
        return {};
    }

    std::vector<double> roc_values;
    roc_values.reserve(prices.size());

    // Fill the first 'period' values with 0.0 as there is not enough data
    for (int i = 0; i < period; ++i) {
        roc_values.push_back(0.0);
    }

    // Calculate ROC for the rest of the prices
    for (size_t i = period; i < prices.size(); ++i) {
        const double current_price = prices[i];

        if (const double price_n_periods_ago = prices[i - period]; price_n_periods_ago == 0) {
            roc_values.push_back(0.0);
        } else {
            double roc = (current_price - price_n_periods_ago) / price_n_periods_ago * 100.0;
            roc_values.push_back(roc);
        }
    }
    return roc_values;
}

/**
 * @brief Generates buy/sell signals based on ROC crossovers of the zero line.
 *
 * @param roc_values A vector of ROC values.
 * @return A vector of Signal enums.
 */
std::vector<Signal> getROCSignals(const std::vector<double>& roc_values) {
    if (roc_values.size() < 2) {
        return {};
    }
    std::vector<Signal> signals;
    signals.reserve(roc_values.size());
    signals.push_back(Signal::Hold); // No signal on the first day
    for (size_t i = 1; i < roc_values.size(); ++i) {
        // Buy signal: ROC crosses above zero
        if (roc_values[i-1] <= 0 && roc_values[i] > 0) {
            signals.push_back(Signal::Buy);
        }
        // Sell signal: ROC crosses below zero
        else if (roc_values[i-1] >= 0 && roc_values[i] < 0) {
            signals.push_back(Signal::Sell);
        }
        // Hold if no crossover
        else {
            signals.push_back(Signal::Hold);
        }
    }
    return signals;
}

std::string signalToString(const Signal signal) {
    static const std::map<Signal, std::string> signal_map = {
        {Signal::Buy, "Buy"},
        {Signal::Sell, "Sell"},
        {Signal::Hold, "Hold"}
    };
    return signal_map.at(signal);
}