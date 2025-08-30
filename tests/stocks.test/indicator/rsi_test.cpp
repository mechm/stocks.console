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

