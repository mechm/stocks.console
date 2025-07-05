#include "pch.h"
#include "CppUnitTest.h"

#include "../stocks.console.api/alpacha.h"

#include <vector>
#include <iomanip>
#include <json/json.h>

#include <iostream>
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace stockconsole_api_unit_test
{
	TEST_CLASS(stockconsole_api_alpacha_unit_test)
	{

	private:
		Alpacha* alpaca;	
		const char* apiKey;
		const char* secretKey;
		bool isPaperTrading;

		TEST_METHOD_INITIALIZE(SetUp)
		{
			//Json::Value root;   // 'root' will contain the root value after parsing.
			//std::ifstream config_doc("config_doc.json", std::ifstream::binary);
			//config_doc >> root;
			
			// Initialize test configuration
			//apiKey = root.get("TEST_ALPACA_API_KEY", "").asCString();
			//secretKey = root.get("TEST_ALPACA_SECRET_KEY", "").asCString();
			apiKey = "PKYD8GXEY7N6HC8VF9JU";
			secretKey = "ZCEve7k69aN7YqGMwNjXkB1Ju5AibTXokb4xBHse";
			isPaperTrading = true;

			// Initialize Alpaca instances
			alpaca = new Alpacha(apiKey, secretKey, isPaperTrading);
		}

		TEST_METHOD_CLEANUP(TearDown)
		{
			// Clean up resources
			delete alpaca;
			alpaca = nullptr;
		}

		TEST_METHOD(TestGetAccount)
		{
			// Arrange
			bool expected = true;

			// Act		
			RequestResponse result = alpaca->GetAccount();

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestBuyStock)
		{
			// Arrange			
			bool expected = true;

			// Act
			RequestResponse result = alpaca->BuyStock("AAPL", 0.01);

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestSellStock)
		{
			// Arrange			
			bool expected = true;

			// Act
			RequestResponse result = alpaca->SellStock("AAPL", 0.01);

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestStockDetail)
		{
			// Arrange			
			bool expected = true;

			// Act
			RequestResponse result = alpaca->GetAssetBySymbol("AAPL");

			// Assert
			Assert::AreEqual(expected, result.success);
		}

		TEST_METHOD(TestGetHistoricalBars)
		{
			// Arrange			
			bool expected = true;

			// Parse "2023-06-17 12:36:51" string to time_t
			std::string datetime_str = "2023-06-17 12:36:51";
			struct tm tm_time = {};
			std::istringstream ss(datetime_str);
			ss >> std::get_time(&tm_time, "%Y-%m-%d %H:%M:%S");

			time_t target_time = mktime(&tm_time);

			// Act
			RequestResponse result = alpaca->GetHistoricalBars("AAPL", "1D", target_time);

			// Assert
			Assert::AreEqual(expected, result.success);
		}
	};
};