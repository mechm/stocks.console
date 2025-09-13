#include "../../stocks.console.indicator/include/obv.h"

#include "gtest/gtest.h"

#include <vector>

TEST(OBV, CalculateOBV_BasicUpDownFlat){

    // Arrange
    const std::vector<double> prices = { 100.0, 101.0, 100.0, 100.0, 102.0, 101.0 };
    const std::vector<long long> volumes = { 1000, 2000, 1500, 1200, 1800, 1600 };

    // Act
    // Expected OBV calculation:
    // Day 0: 0
    // Day 1: 0 + 2000 = 2000 (up)
    // Day 2: 2000 - 1500 = 500 (down)
    // Day 3: 500 (flat)
    // Day 4: 500 + 1800 = 2300 (up)
    // Day 5: 2300 - 1600 = 700 (down)
    const std::vector<long long> expected = { 0, 2000, 500, 500, 2300, 700 };

    const std::vector<long long> result = calculateOBV(prices, volumes);

    // Assert
    EXPECT_EQ(expected.size(), result.size());
    for (size_t i = 0; i < expected.size(); ++i)
        EXPECT_EQ(expected[i], result[i]);
}

TEST(OBV, CalculateOBV_EmptyInput){

    // Arrange
    const std::vector<double> prices;
    const std::vector<long long> volumes;

    // Act
    const std::vector<long long> result = calculateOBV(prices, volumes);

    // Assert
    ASSERT_TRUE(result.empty());
}

TEST(OBV, CalculateOBV_SizeMismatch){

    // Arrange
    const std::vector<double> prices = { 100.0, 101.0 };
    const std::vector<long long> volumes = { 1000 };

    // Act
    const std::vector<long long> result = calculateOBV(prices, volumes);

    // Assert
    ASSERT_TRUE(result.empty());
}



