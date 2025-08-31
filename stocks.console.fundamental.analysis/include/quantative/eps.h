#pragma once

#include <vector>
#include <string>

// Enum for signal types for type-safe code
enum class EPS_Signal {
    Buy,
    Sell,
    Hold
};

// Function to calculate Earnings Per Share (EPS)
double calculateEPS(double netIncome, long long sharesOutstanding);

// Function to generate signals based on EPS trend
std::vector<EPS_Signal> getEPSSignals(const std::vector<double>& epsValues);

// Helper function to convert the enum to a string for printing
std::string epsSignalToString(EPS_Signal signal);