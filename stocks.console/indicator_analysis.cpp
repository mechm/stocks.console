#include <iostream>
#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "asset_validation.h"
#include "indicator_analysis.h"
#include "sma.h"
#include "rsi.h"

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

    while(true) {
        std::string indicator = GetValidIndicator();

        // Handle help request
        if (indicator == "h" || indicator == "H") {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            showIndicatorHelpMenu();
            continue;
        }

        // Try to convert indicator to int, handle invalid input
        int indicatorValue = -1;
        try {
            indicatorValue = std::stoi(indicator);
        }
        catch (const std::exception&) {
            std::cout << "Unknown indicator selection." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
       
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (indicatorValue) {
        case 0: // Main menu
            return;
        case 1: // SMA
            ShowSMA(alpacha, assetResult.asset.symbol);
            break;
        case 2: // RSI
            ShowRSI(alpacha, assetResult.asset.symbol);
            break;
        case 3: // MACD
            //ShowMACD(alpacha, assetResult.asset.symbol);
            break;
        case 4: // OBV
            // Future: ShowOBV(alpacha, assetResult.asset.symbol);
            break;
        default:
            std::cout << "Unknown indicator selection." << std::endl;
            break;
        }
    }
}

static void showIndicatorHelpMenu()
{
    struct IndicatorHelp {
        int id;
        const char* name;
        const char* description;
        void (*printHelp)();
    };

    // Array of available indicators and their help functions
    constexpr IndicatorHelp indicators[] = {
         {1, "SMA",  "Simple Moving Average, a trend-following indicator that smooths price data.", printSMAHelp},
         {2, "RSI",  "Relative Strength Index, a momentum oscillator that measures the speed and change of price movements.", printRSIHelp},
        // {3, "MACD", "Moving Average Convergence Divergence, a trend-following momentum indicator that shows the relationship between two moving averages of a security's price.", printMACDHelp },
         //{4, "OBV",  "On-Balance Volume, a volume-based indicator that adds volume on up days and subtracts volume on down days to confirm price trends and spot potential reversals.", printOBVHelp }
    };

    std::cout << "Available indicators:\n";
    for (const auto& ind : indicators) {
        std::cout << ind.id << ") " << ind.name << ": " << ind.description << "\n";
    }

    std::cout << "Show help for: ";
    for (const auto& ind : indicators) {
        std::cout << ind.id << ") " << ind.name << "  ";
    }
    std::cout << "(Enter number, other to skip): ";

    int helpChoice = 0;
    std::cin >> helpChoice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    for (const auto& ind : indicators) {
        if (helpChoice == ind.id) {
            ind.printHelp();
            break;
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

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