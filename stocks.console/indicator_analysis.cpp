#include <iostream>
#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "asset_validation.h"
#include "indicator_analysis.h"
#include "sma.h"

void HandleIndicatorAnalysis(Alpacha& alpacha) 
{
    const AssetResult assetResult = GetValidAssetWithCancel(alpacha);
    if (!assetResult.success) {
        std::cout << "Returning to main menu...\n" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    const int indicator = GetValidIndicator();
    if (indicator == 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    if (indicator == 1) { // SMA
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        ShowSMA(alpacha, assetResult.asset.symbol);
    }
    else if (indicator == 2) {
        







    }
    else if (indicator == 3) {
        










    }
}

static int GetValidIndicator() {
    int indicator = 0;
    while (true) {
        std::cout << "Choose indicator, 1) SMA, 2) RSI, 3) MACD, 0) Main menu: ";
        std::cin >> indicator;
        if (std::cin.fail()) {
            std::cout << "Enter a valid selection..." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (indicator >= 0 && indicator <= 3) {
            return indicator;
        }
        std::cout << "Please enter a number between 0-3." << std::endl;
    }
}