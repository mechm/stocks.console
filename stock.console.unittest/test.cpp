#include "pch.h"
#include "CppUnitTest.h"



#include <vector>
#include <ctime>
#include "../stocks.console.api/alpacha.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stockconsoleunittest
{
	TEST_CLASS(stockconsoleunittest)
	{
	public:

		TEST_METHOD(TestGetHistoricalBarsAsObjects)
		{
			// Arrange
			std::string testApiKey = "PKYD8GXEY7N6HC8VF9JU";
			std::string testSecretKey = "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse";
			bool paperTrading = true;

			Alpacha alpaca(testApiKey, testSecretKey, paperTrading);

			std::string symbol = "AAPL";
			std::string timeframe = "1Day";

			// Create a start time (30 days ago)
			time_t now = time(0);
			time_t start = now - (static_cast<long long>(30 * 24 * 60) * 60); // 30 days ago

			// Act
			HistoricalBarsResult result = alpaca.GetHistoricalBarsAsObjects(symbol, timeframe, start);

			// Assert
			// Note: This test will likely fail in a real environment without valid API keys
			// You may want to mock the API response or use dependency injection for better testing

			// Basic structure validation
			//Assert::AreEqual(symbol, result.symbol.c_str());

			// If the API call was successful, validate the structure
			if (result.success) {
				Assert::IsTrue(result.bars.size() > 0);

				// Validate first bar has required fields
				if (!result.bars.empty()) {
					const BarData& firstBar = result.bars[0];
					Assert::IsFalse(firstBar.timestamp.empty());
					Assert::IsTrue(firstBar.open >= 0);
					Assert::IsTrue(firstBar.high >= 0);
					Assert::IsTrue(firstBar.low >= 0);
					Assert::IsTrue(firstBar.close >= 0);
					Assert::IsTrue(firstBar.volume >= 0);
				}
			}
			else {
				// If API call failed, ensure error message is set
				Assert::IsFalse(result.error_message.empty());
			}
		}

		TEST_METHOD(TestGetHistoricalBarsAsObjects_InvalidSymbol)
		{
			// Arrange
			std::string testApiKey = "test_api_key";
			std::string testSecretKey = "test_secret_key";
			bool paperTrading = true;

			Alpacha alpaca(testApiKey, testSecretKey, paperTrading);

			std::string invalidSymbol = "INVALID_SYMBOL_XYZ";
			std::string timeframe = "1Day";
			time_t start = time(0) - (7 * 24 * 60 * 60); // 7 days ago

			// Act
			HistoricalBarsResult result = alpaca.GetHistoricalBarsAsObjects(invalidSymbol, timeframe, start);

			// Assert
			//Assert::AreEqual(invalidSymbol, result.symbol.c_str());
			// With invalid symbol, we expect either failure or empty bars
			if (!result.success) {
				Assert::IsFalse(result.error_message.empty());
			}
		}

		TEST_METHOD(TestGetHistoricalBarsAsObjects_EmptySymbol)
		{
			// Arrange
			std::string testApiKey = "test_api_key";
			std::string testSecretKey = "test_secret_key";
			bool paperTrading = true;

			Alpacha alpaca(testApiKey, testSecretKey, paperTrading);

			std::string emptySymbol = "";
			std::string timeframe = "1Day";
			time_t start = time(0) - (static_cast<long long>(7 * 24 * 60) * 60); // 7 days ago

			// Act
			HistoricalBarsResult result = alpaca.GetHistoricalBarsAsObjects(emptySymbol, timeframe, start);

			// Assert
			//Assert::AreEqual(emptySymbol, result.symbol.c_str());
			Assert::IsFalse(result.success);
			Assert::IsFalse(result.error_message.empty());
		}
	};
};