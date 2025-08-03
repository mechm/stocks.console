#include <iostream>
#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "asset_validation.h"
#include "indicator_analysis.h"
#include "sma.h"
#include "rsi2.h"
#include "../stocks.console.indicator/rsi.h"
#include "../stocks.console.indicator/sma.h"


void HandleIndicatorAnalysis(Alpacha& alpacha)
{
    const AssetResult assetResult = GetValidAssetWithCancel(alpacha);
    if (!assetResult.success) {
        std::cout << "Returning to main menu...\n" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    bool tryAnother = true;
    while (tryAnother) {
        const std::string indicator = GetValidIndicator();
        
            switch (std::stoi(indicator)) { // Convert string to integer for switch
            case 0: // Main menu
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return;
            case 1: // Indicator help
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                showIndicatorHelpMenu();
                break;
            case 2: // SMA
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                ShowSMA(alpacha, assetResult.asset.symbol);
                break;
            case 3: // RSI
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                ShowRSI(alpacha, assetResult.asset.symbol);
                break;
            case 4: // MACD
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                // MACD logic placeholder
                break;
            default:
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Unknown indicator selection." << std::endl;
                break;
            }

        // Ask if user wants to try another indicator
        std::cout << "Would you like to try another indicator for this asset? (y/n): ";
        char response;
        std::cin >> response;
        if (std::cin.fail() || (response != 'y' && response != 'Y')) {
            tryAnother = false;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}

static void showIndicatorHelpMenu()
{
    std::cout << "Available indicators:\n"
        << "1) Indicator Help: Displays this help message.\n"
        << "2) SMA: Simple Moving Average, a trend-following indicator that smooths price data.\n"
        << "3) RSI: Relative Strength Index, a momentum oscillator that measures the speed and change of price movements.\n"
        << "4) MACD: Moving Average Convergence Divergence, a trend-following momentum indicator that shows the relationship between two moving averages of a security's price.\n";

    // Ask which help to show more information for
    std::cout << "Show help for: 1) SMA  2) RSI  (Enter number, other to skip): ";
    int helpChoice = 0;
    std::cin >> helpChoice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else {
        if (helpChoice == 1) {
            printSMAHelp();
        }
        else if (helpChoice == 2) {
            printRSIHelp();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// Rename the function to avoid overloading conflicts
static std::string GetValidIndicator() {
    std::string indicator;
    while (true) {
        std::cout << "Choose indicator, 1) SMA, 2) RSI, 3) MACD, 0) Main menu, h) help: ";
        std::cin >> indicator;
        if (std::cin.fail()) {
            std::cout << "Enter a valid selection..." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if (indicator == "1" || indicator == "2" || indicator == "3" || indicator == "0" || indicator == "h") {
            return indicator;
        }
        std::cout << "Please enter a valid option (0-3 or h)." << std::endl;
    }
}