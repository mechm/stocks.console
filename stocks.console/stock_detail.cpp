#include <json/value.h>
#include <iostream>
#include <vector>
#include <json/json.h>

#include "../stocks.console.api/alpacha.h"
#include "asset_validation.h"
#include "stock_d.h"
#include "../stocks.console.utilities/formatter.h"


#include "../stocks.console.api/finn_hub.h"

//void HandleStockDetail(const FinnHub& finnHub);


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

void HandleOrder2(Alpacha& alpacha, const int command, const Json::Value& root)
{
    const AssetResult assetResult = GetValidAssetWithCancel(alpacha);
    if (!assetResult.success) {
        std::cout << "Returning to main menu...\n" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    float amount = 0.0;
    std::cout << "Enter the amount: ";
    std::cin >> amount;






    const std::string side = (command == 3) ? "buy" : "sell";
    std::cout << "Placing " << side << " order for " << assetResult.asset.symbol << "..." << std::endl;

    const auto [success, response] = alpacha.BuyStock(assetResult.asset.symbol, amount);
    if (!success) {
        std::cout << "Error placing order: " << response << std::endl;
        return;
    }

    PrintFormattedJson(response, "Order Details - " + assetResult.asset.symbol);
}