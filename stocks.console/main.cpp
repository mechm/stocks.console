#include <iostream>
using namespace std;

#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "../stocks.console.indicator/sma.h"
#include "string_utilities.h"
#include "main.h"

int main()
{
    Json::Value root;
    std::ifstream config_doc("config_doc.json", std::ifstream::binary);
    config_doc >> root;    

    bool running = true;

    Alpacha alpacha(root.get("ALPACA_API_KEY", "").asString(),
        root.get("ALPACA_SECRET_KEY", "").asString());

    while (running) {

        cout << "Enter command "
                "1: Get Account Details, " 
                "2: Check stock against indicator, "
                "3: Buy Stock, "
                "4: Sell Stock, " 
                "0: Exit): ";

        int command;
        cin >> command;

        if (cin.fail()) {
            cout << "Enter a valid value..." << endl;
            cin.clear();
            cin.ignore(10000, '\n');    // Ignore up to 10000 characters or until newline
            continue;
        }

        if (command == 0) {
            running = false;
            cout << "Exiting program..." << endl;
            continue;
        }

        // check account details
        if (command == 1) {
            RequestResponse account = alpacha.GetAccount();
            if (account.success)
            {
                cout << "\nAccount details: " << account.response <<  "\n" << endl;
            }
            continue;
        }

        // run stock symbol against an indicator
        if (command == 2) {    
            
            string symbol; // No need to initialize, function will set it      
            AssetResult assetResult = GetValidAssetWithCancel(symbol, alpacha);

            if (!assetResult.success) {
                cout << "Returning to main menu...\n" << endl;
                continue; // Go back to main menu
            }

            int indicator;
            cout << "Choose indicator, 1) SMA, 2) RSI, 3) MACD: ";
            cin >> indicator;

            if (cin.fail()) {
                cout << "Enter a valid selection..." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
                continue;
            }

            if (indicator == 1) {
              
                string date = GetValidDateOrEmpty();

                time_t validatedTime = 0; // Default to 0 for empty date
                if (!date.empty()) {
                    StringUtilities::ValidateDate(date, validatedTime);
                }

                HistoricalBarsResult historicPrices = 
                    alpacha.GetHistoricalBarsAsObjects(symbol, "1D", validatedTime);

                if (historicPrices.success && !historicPrices.bars.empty()) {              
                    vector<double> closingPrices;
                    for (const auto& bar : historicPrices.bars) {
                        closingPrices.push_back(bar.close);
                    }

                    int period = GetValidPeriod();

                    if (closingPrices.size() >= period) 
                    {
                        // int period = 10;
                        double threshold = 1.0; // 1% threshold
                        // Calculate SMA
                        double sma = calculateSMA(closingPrices, period);
                        cout << "\nSMA (" << period << "-period) for " << symbol << ": " << fixed << setprecision(2) << sma << "\n" << endl;
                        double currentPrice = closingPrices[0];

                        // Get signal                     
                        int signal = getSMASignal(currentPrice, sma, threshold);

                        // Print analysis
                        printSMAAnalysis(currentPrice, sma, signal);
                    }
                    else {
                        cout << "Not enough historical data for SMA calculation. Need at least " << period << " data points." << endl;
                    }
                }
                else {
                    cout << "Failed to retrieve historical data for " << symbol << endl;
                }
            }
            else if (indicator == 2)
            {
                // Get API credentials from config file
                Alpacha alpacha(root.get("ALPACA_API_KEY", "").asString(),
                    root.get("ALPACA_SECRET_KEY", "").asString());
            }
        }

        // buy or sell stock
        else if (command == 3 || command == 4) {            
            string symbol;

            cout << "Enter stock symbol: ";
            cin >> symbol;

            // check valid stock symbol

            float amount;
            cout << "Enter the amount: ";
            cin >> amount;

            // check valid amount

            string side = (command == 3) ? "buy" : "sell";

            cout << "Placing " << side << " order for " << symbol << "..." << endl;
            
            Alpacha alpacha1(root.get("ALPACA_API_KEY", "").asString(),
                            root.get("ALPACA_SECRET_KEY", "").asString());

            alpacha1.BuyStock(symbol, amount);
        }
        else {
            cout << "Unknown command: " << command << endl;
        }
        cout << endl; // Add a blank line between commands for better readability
    }
    return 0;
}

static AssetResult GetValidAssetWithCancel(string& symbol, Alpacha alpacha) {
    AssetResult assetResult;

    while (true) {
        cout << "Enter stock symbol (or 'cancel' to return to main menu): ";
        cin >> symbol;

        if (symbol == "cancel" || symbol == "CANCEL") {
            assetResult.success = false;
            assetResult.errorMessage = "User cancelled";
            break;
        }

        assetResult = alpacha.GetAssetBySymbolAsObject(symbol);

        if (!assetResult.success) {
            cout << "\nInvalid stock symbol: " << symbol << "\n" << endl;
            cout << "Error: " << assetResult.errorMessage << "\n" << endl;
        }
        else if (!assetResult.asset.tradable) {
            cout << "\nWarning: Stock " << symbol << " is not tradable!\n" << endl;
        }
        else {
            break; // Valid and tradable asset found
        }
    }

    return assetResult;
}


static string GetValidDateOrEmpty() {
    string date;
    time_t validatedTime;

    while (true) {
        cout << "Choose date (YYYY-MM-DD format), or leave blank: ";
        
        cin.ignore(); // Clear any leftover newline characters
        getline(cin, date); // Use getline to capture empty input
       
        // If empty, return immediately
        if (date.empty()) {
            return date;
        }      

        // Validate the date
        if (StringUtilities::ValidateDate(date, validatedTime)) {
            return date; // Valid date, return it
        }

        // Invalid date, ask again
        cout << "Please enter a valid date and try again." << endl;
        cin.clear();
        cin.ignore(10000, '\n');
    }
}

// Add this function before main() or in a header file
static int GetValidPeriod() {
    int period;

    while (true) {
        cout << "Enter SMA period (number of days, e.g., 3, 10, 20): ";
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