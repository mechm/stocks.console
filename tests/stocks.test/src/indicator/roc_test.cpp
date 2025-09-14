#include "../../stocks.console.indicator/include/momentum/roc.h"

#include "gtest/gtest.h"

#include <vector>
#include <iostream>
#include <numeric>
#include <iomanip>
#include <cassert>
#include <string>

// Helper function to compare floating-point numbers
bool areAlmostEqual2(double a, double b, double epsilon = 0.01) {
    return std::abs(a - b) < epsilon;
}

std::string signalToString2(const Signal signal) {
    static const std::map<Signal, std::string> signal_map = {
        {Signal::Buy, "Buy"},
        {Signal::Sell, "Sell"},
        {Signal::Hold, "Hold"}
    };
    return signal_map.at(signal);
}


TEST(ROC, CalculateApple){

   std::cout << "--- Running test for ROC with Apple stock prices ---" << std::endl;

    // AAPL closing prices (4-day lookback period)
    const std::vector<double> apple_prices = {
        200.00, // Day 1: Aug 11, 2025
        198.50, // Day 2: Aug 12, 2025
        195.00, // Day 3: Aug 13, 2025
        196.50, // Day 4: Aug 14, 2025
        197.00, // Day 5: Aug 15, 2025
        201.00, // Day 6: Aug 18, 2025
        203.50, // Day 7: Aug 19, 2025
        201.50, // Day 8: Aug 20, 2025
        199.00, // Day 9: Aug 21, 2025
        195.00, // Day 10: Aug 22, 2025
        196.50  // Day 11: Aug 25, 2025
    };

    // Expected ROC values (re-calculated and corrected)
    const std::vector<double> expected_roc = {
        0.00, 0.00, 0.00, 0.00,
        -1.50, // Day 5: ((197.00 - 200.00) / 200.00) * 100
        1.26,  // Day 6: ((201.00 - 198.50) / 198.50) * 100
        4.36,  // Day 7: ((203.50 - 195.00) / 195.00) * 100
        2.54,  // Day 8: ((201.50 - 196.50) / 196.50) * 100
        1.01,  // Day 9: ((199.00 - 197.00) / 197.00) * 100
        -2.99, // Day 10: ((195.00 - 201.00) / 201.00) * 100
        -3.44  // Day 11: ((196.50 - 203.50) / 203.50) * 100
    };

    // Expected signals
    const std::vector<Signal> expected_signals = {
        Signal::Hold, Signal::Hold, Signal::Hold, Signal::Hold,
        Signal::Sell,  // Day 5: ROC < 0, previous ROC is 0.0, so this is a sell signal
        Signal::Buy,   // Day 6: ROC > 0, previous ROC < 0
        Signal::Hold,  // Day 7: No crossover
        Signal::Hold,  // Day 8: No crossover
        Signal::Hold,  // Day 9: No crossover
        Signal::Sell,  // Day 10: ROC < 0, previous ROC > 0
        Signal::Hold   // Day 11: No crossover
    };

    constexpr int period = 4;
    const std::vector<double> roc_results = calculateROC(apple_prices, period);
    const std::vector<Signal> signals = getROCSignals(roc_results);

    // Verify the size of the result vectors
    assert(roc_results.size() == expected_roc.size());
    assert(signals.size() == expected_signals.size());

    // Verify each ROC value
    for (size_t i = 0; i < roc_results.size(); ++i) {
        if (!areAlmostEqual2(roc_results[i], expected_roc[i])) {
            std::cerr << "Test failed for ROC at index " << i << ". Expected: "
                      << expected_roc[i] << ", but got: "
                      << roc_results[i] << std::endl;
            assert(false);
        }
    }

    // Verify each signal
    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i] != expected_signals[i]) {
            std::cerr << "Test failed for Signal at index " << i << ". Expected: "
                      << signalToString2(expected_signals[i]) << ", but got: "
                      << signalToString2(signals[i]) << std::endl;
            assert(false);
        }
    }

    std::cout << "All tests passed! 🎉" << std::endl;
}