#pragma once

#include <vector>

// Structure to hold daily price data
struct PriceData {
    double high;
    double low;
    double close;
};

// Enum for signal types for type-safe code
enum class Signal {
    Buy,
    Sell,
    Hold
};

std::vector<double> calculateStochasticK(const std::vector<PriceData>& data, int period);
std::vector<double> calculateStochasticD(const std::vector<double>& k_values, int d_period);
std::vector<Signal> getStochasticSignals(const std::vector<double>& k_values, const std::vector<double>& d_values);