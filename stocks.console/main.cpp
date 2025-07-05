#include <iostream>
using namespace std;

#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "../stocks.console.indicator/sma.h"
#include "string_utilities.h"

int main()
{
    Json::Value root;   // 'root' will contain the root value after parsing.
    std::ifstream config_doc("config_doc.json", std::ifstream::binary);
    config_doc >> root;    

    bool running = true;

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
            continue;
        }

        if (command == 0) {
            running = false;
            cout << "Exiting program..." << endl;
            continue;
        }

        // check account details
        if (command == 1) {
            Alpacha alpacha(root.get("ALPACA_API_KEY", "").asString(), 
                            root.get("ALPACA_SECRET_KEY", "").asString());

            RequestResponse account = alpacha.GetAccount();
            if (account.success)
            {
                cout << "Account details: " << account.response << endl;
            }
        }

        // run stock symbol against an indicator
        if (command == 2) {
            string symbol;
            int indicator;
            string date;

            cout << "Enter stock symbol: ";
            cin >> symbol;
            
            //// check valid stock symbol
            //Alpacha alpacha(root.get("ALPACA_API_KEY", "").asString(),
            //    root.get("ALPACA_SECRET_KEY", "").asString());

       /*     AssetsResult account = Alpacha::GetAssetsAsObjects();
            if (account.success)
            {
                cout << "Account details: " << account.response << endl;
            }*/

            //RequestResponse a = AlpachaAccount::GetAllOpenPositions();
            //alpacha.GetAllOpenPositions();
            

            //Alpacha::GetAssets();

            cout << "Choose indicator, 1) SMA, 2) RSI, 3) MACD: ";
            cin >> indicator;

            if (cin.fail()) {
                cout << "Enter a valid selection..." << endl;
                continue;
            }

            if (indicator == 1) {

                cout << "Choose date (YYYY-MM-DD format), or leave blank: ";
                cin >> date;

                time_t validatedTime;
                if (!StringUtilities::ValidateDate(date, validatedTime)) {
                    cout << "Please enter a valid date and try again." << endl;
                    continue; // Go back to main menu
                }

                Alpacha alpacha1(root.get("ALPACA_API_KEY", "").asString(),
                    root.get("ALPACA_SECRET_KEY", "").asString());

                HistoricalBarsResult historicPrices = 
                    alpacha1.GetHistoricalBarsAsObjects(symbol, "1D", validatedTime);

                if (historicPrices.success && !historicPrices.bars.empty()) {              
                    vector<double> closingPrices;
                    for (const auto& bar : historicPrices.bars) {
                        closingPrices.push_back(bar.close);
                    }

                    int period = 3; // You might want to make this configurable
                    if (closingPrices.size() >= period) {
                        //double sma = calculateSMA(closingPrices, period);
                        //cout << "SMA (" << period << "-period) for " << symbol << ": " << fixed << setprecision(2) << sma << endl;

                        //


                        //int period = 10;
                        //double threshold = 1.0; // 1% threshold
                        //// Calculate SMA
                        //double sma = calculateSMA(closingPrices, period);
                        //double currentPrice = closingPrices[0];

                        //// Get signal
                        //int signal = getSMASignal(currentPrice, sma, threshold);

                        //// Print analysis
                        //printSMAAnalysis(currentPrice, sma, signal);

                        //// Alternative: Get signal with history
                        ////int signalWithHistory = getSMASignalWithHistory(closingPrices, period, threshold);





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

