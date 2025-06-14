#include "pch.h"
#include "CppUnitTest.h"

#include "../stocks.console.indicator/sma.cpp"

#include <vector>

//#include "../stocks.console.strategy/sma.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace stockstrategyunittest
{
	TEST_CLASS(stockstrategyunittest)
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
}
