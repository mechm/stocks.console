#include "gtest/gtest.h"

#include <vector>
#include "../../stocks.console.indicator/rsi.h"

TEST(RSI, CalculateRSI_OverBought){

    // Arrange
    const std::vector<double> prices = { 10.0, 11.0, 12.0, 13.0, 14.0 };
    constexpr int period = 4;
    constexpr double expected = 100.0;

    // Act
    const double result = calculateRSI(prices, period);

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(RSI, CalculateRSI_OverSold){

    // Arrange
    const std::vector<double> prices = { 14.0, 13.0, 12.0, 11.0, 10.0 };
    constexpr int period = 4;
    constexpr double expected = 0.0;

    // Act
    const double result = calculateRSI(prices, period);

    // Assert
    EXPECT_EQ(expected, result);
}

TEST(RSI, CalculateRSI_Neutral){

    // Arrange
    // Apple stock - 25 July 2025 to 07 July 2025
    // for 14 day rsi needs comparison of 15 days
    // earliest to latest
    const std::vector<double> prices = { 209.95, 210.01, 211.14, 212.41, 211.16, 208.62,
                209.11, 210.16, 210.02, 211.18, 212.48, 214.40,
                214.15, 213.76, 213.88 };
    constexpr int period = 14;
    constexpr double expected = 65.034429992348919;

    // Act
    const double result = calculateRSI(prices, period);

    // Assert
    EXPECT_EQ(expected, result);
}

