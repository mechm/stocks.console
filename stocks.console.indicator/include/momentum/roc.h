#pragma once

#include <vector>

// Enum for signal types for type-safe code
enum class Signal {
    Buy,
    Sell,
    Hold
};

std::vector<double> calculateROC(const std::vector<double>& prices, int period);
std::vector<Signal> getROCSignals(const std::vector<double>& roc_values);
