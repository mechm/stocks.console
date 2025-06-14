#include "pch.h"
#include "CppUnitTest.h"


#include "../stocks.console/alpacha.cpp"

#include <vector>

//#include "../stocks.console.strategy/sma.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stockconsoleunittest
{
	TEST_CLASS(stockconsoleunittest)
	{
	public:	

		TEST_METHOD(TestGetAccount)
		{
			// Arrange
			Alpacha alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Act
			OrderResult result = alpacha.GetAccount();

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestBuyStock)
		{
			// Arrange
			Alpacha alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Act
			OrderResult result = alpacha.BuyStock("AAPL", 0.01);

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestSellStock)
		{
			// Arrange
			Alpacha alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;	

			// Act
			OrderResult result = alpacha.SellStock("AAPL", 0.01);

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestStockDetail)
		{
			// Arrange
			Alpacha alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Act
			OrderResult result = alpacha.StockDetail("AAPL");

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestGetAllOpenPositions)
		{
			// Arrange
			Alpacha alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Act
			OrderResult result = alpacha.GetAllOpenPositions();

			// Assert
			Assert::AreEqual(expected, result.success);
		}
	};
}
