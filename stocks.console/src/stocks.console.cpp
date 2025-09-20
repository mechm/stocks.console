#include "../include/stock_detail.h"

#include "../include/indicator/indicator_analysis.h"
#include "../include/account_details.h"
#include "../include/order.h"

#include "../../stocks.console.api/include/alpacha.h"
#include "../../stocks.console.api/include/finn_hub.h"

#include <json/json.h>

#include <fstream>
#include <iostream>


#include <pqxx/pqxx>


int main()
{

    try {
        // // // Create a connection string
        //std::string conn_string = "dbname=stocks user=sa password=lymm host=localhost port=5432";
        //std::string conn_string = "dbname=mytestdb user=postgres password=mysecretpassword host=localhost port=5432";
        std::string conn_string = "dbname=stocks user=sa password=lymm host=localhost port=5432";
        // // //
        // // // Establish a connection
        pqxx::connection conn(conn_string);
        //std::cout << "Connected to the database." << std::endl;
        // Use a transaction object to execute a query
        pqxx::work txn(conn);

        // Execute a query
        pqxx::result r = txn.exec("SELECT id, name FROM products ORDER BY id;");

        // Iterate over the results
        for (pqxx::row row : r) {
            std::cout << "ID: " << row[0].as<int>() << ", Name: " << row[1].as<std::string>() << std::endl;
        }

        // Commit the transaction
        txn.commit();

    } catch (const pqxx::sql_error& e) {
        std::cerr << "SQL error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1;
    }



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