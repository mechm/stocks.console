#include "../include/stock_detail.h"

#include "../include/indicator/indicator_analysis.h"
#include "../include/account_details.h"
#include "../include/order.h"

#include "../../stocks.console.api/include/alpacha.h"
#include "../../stocks.console.api/include/finn_hub.h"

#include <json/json.h>

#include <fstream>
#include <iostream>

int main()
{
    Json::Value root;
    std::ifstream config_doc("config_doc.json", std::ifstream::binary);
    config_doc >> root; 

    Alpacha alpacha(root.get("ALPACA_API_KEY", "").asString(),
        root.get("ALPACA_SECRET_KEY", "").asString());

    FinnHub finnhub(root.get("FINN_HUBB_KEY", "").asString());

    bool running = true;    

    while (running) {

        std::cout << "\n====================[ Command Menu ]====================\n"
                << "  1: Get Account Details\n"
                << "  2: Get Account Positions\n"
                << "  3: Check stock against indicator\n"
                << "  4: Buy Stock\n"
                << "  5: Sell Stock\n"
                << "  6: Get Stock Details\n"
                << "  0: Exit\n"
                << "--------------------------------------------------------\n"
                << "Enter command: ";

        int command;
        std::cin >> command;

        if (std::cin.fail()) {
            std::cout << "Enter a valid value..." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (command) {
        case 0:
            running = false;
            std::cout << "Exiting program..." << std::endl;
            break;
        case 1:
            HandleAccountDetails(alpacha);
            break;
        case 2:
            HandleAccountPositions(alpacha);
            break;
        case 3:
            HandleIndicatorAnalysis(alpacha);
            break;
        case 4:
        case 5:
            HandleOrder(alpacha, command, root);
            break;
        case 6:
            HandleStockDetail(finnhub);
            break;
        default:
            std::cout << "Unknown command: " << command << std::endl;
            break;
        }

        std::cout << std::endl; // Add a blank line between commands for better readability
    }
}