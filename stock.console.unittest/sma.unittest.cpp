#include "pch.h"
#include "CppUnitTest.h"
#include "../stocks.console.indicator/sma.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace stock_indicator_unit_test
{
	TEST_CLASS(stock_indicator_sma_unit_test)
	{

	public:
		TEST_METHOD(TestCalculateSMA_BasicCase)
		{
			// Arrange
			std::vector<double> prices = { 10.0, 11.0, 12.0, 13.0, 14.0 };
			int period = 5;
			double expected = 12.0;

			// Act
			double result = calculateSMA(prices, period);

			// Assert
			Assert::AreEqual(expected, result);
		}
	};
};
