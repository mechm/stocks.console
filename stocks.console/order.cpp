#include <json/value.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "asset_validation.h"
#include "order.h"
#include "account_details.h"
#include "../stocks.console.utilities/formatter.h"

void HandleOrder(Alpacha& alpacha, int command, const Json::Value& root) 
{
    AssetResult assetResult = GetValidAssetWithCancel(alpacha);
    if (!assetResult.success) {
        std::cout << "Returning to main menu...\n" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    float amount = 0.0;
    std::cout << "Enter the amount: ";
    std::cin >> amount;
    std::string side = (command == 3) ? "buy" : "sell";
    std::cout << "Placing " << side << " order for " << assetResult.asset.symbol << "..." << std::endl;
  
    RequestResponse result = alpacha.BuyStock(assetResult.asset.symbol, amount);
    if (!result.success) {
        std::cout << "Error placing order: " << result.response << std::endl;
        return;
	}

	PrintFormattedJson(result.response, "Order Details - " + assetResult.asset.symbol);
}