#pragma once

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