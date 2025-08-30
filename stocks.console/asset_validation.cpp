#include <string>
#include <iostream>
#include <algorithm>
#include <cctype>

#include "../stocks.console.api/alpacha.h"
#include "asset_validation.h"

AssetResult GetValidAssetWithCancel(const Alpacha& alpacha) {
    std::string symbol;

    while (true) {
        std::cout << "Enter stock symbol (or 'cancel' to return to main menu): ";
        std::cin >> symbol;

        // Handle user cancellation
        if (isUserCancelling(symbol)) {
            return AssetResult{ false, Asset{}, "User cancelled" };
        }

        AssetResult assetResult = alpacha.GetAssetBySymbolAsObject(symbol);

        if (!assetResult.success) {
            std::cout << "\nInvalid stock symbol: " << symbol << std::endl;
            std::cout << "Error: " << assetResult.errorMessage << "\n" << std::endl;
            continue;
        }

        if (!assetResult.asset.tradable) {
            std::cout << "\nWarning: Stock " << symbol << " is not tradable!\n" << std::endl;
            continue;
        }

        // Valid and tradable asset found
        return assetResult;
    }
}

static bool isUserCancelling(const std::string& input) {
    if (input.size() != 6) return false;
    return std::equal(input.begin(), input.end(), "cancel",
        [](const char a, const char b) { return std::tolower(a) == std::tolower(b); });
}
