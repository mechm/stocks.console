#include "../../stocks.console.indicator/include/macd.h"

#include "gtest/gtest.h"

#include <vector>

TEST(EMA, ReturnsZero_WhenNotEnoughData){

    // Arrange
    const std::vector<double> prices = { 1.0, 2.0, 3.0 };
    constexpr int period = 4;

    // Act
    const double result = calculateEMA(prices, period);

    // Assert
    EXPECT_EQ(0.0, result);
}

TEST(EMA, CalculatesCorrectly){

    // Arrange
    const std::vector<double> prices = { 10, 11, 12, 13, 14, 15 };
    constexpr int period = 3;

    // Manually calculated EMA for this sequence and period
    // Initial EMA = 10, multiplier = 0.5
    // Step 1: (11 * 0.5) + (10 * 0.5) = 10.5
    // Step 2: (12 * 0.5) + (10.5 * 0.5) = 11.25
    // Step 3: (13 * 0.5) + (11.25 * 0.5) = 12.125
    // Step 4: (14 * 0.5) + (12.125 * 0.5) = 13.0625
    // Step 5: (15 * 0.5) + (13.0625 * 0.5) = 14.03125

    // Act
    const double result = calculateEMA(prices, period);

    // Assert
    EXPECT_EQ(14.03125, result);
}

TEST(EMA, CalculatesEightDay){

    // Arrange
    // Apple - 23/07/2025 - 01/08/2025
    const std::vector<double> apple_prices = {
        214.15, // July 23, 2025
        213.76, // July 24, 2025
        213.88, // July 25, 2025
        214.05, // July 28, 2025
        211.27, // July 29, 2025
        209.05, // July 30, 2025
        207.57, // July 31, 2025
        202.38  // August 1, 2025
    };

    constexpr int period = 8;

    // Manually calculated EMA for this sequence and period
    // Initial EMA = 214.15, multiplier = 0.22
    // Step 1: (213.76 * 0.22) + (214.15 * (1 - 0.22) = 214.063
    // Step 2: (213.88 * 0.22) + (214.063 * (1 - 0.22) = 214.023
    // Step 3: (214.05 * 0.22) + (214.023 * (1 - 0.22) = 214.029
    // Step 4: (211.27 * 0.22) + (214.029 * (1 - 0.22) = 213.416
    // Step 5: (209.05 * 0.22) + (213.416 * (1 - 0.22) = 212.445
    // Step 6: (207.57 * 0.22) + (212.445 * (1 - 0.22) = 211.362
    // Step 7: (202.38 * 0.22) + (211.362 * (1 - 0.22) = 209.366

    // Act
    const double result = calculateEMA(apple_prices, period);

    // Assert
    ASSERT_NEAR(209.36604, result, 1e-5);
}

TEST(MACD, ReturnsZero_WhenNotEnoughData){

    // Arrange
    const std::vector<double> prices = { 1.0, 2.0, 3.0, 4.0, 5.0 };

    // Act
    const double result = calculateMACD(prices, 12, 26, 9);

    // Assert
    ASSERT_NEAR(0.0, result, 1e-6);
}

TEST(MACD, CalculatesCorrectly){

    // Arrange
    const std::vector<double> prices = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
    const double fastEMA = calculateEMA(prices, 12);
    const double slowEMA = calculateEMA(prices, 26);
    const double expectedMACD = fastEMA - slowEMA;

    // Act
    const double result = calculateMACD(prices, 12, 26, 9);

    // Assert
    ASSERT_NEAR(expectedMACD, result, 1e-6);
}