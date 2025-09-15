#include "indicator.h"

#include "../include/base_test.h"

#include <string>
#include <map>

std::string signalToString(const Signal signal) {
    static const std::map<Signal, std::string> signal_map = {
        {Signal::Buy, "Buy"},
        {Signal::Sell, "Sell"},
        {Signal::Hold, "Hold"}
    };
    return signal_map.at(signal);
}

// Helper function to compare floating-point numbers
bool areAlmostEqual(const double a, const double b, const double epsilon) {
    return std::abs(a - b) < epsilon;
}