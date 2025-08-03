#include "pch.h"
#include "CppUnitTest.h"
#include "../stocks.console.api/alpacha.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "../stocks.console/rsi2.cpp"
#include "../stocks.console/date_validation.cpp"


//
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

// Mock Alpacha class for testing
class MockAlpacha : public Alpacha {
public:
    using Alpacha::Alpacha;

    HistoricalClosedPrices mockPrices;

    HistoricalClosedPrices GetHistoricalClosedPrices(const std::string& symbol, const std::string& timeframe, const time_t start) const {
        return mockPrices;
    }


    time_t GetTradingDateNDaysAgo(int daysAgo) {
        return 1234567890; // dummy value
    }
};

namespace ShowRSITests
{
   TEST_CLASS(ShowRSITests)
    {
       
    public:
        // Helper to capture std::cout output
        std::string CaptureShowRSIOutput(MockAlpacha& alpacha, const std::string& symbol) {
            std::stringstream buffer;
            std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
            //alpaca2 = new Alpacha("", "");
            Alpacha alpacha3("","" );
          
            ShowRSI(alpacha, symbol);
         
            std::cout.rdbuf(old);
            return buffer.str();
        }
////
////        TEST_METHOD(ShowRSI_FailsOnUnsuccessfulFetch)
////        {
////            MockAlpacha alpacha("", "");
////            alpacha.mockPrices.success = false;
////            alpacha.mockPrices.error_message = "API error";
////            std::string output = CaptureShowRSIOutput(alpacha, "AAPL");
////            Assert::IsTrue(output.find("Failed to retrieve historical data for AAPL: API error") != std::string::npos);
////        }
////
////        TEST_METHOD(ShowRSI_FailsOnEmptyPrices)
////        {
////            MockAlpacha alpacha("", "");
////            alpacha.mockPrices.success = true;
////            alpacha.mockPrices.prices.clear();
////            std::string output = CaptureShowRSIOutput(alpacha, "AAPL");
////            Assert::IsTrue(output.find("No bar data found for AAPL") != std::string::npos);
////        }
////
////        TEST_METHOD(ShowRSI_FailsOnNotEnoughData)
////        {
////            MockAlpacha alpacha("", "");
////            alpacha.mockPrices.success = true;
////            alpacha.mockPrices.prices = { 100.0, 101.0 }; // less than period+1
////            int period = 5;
////            // Simulate user input for period
////            std::istringstream input("5\n");
////            std::cin.rdbuf(input.rdbuf());
////            std::string output = CaptureShowRSIOutput(alpacha, "AAPL");
////            Assert::IsTrue(output.find("Not enough historical data for RSI calculation") != std::string::npos);
////        }
////
        TEST_METHOD(ShowRSI_PrintsRSIAnalysis)
        {
            //ShowRSI2();
            MockAlpacha alpacha("", "");
            alpacha.mockPrices.success = true;
            alpacha.mockPrices.prices = { 44.34, 44.09, 44.15, 43.61, 44.33, 44.83, 45.10, 45.42, 45.84, 46.08, 45.89, 46.03, 45.61, 46.28, 46.28 };
            // Simulate user input for period
            std::istringstream input("14\n");
            std::cin.rdbuf(input.rdbuf());
            std::string output = CaptureShowRSIOutput(alpacha, "AAPL");
            Assert::IsTrue(output.find("RSI Analysis") != std::string::npos);
            Assert::IsTrue(output.find("Current Price: $46.28") != std::string::npos);
        }
   };
}