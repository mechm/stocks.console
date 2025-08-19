#include "../stocks.console.api/alpacha.h"
#include <json/value.h>
#include <iostream>
#include "../stocks.console.api/finn_hub.h"
#include "stock_detail.h"


void HandleStockDetail()
{
    std::string companyName;
    std::cout << "Enter a company name: ";
    std::cin >> companyName;

    const FinnHub finHub("");
    const RequestResponse response = finHub.GetSymbolLookup(companyName);


    // Output the response to the console window
    std::cout << "Symbol Lookup Response:\n";
    std::cout << response.response << std::endl;

}