#include <iostream>
using namespace std;

#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "../stocks.console.indicator/sma.h"

#include "string_utilities.h"
#include "date_validation.h"
#include "asset_validation.h"
#include "main.h"

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

// Add this helper function
static void PrintFormattedJson(const string& jsonString, const string& title = "JSON Data") {
    Json::Value root;
    Json::Reader reader;

    if (reader.parse(jsonString, root)) {
        // Header
        cout << "\n" << string(50, '=') << "\n";
        cout << setw(25 + title.length() / 2) << title << "\n";
        cout << string(50, '=') << "\n\n";

        // Formatted JSON
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";  // 4 spaces
        builder["precision"] = 2;

        unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(root, &cout);

        cout << string(50, '=') << "\n";
    }
    else {
        cout << "Invalid JSON format:\n" << jsonString << "\n";
    }
}

// Use it in your function
static void HandleAccountDetails(Alpacha& alpacha) {
    RequestResponse account = alpacha.GetAccount();
    if (account.success) {
        PrintFormattedJson(account.response, "Account Details");
    }
    else {
        cout << "Error: " << account.response << "\n";
    }
}

// Handle account details command
//static void HandleAccountDetails(Alpacha& alpacha) {
//    RequestResponse account = alpacha.GetAccount();
//    if (account.success) {
//        cout << "\nAccount details: " << account.response << "\n" << endl;
//    }
//}

// Handle indicator analysis command
static void HandleIndicatorAnalysis(Alpacha& alpacha) {
    AssetResult assetResult = GetValidAssetWithCancel(const_cast<Alpacha&>(alpacha));
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
       
        if (validatedTime == 0) {
            // Set validatedTime to period number of days ago
            time_t currentTime = time(nullptr);
            //validatedTime = currentTime - (static_cast<long long>(period) * 24 * 60 * 60); // period days ago in seconds
           // cout << "No date entered, using date " << period << " days ago." << endl;

           
            // Calculate one year ago (365 days * 24 hours * 60 minutes * 60 seconds)
            time_t oneYearAgo = currentTime - (static_cast<time_t>(365) * 24 * 60 * 60);
			auto a = alpacha.GetMarketCalendarInfo(oneYearAgo, currentTime);

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
                double currentPrice = closingPrices[0];
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

// Handle buy/sell stock command
static void HandleOrder(const Alpacha& alpacha, int command, const Json::Value& root) {
   
    string symbol;
    cout << "Enter stock symbol: ";
    cin >> symbol;
    float amount;
    cout << "Enter the amount: ";
    cin >> amount;
    string side = (command == 3) ? "buy" : "sell";
    cout << "Placing " << side << " order for " << symbol << "..." << endl;
    Alpacha alpacha1(root.get("ALPACA_API_KEY", "").asString(), root.get("ALPACA_SECRET_KEY", "").asString());
    alpacha1.BuyStock(symbol, amount);
}


