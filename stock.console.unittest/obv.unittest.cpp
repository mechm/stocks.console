#include "pch.h"
#include "CppUnitTest.h"
#include <vector>
#include <iostream>

#include "../stocks.console.indicator/obv.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace obv_unittest
{
    TEST_CLASS(OBVTests)
    {
    public:
        TEST_METHOD(CalculateOBV_BasicUpDownFlat)
        {
            std::vector<double> prices = { 100.0, 101.0, 100.0, 100.0, 102.0, 101.0 };
            std::vector<long long> volumes = { 1000, 2000, 1500, 1200, 1800, 1600 };

            // Expected OBV calculation:
            // Day 0: 0
            // Day 1: 0 + 2000 = 2000 (up)
            // Day 2: 2000 - 1500 = 500 (down)
            // Day 3: 500 (flat)
            // Day 4: 500 + 1800 = 2300 (up)
            // Day 5: 2300 - 1600 = 700 (down)
            std::vector<long long> expected = { 0, 2000, 500, 500, 2300, 700 };

            auto result = calculateOBV(prices, volumes);
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
                Assert::AreEqual(expected[i], result[i]);
        }

        TEST_METHOD(CalculateOBV_EmptyInput)
        {
            std::vector<double> prices;
            std::vector<long long> volumes;
            auto result = calculateOBV(prices, volumes);
            Assert::IsTrue(result.empty());
        }

        TEST_METHOD(CalculateOBV_SizeMismatch)
        {
            std::vector<double> prices = { 100.0, 101.0 };
            std::vector<long long> volumes = { 1000 };
            auto result = calculateOBV(prices, volumes);
            Assert::IsTrue(result.empty());
        }

        TEST_METHOD(CalculateOBV_AllFlat)
        {
            std::vector<double> prices = { 100.0, 100.0, 100.0 };
            std::vector<long long> volumes = { 1000, 2000, 3000 };
            std::vector<long long> expected = { 0, 0, 0 };
            auto result = calculateOBV(prices, volumes);
            Assert::AreEqual(expected.size(), result.size());
            for (size_t i = 0; i < expected.size(); ++i)
                Assert::AreEqual(expected[i], result[i]);
        }

        TEST_METHOD(CalculateOBV_AppleOBV)
        {
            // Test data for Apple (AAPL) from July 23 to August 1, 2025
            std::vector<double> apple_prices = {
                214.15, // Jul 23
                213.76, // Jul 24
                213.88, // Jul 25
                214.05, // Jul 28
                211.27, // Jul 29
                209.05, // Jul 30
                207.57, // Jul 31
                202.38  // Aug 1
            };
    
            // Volume in millions, converted to long long
            // volume is the day after, i.e. Jul 23 2025 is 24 Jul 2025
            std::vector<long long> apple_volumes = {
                46020000, // July 23, 2025
                40270000, // July 24, 2025
                37860000, // July 25, 2025
                51410000, // July 28, 2025
                45510000, // July 29, 2025
                80700000, // July 30, 2025
                104430000, // July 31, 2025
                75110000  // August 1, 2025
            };

            // Expected OBV values, in millions
            std::vector<long long> expected_obv = {
                0,         // Index 0: Initial OBV for July 23
                -40270000, // Index 1: OBV for July 24
                -2410000,  // Index 2: OBV for July 25
                49000000,  // Index 3: OBV for July 28
                3490000,   // Index 4: OBV for July 29
                -77210000, // Index 5: OBV for July 30
                -181640000,// Index 6: OBV for July 31
                -256750000 // Index 7: OBV for August 1
            };

            auto result = calculateOBV(apple_prices, apple_volumes);
            Assert::AreEqual(result.size(), expected_obv.size());
            for (size_t i = 0; i < expected_obv.size(); ++i)
                Assert::AreEqual(expected_obv[i], result[i]);
        }

        TEST_METHOD(CalculateOBV_SignalsWithAppleData)
        {    
            std::cout << "Running test for getOBVSignals with Apple data..." << std::endl;

            // Apple (AAPL) data from July 23 to August 1, 2025
            std::vector<double> apple_prices = {
                214.15, // Jul 23
                213.76, // Jul 24
                213.88, // Jul 25
                214.05, // Jul 28
                211.27, // Jul 29
                209.05, // Jul 30
                207.57, // Jul 31
                202.38  // Aug 1
            };

            std::vector<long long> apple_volumes = {
                46020000,
                40270000,
                37860000,
                51410000,
                45510000,
                80700000,
                104430000,
                75110000
            };

            // The expected signals based on manual analysis
            std::vector<int> expected_signals = {
                0, // Jul 23 (initial)
                -1, // Jul 24 (price down, OBV down)
                1,  // Jul 25 (price up, OBV up)
                1,  // Jul 28 (price up, OBV up)
                -1, // Jul 29 (price down, OBV down)
                -1, // Jul 30 (price down, OBV down)
                -1, // Jul 31 (price down, OBV down)
                -1  // Aug 1 (price down, OBV down)
            };

            try {
                // Step 1: Calculate the OBV values first, as they are needed for the signals
                std::vector<long long> obv_values = calculateOBV(apple_prices, apple_volumes);

                // Step 2: Generate the signals using the calculated OBV
                std::vector<int> generated_signals = getOBVSignals(apple_prices, obv_values);

                // Assert that the size of the result vector is correct
                Assert::IsTrue(generated_signals.size() == expected_signals.size());

                // Assert that each generated signal matches the expected signal
                for (size_t i = 0; i < generated_signals.size(); ++i) {
                    if (generated_signals[i] != expected_signals[i]) {
                        std::cerr << "Test failed at index " << i << ". Expected: "                      
                            << expected_signals[i]
                            << ", but got: "
                            << generated_signals[i] << std::endl;
                            Assert::IsTrue(false); // Fail the test
                    }
                }
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error during test setup: " << e.what() << std::endl;
                Assert::IsTrue(false);
            }

            std::cout << "Test for getOBVSignals with Apple data PASSED! 🎉" << std::endl;
        }
    };
}