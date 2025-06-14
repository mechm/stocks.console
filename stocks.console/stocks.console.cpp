#include <iostream>
using namespace std;

#include <vector>
#include <fstream>
#include <json/json.h>

#include "sma.h"
#include "alpacha.h"

int main()
{
    Json::Value root;   // 'root' will contain the root value after parsing.
    std::ifstream config_doc("config_doc.json", std::ifstream::binary);
    config_doc >> root;

    // Get API credentials from config file
    std::string api_key = root.get("ALPACA_API_KEY", "").asString();
    std::string api_secret = root.get("ALPACA_SECRET_KEY", "").asString();

    if (api_key.empty() || api_secret.empty()) {
        cout << "Error: API credentials not found in .config file." << endl;
        return 1;
    }

    bool running = true;

    while (running) {

        cout << "Enter command (1: Get Account Details, 2: Calculate SMA, 3: Buy Stock, 4: Sell Stock, 0: Exit): ";
        string command;
        cin >> command;
        
        if (command == "0") {
            running = false;
            cout << "Exiting program..." << endl;
            continue;
        }

        if (command == "1") {

            Alpacha alpacha(api_key, api_secret);

            OrderResult account = alpacha.GetAccount();
            if (account.success)
            {
                cout << "Account details: " << account.response << endl;
            }
        }
        if (command == "2") {
            // Sample closing prices
            vector<double> closingPrices = { 10.5, 12.3, 11.8, 13.2, 14.5, 12.9 };

            // Calculate the 3-period SMA
            double sma = calculateSMA(closingPrices, 3);

            cout << "SMA: " << sma << endl;
        }
        else if (command == "3" || command == "4") {
            string symbol;

            cout << "Enter stock symbol: ";
            cin >> symbol;

            // check valid stock symbol

            float amount;
            cout << "Enter the amount: ";
            cin >> amount;

            // check valid amount

            string side = (command == "3") ? "buy" : "sell";

            cout << "Placing " << side << " order for " << symbol << "..." << endl;
           
            Alpacha alpacha(api_key, api_secret);
            alpacha.BuyStock(symbol, amount);
        }
        else {
            cout << "Unknown command: " << command << endl;
        }
        cout << endl; // Add a blank line between commands for better readability
    }
    return 0;
}