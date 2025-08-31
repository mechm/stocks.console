#include "../../stocks.console.fundamental.analysis/include/quantative/eps.h"

#include "gtest/gtest.h"

#include <vector>

TEST(EPS, CalculateEPS_Apple){

    // Arrange
    // Net income for Apple for the quarter ending June 30, 2025 (in billions of USD)
    constexpr double apple_net_income = 23.434;

    // Diluted weighted average shares outstanding for the same quarter (in billions)
    constexpr double apple_shares_outstanding_in_billions = 14.948;

    // We need to convert this to a long long for the function input
    constexpr long long apple_shares_outstanding = static_cast<long long>(apple_shares_outstanding_in_billions * 1e9);

    // Expected EPS for the quarter ending June 30, 2025
    constexpr double expected_eps = 1.57;

    // Act
    // Calculate the EPS using the function under test
    const double calculated_eps = calculateEPS(apple_net_income * 1e9, apple_shares_outstanding);

    // Assert
    // Use a custom function to handle floating-point comparison
    ASSERT_NEAR(calculated_eps, expected_eps, 0.003);
}

TEST(EPS, CalculateEPS_Apple_Signals){

    // Quarterly diluted EPS values for Apple (AAPL) from Q1 2024 to Q3 2025
    const std::vector<double> apple_eps_values = {
        2.40, // FY Q1 2024
        1.53, // FY Q2 2024
        1.40, // FY Q3 2024
        0.97, // FY Q4 2024
        2.40, // FY Q1 2025
        1.65, // FY Q2 2025
        1.57  // FY Q3 2025
    };

    // Expected signals based on quarter-over-quarter EPS changes
    const std::vector<EPS_Signal> expected_signals = {
        EPS_Signal::Hold, // Q1 2024 (initial)
        EPS_Signal::Sell, // Q2 2024 (EPS down from 2.40)
        EPS_Signal::Sell, // Q3 2024 (EPS down from 1.53)
        EPS_Signal::Sell, // Q4 2024 (EPS down from 1.40)
        EPS_Signal::Buy,  // Q1 2025 (EPS up from 0.97)
        EPS_Signal::Sell, // Q2 2025 (EPS down from 2.40)
        EPS_Signal::Sell  // Q3 2025 (EPS down from 1.65)
    };

    try {
        const std::vector<EPS_Signal> generated_signals = getEPSSignals(apple_eps_values);

        // Assert that the size of the result vector is correct
        assert(generated_signals.size() == expected_signals.size());

        // Assert that each generated signal matches the expected signal
        for (size_t i = 0; i < generated_signals.size(); ++i) {
            if (generated_signals[i] != expected_signals[i]) {
                std::cerr << "Test failed at index " << i << ". Expected: "
                          << epsSignalToString(expected_signals[i])
                          << ", but got: "
                          << epsSignalToString(generated_signals[i]) << std::endl;
                assert(false); // Fail the test
            }
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error during test setup: " << e.what() << std::endl;
        assert(false);
    }
}



