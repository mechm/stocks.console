#include "pch.h"
#include "CppUnitTest.h"

#include "../stocks.console.api/alpacha.cpp"

#include <vector>
#include <iomanip>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stockconsole_api_unit_test
{
	TEST_CLASS(stockconsole_api_alpacha_unit_test)
	{

	public:

	public:

		TEST_METHOD(TestGetAccount)
		{
			// Arrange
			AlpachaAccount alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Act
			RequestResponse result = alpacha.GetAccount();

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestBuyStock)
		{
			// Arrange
			AlpachaAccount alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Act
			RequestResponse result = alpacha.BuyStock("AAPL", 0.01);

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestSellStock)
		{
			// Arrange
			AlpachaAccount alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Act
			RequestResponse result = alpacha.SellStock("AAPL", 0.01);

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestStockDetail)
		{
			// Arrange
			AlpachaMarketData alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Act
			RequestResponse result = alpacha.GetAssetBySymbol("AAPL");

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		//TEST_METHOD(TestGetAllOpenPositions)
		//{
		//	// Arrange
		//	Alpacha alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
		//	bool expected = true;

		//	// Act
		//	RequestResponse result = alpacha.GetAllOpenPositions();

		//	// Assert
		//	Assert::AreEqual(expected, result.success);
		//}

		TEST_METHOD(TestGetHistoricalBars)
		{
			// Arrange
			AlpachaMarketData alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Parse "2023-06-17 12:36:51" string to time_t
			std::string datetime_str = "2023-06-17 12:36:51";
			struct tm tm_time = {};
			std::istringstream ss(datetime_str);
			ss >> std::get_time(&tm_time, "%Y-%m-%d %H:%M:%S");

			time_t target_time = mktime(&tm_time);

			// Act
			RequestResponse result = alpacha.GetHistoricalBars("AAPL", "1D", target_time);

			// Assert
			Assert::AreEqual(expected, result.success);
		}

	};
};