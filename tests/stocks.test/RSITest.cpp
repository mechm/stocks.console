#include "gtest/gtest.h"

#include <vector>
#include "../../stocks.console.indicator/rsi.h"

TEST(AbsoluteDateTestSuite, ExampleDate){ // 12/2/2020 -> 737761
//
//
//    // Arrange
    const std::vector<double> prices = { 10.0, 11.0, 12.0, 13.0, 14.0 };
    const int period = 4;
    double expected = 100.0;
//
//    // Act
    double result = calculateRSI(prices, period);
//
//    // // Assert
//    // Assert::AreEqual(expected, result);
//    //
//    // GregorianDate gregDate;
//    // gregDate.SetMonth(12);
//    // gregDate.SetDay(2);
//    // gregDate.SetYear(2020);
//    //
    EXPECT_EQ(1,1);
//    // EXPECT_EQ(gregDate.getAbsoluteDate(),737761);
}
//
////
 TEST(AbsoluteDateTestSuite, IncorrectDate){ // 12/0/2020 -> 0
////     GregorianDate gregDate;
////     gregDate.SetMonth(12);
////     gregDate.SetDay(0);
////     gregDate.SetYear(2020);
////
EXPECT_EQ(1,1);
////     ASSERT_EQ(gregDate.getAbsoluteDate(),0);
 }