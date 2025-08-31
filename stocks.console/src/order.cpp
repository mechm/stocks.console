#include <json/value.h>
#include <iostream>
#include <vector>
#include <json/json.h>

#include "../../stocks.console.api/alpacha.h"
#include "../include/asset_validation.h"
#include "../include/order.h"
#include "../../stocks.console.utilities/formatter.h"

void HandleOrder(const Alpacha& alpacha, const int command, const Json::Value& root)
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