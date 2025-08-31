#include "../../include/quantative/eps.h"

#include <vector>
#include <string>
#include <map>
#include <stdexcept>

/**
 * @brief Calculates the Earnings Per Share (EPS).
 *
 * @param netIncome The company's total net income.
 * @param sharesOutstanding The weighted average number of shares outstanding.
 * @return The calculated EPS.
 * @throws std::invalid_argument if sharesOutstanding is zero or negative.
 */
double calculateEPS(const double netIncome, const long long sharesOutstanding) {
    if (sharesOutstanding <= 0) {
        throw std::invalid_argument("Shares outstanding must be a positive number.");
    }
    return netIncome / sharesOutstanding;
}

std::vector<EPS_Signal> getEPSSignals(const std::vector<double>& epsValues) {
    if (epsValues.size() < 2) {
        throw std::invalid_argument("EPS values vector must contain at least two data points for a trend analysis.");
    }

    std::vector<EPS_Signal> signals;
    signals.reserve(epsValues.size());
    signals.push_back(EPS_Signal::Hold); // The first point has no previous data for comparison

    for (size_t i = 1; i < epsValues.size(); ++i) {
        if (epsValues[i] > epsValues[i-1]) {
            signals.push_back(EPS_Signal::Buy);
        } else if (epsValues[i] < epsValues[i-1]) {
            signals.push_back(EPS_Signal::Sell);
        } else {
            signals.push_back(EPS_Signal::Hold);
        }
    }

    return signals;
}

std::string epsSignalToString(const EPS_Signal signal) {
    static const std::map<EPS_Signal, std::string> signal_map = {
        {EPS_Signal::Buy, "Buy"},
        {EPS_Signal::Sell, "Sell"},
        {EPS_Signal::Hold, "Hold"}
    };
    return signal_map.at(signal);
}