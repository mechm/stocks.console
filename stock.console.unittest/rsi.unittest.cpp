#include "pch.h"
#include "CppUnitTest.h"
#include "../stocks.console.indicator/rsi.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace stock_indicator_unit_test
{
	TEST_CLASS(stock_indicator_rsi_unit_test)
	{

	public:
		TEST_METHOD(TestCalculateRSI_OverBought)
		{
			// Arrange
			std::vector<double> prices = { 10.0, 11.0, 12.0, 13.0, 14.0 };
			int period = 4;
			double expected = 100.0;

			// Act
			double result = calculateRSI(prices, period);

			// Assert
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(TestCalculateRSI_OverSold)
		{
			// Arrange
			std::vector<double> prices = { 14.0, 13.0, 12.0, 11.0, 10.0 };
			int period = 4;
			double expected = 0.0;

			// Act
			double result = calculateRSI(prices, period);

			// Assert
			Assert::AreEqual(expected, result);
		}

		TEST_METHOD(TestCalculateRSI_Neutral)
		{
			// Arrange // Apple stock - 25 July 2025 to 07 July 2025
			// for 14 day rsi needs comparison of 15 days
			// earliest to latest
			std::vector<double> prices = {
				209.95, 210.01, 211.14, 212.41, 211.16, 208.62,
				209.11, 210.16, 210.02, 211.18, 212.48, 214.40,
				214.15, 213.76, 213.88
			};

			int period = 14;

			double expected = 65.034429992348919;

			// Act
			double result = calculateRSI(prices, period);

			// Assert
			Assert::AreEqual(expected, result);
		}
	};
};
