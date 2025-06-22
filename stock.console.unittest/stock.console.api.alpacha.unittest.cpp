#include "pch.h"
#include "CppUnitTest.h"

#include "../stocks.console.api/alpachat.cpp"

#include <vector>
#include <iomanip>

//#include "../stocks.console.strategy/sma.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stockconsole_api_unit_test
{
	TEST_CLASS(stockconsole_api_alpacha_unit_test)
	{

	public:
		TEST_METHOD(TestGetStockAuction)
		{
			// Arrange
			AlpachaT alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Act
			OrderResult result = alpacha.GetStockAuction("AAPL");

			// Assert
			Assert::AreEqual(expected, result.success);
		}


		TEST_METHOD(TestGetHistoricalBars)
		{
			// Arrange
			AlpachaT alpacha("PKYD8GXEY7N6HC8VF9JU", "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse", true);
			bool expected = true;

			// Parse "2023-06-17 12:36:51" string to time_t
			std::string datetime_str = "2023-06-17 12:36:51";
			struct tm tm_time = {};
			std::istringstream ss(datetime_str);
			ss >> std::get_time(&tm_time, "%Y-%m-%d %H:%M:%S");

			time_t target_time = mktime(&tm_time);

			// Act
			OrderResult result = alpacha.GetHistoricalBars("AAPL", "1D", target_time);

			// Assert
			Assert::AreEqual(expected, result.success);
		}

	};
}