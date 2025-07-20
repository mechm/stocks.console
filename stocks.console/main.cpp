#include <iostream>
using namespace std;

#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>
#include <algorithm>

#include "../stocks.console.api/alpacha.h"
#include "../stocks.console.indicator/sma.h"

#include "date_validation.h"
#include "asset_validation.h"
#include "main.h"

#include "account_details.h"
#include "order.h"

int main()
{
    Json::Value root;
    std::ifstream config_doc("config_doc.json", std::ifstream::binary);
    config_doc >> root; 

    Alpacha alpacha(root.get("ALPACA_API_KEY", "").asString(),
        root.get("ALPACA_SECRET_KEY", "").asString());
   
    bool running = true;

    while (running) {
      
        cout << "\n====================[ Command Menu ]====================\n"
            << "  1: Get Account Details\n"
            << "  2: Check stock against indicator\n"
            << "  3: Buy Stock\n"
            << "  4: Sell Stock\n"
            << "  0: Exit\n"
            << "--------------------------------------------------------\n"
            << "Enter command: ";

        int command;
        cin >> command;

        if (cin.fail()) {
            cout << "Enter a valid value..." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (command) {
        case 0:
            running = false;
            cout << "Exiting program..." << endl;
            break;
        case 1:
            HandleAccountDetails(alpacha);
            break;
        case 2:
            HandleIndicatorAnalysis(alpacha);
            break;
        case 3:
        case 4:
            HandleOrder(alpacha, command, root);
            break;
        default:
            cout << "Unknown command: " << command << endl;
            break;
        }

        cout << endl; // Add a blank line between commands for better readability
    }
    return 0;
}

static void HandleIndicatorAnalysis(Alpacha& alpacha) {
    AssetResult assetResult = GetValidAssetWithCancel(alpacha);
    if (!assetResult.success) {
        cout << "Returning to main menu...\n" << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    int indicator = GetValidIndicator();
    if (indicator == 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (indicator == 1) { // SMA
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
       
        time_t validatedTime = 0;
        GetValidDateOrEmpty(validatedTime);
        int period = GetValidPeriod();
       
        // no date set so find x number of dates from period
        if (validatedTime == 0) {
            validatedTime = alpacha.GetTradingDateNDaysAgo(period);
        }
        
      



        HistoricalBarsResult historicPrices = 
            alpacha.GetHistoricalBarsAsObjects(assetResult.asset.symbol, "1D", validatedTime);

        if (historicPrices.success && !historicPrices.bars.empty()) {
           
            
            vector<double> closingPrices;
            for (const auto& bar : historicPrices.bars) {
                closingPrices.push_back(bar.close);
            }





            
            if (closingPrices.size() >= period) {
                double threshold = 1.0;
                double sma = calculateSMA(closingPrices, period);
                cout << "\nSMA (" << period << "-period) for " << assetResult.asset.symbol << ": " << fixed << setprecision(2) << sma << "\n" << endl;
                double currentPrice = closingPrices.back();
                int signal = getSMASignal(currentPrice, sma, threshold);
                printSMAAnalysis(currentPrice, sma, signal);
            }
            else {
                cout << "Not enough historical data for SMA calculation. Need at least " << period << " data points." << endl;
            }
        }





        else {
            cout << "Failed to retrieve historical data for " << assetResult.asset.symbol << endl;
        }
    }
    else if (indicator == 2) {
        // RSI logic placeholder
    }
    else if (indicator == 3) {
        // MACD logic placeholder
    }
}

static int GetValidPeriod() {
    int period;

    while (true) {
        cout << "Enter SMA period (number of days, e.g. 3, 10, 20): ";
        cin >> period;

        if (cin.fail()) {
            cout << "Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        if (period <= 0) {
            cout << "Period must be greater than 0." << endl;
            continue;
        }

        if (period > 200) {
            cout << "Period seems too large (max 200). Please enter a reasonable value." << endl;
            continue;
        }

        return period; // Valid period found
    }
}


// Prompt for valid indicator selection
static int GetValidIndicator() {
    int indicator;
    while (true) {
        cout << "Choose indicator, 1) SMA, 2) RSI, 3) MACD, 0) Main menu: ";
        cin >> indicator;
        if (cin.fail()) {
            cout << "Enter a valid selection..." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (indicator >= 0 && indicator <= 3) {
            return indicator;
        }
        cout << "Please enter a number between 0-3." << endl;
    }
}