#include "pch.h"
#include "CppUnitTest.h"

#include "../stocks.console.indicator/rsi.h"

#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace stockindicatorunittest
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
	};
}
