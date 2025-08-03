#include "pch.h"
#include "CppUnitTest.h"
#include "../stocks.console.indicator/macd.h"
#include <vector>
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stock_indicator_unit_test
{
    TEST_CLASS(stock_indicator_macd_unit_test)
    {
    public:
        TEST_METHOD(EMA_ReturnsZero_WhenNotEnoughData)
        {
            std::vector<double> prices = { 1.0, 2.0, 3.0 };
            int period = 5;
            double ema = calculateEMA(prices, period);
            Assert::AreEqual(0.0, ema, 1e-6);
        }

        TEST_METHOD(EMA_CalculatesCorrectly)
        {
            std::vector<double> prices = { 10, 11, 12, 13, 14, 15 };
            int period = 3;
            double ema = calculateEMA(prices, period);
            // Manually calculated EMA for this sequence and period
            // Initial EMA = 10, multiplier = 0.5
            // Step 1: (11 * 0.5) + (10 * 0.5) = 10.5
            // Step 2: (12 * 0.5) + (10.5 * 0.5) = 11.25
            // Step 3: (13 * 0.5) + (11.25 * 0.5) = 12.125
            // Step 4: (14 * 0.5) + (12.125 * 0.5) = 13.0625
            // Step 5: (15 * 0.5) + (13.0625 * 0.5) = 14.03125
            Assert::AreEqual(14.03125, ema, 1e-5);
        }      
  
        TEST_METHOD(EMA_CalculatesEightDay)
        {
            // Apple - 23/07/2025 - 01/08/2025
            std::vector<double> apple_prices = {
                214.15, // July 23, 2025
                213.76, // July 24, 2025
                213.88, // July 25, 2025
                214.05, // July 28, 2025
                211.27, // July 29, 2025
                209.05, // July 30, 2025
                207.57, // July 31, 2025
                202.38  // August 1, 2025
            };

            int period = 8;
            double ema = calculateEMA(apple_prices, period);
            // Manually calculated EMA for this sequence and period
            // Initial EMA = 214.15, multiplier = 0.22     
            // Step 1: (213.76 * 0.22) + (214.15 * (1 - 0.22) = 214.063
            // Step 2: (213.88 * 0.22) + (214.063 * (1 - 0.22) = 214.023
            // Step 3: (214.05 * 0.22) + (214.023 * (1 - 0.22) = 214.029
            // Step 4: (211.27 * 0.22) + (214.029 * (1 - 0.22) = 213.416
            // Step 5: (209.05 * 0.22) + (213.416 * (1 - 0.22) = 212.445
            // Step 6: (207.57 * 0.22) + (212.445 * (1 - 0.22) = 211.362
            // Step 7: (202.38 * 0.22) + (211.362 * (1 - 0.22) = 209.366
            Assert::AreEqual(209.36604, ema, 1e-5);
        }

        TEST_METHOD(MACD_ReturnsZero_WhenNotEnoughData)
        {
            std::vector<double> prices = { 1.0, 2.0, 3.0, 4.0, 5.0 };
            double macd = calculateMACD(prices, 12, 26, 9);
            Assert::AreEqual(0.0, macd, 1e-6);
        }

        TEST_METHOD(MACD_CalculatesCorrectly)
        {
            std::vector<double> prices = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };
            // For this test, fastPeriod=12, slowPeriod=26, so both EMAs use all data
            double fastEMA = calculateEMA(prices, 12);
            double slowEMA = calculateEMA(prices, 26);
            double expectedMACD = fastEMA - slowEMA;
            double macd = calculateMACD(prices, 12, 26, 9);
            Assert::AreEqual(expectedMACD, macd, 1e-6);
        }
    };
}