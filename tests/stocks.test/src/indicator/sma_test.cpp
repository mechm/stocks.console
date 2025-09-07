#include "../../stocks.console.indicator/include/sma.h"

#include "gtest/gtest.h"

#include <vector>

TEST(SMA, CalculateSMA_BasicCase){

    // Arrange
    const std::vector<double> prices = { 10.0, 11.0, 12.0, 13.0, 14.0 };
    const constexpr int period = 5;
    const constexpr double expected = 12.0;

    // Act
    const double result = calculateSMA(prices, period);

    // Assert
    EXPECT_EQ(expected, result);
}