#include "../../stocks.console.indicator/include/stochastic_oscillator.h"
#include "../../include/base_test.h"

#include "gtest/gtest.h"

#include <vector>

TEST(Stochastic, AppleStochastic_Hold_Signals){

    std::cout << "Running test for getStochasticSignals with Apple data..." << std::endl;

    // AAPL data (High, Low, Close) with dates
    const std::vector<PriceData> aapl_data = {
        {188.00, 185.50, 186.00}, // Day 1: Jan 12, 2024
        {189.00, 186.50, 187.00}, // Day 2: Jan 16, 2024
        {190.50, 188.00, 189.50}, // Day 3: Jan 17, 2024
        {192.00, 189.50, 191.00}, // Day 4: Jan 18, 2024
        {192.50, 190.00, 192.00}, // Day 5: Jan 19, 2024
        {193.00, 190.50, 191.00}, // Day 6: Jan 22, 2024
        {192.50, 189.50, 191.50}, // Day 7: Jan 23, 2024
        {190.80, 188.00, 189.00}, // Day 8: Jan 24, 2024
        {188.50, 186.00, 187.00}, // Day 9: Jan 25, 2024
        {187.50, 185.00, 186.00}, // Day 10: Jan 26, 2024
        {188.00, 185.50, 187.50}, // Day 11: Jan 29, 2024
        {189.00, 186.00, 188.00}, // Day 12: Jan 30, 2024
        {190.00, 187.50, 189.00}, // Day 13: Jan 31, 2024
        {191.00, 188.50, 190.00}, // Day 14: Feb 1, 2024
        {192.00, 189.00, 191.00}  // Day 15: Feb 2, 2024
    };

    constexpr int period_k = 10;
    constexpr int period_d = 3;

    // --- Step 1: Calculate the %K values ---
    const std::vector<double> k_values = calculateStochasticK(aapl_data, period_k);
    const std::vector<double> expected_k = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 12.50, 31.25, 37.5, 50.00, 62.50, 75.00};
    for(size_t i = 0; i < k_values.size(); ++i) {
        assert(areAlmostEqual(k_values[i], expected_k[i]));
    }

    // --- Step 2: Calculate the %D values ---
    const std::vector<double> d_values = calculateStochasticD(k_values, period_d);
    const std::vector<double> expected_d = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 4.17, 14.58, 27.08, 39.58, 50.00, 62.50};
    for(size_t i = 0; i < d_values.size(); ++i) {
        assert(areAlmostEqual(d_values[i], expected_d[i]));
    }

    // --- Step 3: Generate the signals and assert ---
    const std::vector<Signal> signals = getStochasticSignals(k_values, d_values);
    const std::vector<Signal> expected_signals = {
        Signal::Hold,     // Day 1: Jan 12, 2024
        Signal::Hold,     // Day 2: Jan 16, 2024
        Signal::Hold,     // Day 3: Jan 17, 2024
        Signal::Hold,     // Day 4: Jan 18, 2024
        Signal::Hold,     // Day 5: Jan 19, 2024
        Signal::Hold,     // Day 6: Jan 22, 2024
        Signal::Hold,     // Day 7: Jan 23, 2024
        Signal::Hold,     // Day 8: Jan 24, 2024
        Signal::Hold,     // Day 9: Jan 25, 2024
        Signal::Hold,     // Day 10: Jan 26, 2024
        Signal::Hold,      // Day 11: Jan 29, 2024
        Signal::Hold,      // Day 12: Jan 30, 2024
        Signal::Hold,      // Day 13: Jan 31, 2024
        Signal::Hold,      // Day 14: Feb 1, 2024
        Signal::Hold       // Day 15: Feb 2, 2024
    };

    assert(signals.size() == expected_signals.size());
    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i] != expected_signals[i]) {
            std::cerr << "Test failed at index " << i << ". Expected: "
                      << signalToString(expected_signals[i])
                      << ", but got: "
                      << signalToString(signals[i]) << std::endl;
            assert(false);
        }
    }

    std::cout << "All tests passed! 🎉" << std::endl;
}

// The main test function
TEST(Stochastic, AppleStochastic_Buy_Sell_Signals){
    std::cout << "Running test for a Stochastic buy signal using real AAPL data..." << std::endl;

    // Real AAPL data from Nov 28 to Dec 19, 2023 (15 trading days)
    const std::vector<PriceData> aapl_data = {
        {191.00, 188.75, 189.50}, // Day 1: Nov 28
        {190.90, 189.00, 190.30}, // Day 2: Nov 29
        {190.30, 188.00, 189.90}, // Day 3: Nov 30
        {190.50, 188.20, 190.50}, // Day 4: Dec 01
        {190.00, 189.20, 189.50}, // Day 5: Dec 04
        {191.00, 188.50, 189.80}, // Day 6: Dec 05
        {193.80, 190.00, 192.50}, // Day 7: Dec 06
        {193.30, 190.80, 192.50}, // Day 8: Dec 07
        {195.00, 192.00, 193.00}, // Day 9: Dec 08
        {193.30, 190.72, 191.49}, // Day 10: Dec 11
        {194.75, 191.70, 193.00}, // Day 11: Dec 12
        {196.00, 192.85, 194.20}, // Day 12: Dec 13
        {195.40, 191.00, 191.49}, // Day 13: Dec 14
        {194.50, 191.00, 191.50}, // Day 14: Dec 15
        {194.00, 191.10, 193.00}  // Day 15: Dec 18
    };

    constexpr int period_k = 10;
    constexpr int period_d = 3;

    // --- Step 1: Calculate the %K values ---
    const std::vector<double> k_values = calculateStochasticK(aapl_data, period_k);
    const std::vector<double> expected_k = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 49.86, 71.43, 77.50, 42.18, 40.00, 60.00};
    for(size_t i = 0; i < k_values.size(); ++i) {
        assert(areAlmostEqual(k_values[i], expected_k[i]));
    }

    // --- Step 2: Calculate the %D values ---
    const std::vector<double> d_values = calculateStochasticD(k_values, period_d);
    const std::vector<double> expected_d = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 16.62, 40.43, 66.26, 63.70, 53.23, 47.39};
    for(size_t i = 0; i < d_values.size(); ++i) {
        assert(areAlmostEqual(d_values[i], expected_d[i]));
    }

    // --- Step 3: Generate the signals and assert ---
    const std::vector<Signal> signals = getStochasticSignals(k_values, d_values);
    const std::vector<Signal> expected_signals = {
        Signal::Hold,     // Day 1: Nov 28
        Signal::Hold,     // Day 2: Nov 29
        Signal::Hold,     // Day 3: Nov 30
        Signal::Hold,     // Day 4: Dec 01
        Signal::Hold,     // Day 5: Dec 04
        Signal::Hold,     // Day 6: Dec 05
        Signal::Hold,     // Day 7: Dec 06
        Signal::Hold,     // Day 8: Dec 07
        Signal::Hold,     // Day 9: Dec 08
        Signal::Hold,     // Day 10: Dec 11 (No crossover from Day 9)
        Signal::Hold,     // Day 11: Dec 12 (No crossover from Day 10)
        Signal::Hold,     // Day 12: Dec 13 (No crossover)
        Signal::Sell,     // Day 13: Dec 14 (%K crosses below %D)
        Signal::Hold,     // Day 14: Dec 15 (No crossover)
        Signal::Buy       // Day 15: Dec 18 (%K crosses above %D)
    };
    assert(signals.size() == expected_signals.size());
    for (size_t i = 0; i < signals.size(); ++i) {
        if (signals[i] != expected_signals[i]) {
            std::cerr << "Test failed at index " << i << ". Expected: "
                      << signalToString(expected_signals[i])
                      << ", but got: "
                      << signalToString(signals[i]) << std::endl;
            assert(false);
        }
    }

    std::cout << "All tests passed! 🎉" << std::endl;
}
