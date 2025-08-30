#include <json/value.h>
#include <iostream>

#include "../stocks.console.utilities/formatter.h"
#include "../stocks.console.api/finn_hub.h"

void HandleStockDetail(const FinnHub& finnHub)
{
    std::string companyName;
    std::cout << "Enter a company name: ";
    std::cin >> companyName;

    if (const auto [success, response] = finnHub.GetSymbolLookup(companyName); success) {
        PrintFormattedJson(response, "Symbol Lookup");
    }
    else {
        std::cout << "Error: " << response << "\n";
    }

}
