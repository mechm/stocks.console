#include "stochastic_oscillator.h"

#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

void printStochasticKHelp() {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "%K Line: The Main Stochastic Indicator" << std::endl;
    std::cout << "The %K line compares the current closing price to the " << std::endl;
    std::cout << "high-low range over a given period. It is a momentum " << std::endl;
    std::cout << "oscillator that shows if the price is closing near its " << std::endl;
    std::cout << "highs (strong momentum) or lows (weak momentum)." << std::endl;
    std::cout << "Formula: ((Close - Lowest Low) / (Highest High - Lowest Low)) * 100" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}

void printStochasticDHelp() {
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "%D Line: The Stochastic Signal Line" << std::endl;
    std::cout << "The %D line is a simple moving average (SMA) of the %K line. " << std::endl;
    std::cout << "It is used to smooth out the %K line and provide a clearer " << std::endl;
    std::cout << "trend. Crossovers between the %K and %D lines are often used " << std::endl;
    std::cout << "to generate buy and sell signals." << std::endl;
    std::cout << "Formula: 3-period SMA of the %K line" << std::endl;
    std::cout << "-----------------------------------" << std::endl;
}

// Function to calculate the %K line
std::vector<double> calculateStochasticK(const std::vector<PriceData>& data, int period) {
    if (data.empty() || period <= 0) {
        return {};
    }
    std::vector<double> k_values;
    k_values.reserve(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        if (i < period - 1) {
            k_values.push_back(0.0);
            continue;
        }
        double highest_high = 0.0;
        double lowest_low = std::numeric_limits<double>::max();
        for (int j = 0; j < period; ++j) {
            highest_high = std::max(highest_high, data[i - j].high);
            lowest_low = std::min(lowest_low, data[i - j].low);
        }
        if (highest_high == lowest_low) {
            k_values.push_back(0.0);
        } else {
            double k = (data[i].close - lowest_low) / (highest_high - lowest_low) * 100.0;
            k_values.push_back(k);
        }
    }
    return k_values;
}

// Function to calculate the %D line (3-period SMA of %K)
std::vector<double> calculateStochasticD(const std::vector<double>& k_values, int d_period) {
    if (k_values.empty() || d_period <= 0) {
        return {};
    }
    std::vector<double> d_values;
    d_values.reserve(k_values.size());
    for (int i = 0; i < d_period - 1; ++i) {
        d_values.push_back(0.0);
    }
    for (size_t i = d_period - 1; i < k_values.size(); ++i) {
        double sum = 0.0;
        for (int j = 0; j < d_period; ++j) {
            sum += k_values[i - j];
        }
        d_values.push_back(sum / d_period);
    }
    return d_values;
}

std::vector<Signal> getStochasticSignals(const std::vector<double>& k_values, const std::vector<double>& d_values) {
    if (k_values.size() != d_values.size() || k_values.size() < 2) {
        return {};
    }
    std::vector<Signal> signals;
    signals.reserve(k_values.size());
    signals.push_back(Signal::Hold);
    for (size_t i = 1; i < k_values.size(); ++i) {
        if (k_values[i-1] < d_values[i-1] && k_values[i] > d_values[i]) {
            signals.push_back(Signal::Buy);
        } else if (k_values[i-1] > d_values[i-1] && k_values[i] < d_values[i]) {
            signals.push_back(Signal::Sell);
        } else {
            signals.push_back(Signal::Hold);
        }
    }
    return signals;
}
//
// std::string signalToString(const Signal signal) {
//     static const std::map<Signal, std::string> signal_map = {
//         {Signal::Buy, "Buy"},
//         {Signal::Sell, "Sell"},
//         {Signal::Hold, "Hold"}
//     };
//     return signal_map.at(signal);
// }