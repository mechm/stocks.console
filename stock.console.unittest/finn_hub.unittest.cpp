#include "pch.h"
#include "CppUnitTest.h"

#include <vector>
#include "../stocks.console.api/finn_hub.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace stock_indicator_unit_test
{
	TEST_CLASS(finn_hub_unit_test)
	{

	public:
		TEST_METHOD(TestGetSymbolLookup)
		{
			FinnHub finHub("d2hn5tpr01qqpjgoufegd2hn5tpr01qqpjgouff0");

			// Act
			RequestResponse result = finHub.GetSymbolLookup("apple");

			// Assert
			//Assert::AreEqual(expected, result);
		}
	};
}