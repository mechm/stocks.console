#include <iostream>
#include <json/json.h>

#include "../../stocks.console.api/alpacha.h"
#include "../../stocks.console.utilities/formatter.h"

#include "../include/account_details.h"

void HandleAccountDetails(const Alpacha& alpacha) {
    if (const auto [success, response] = alpacha.GetAccount(); success) {
        PrintFormattedJson(response, "Account Details");
    }
    else {
        std::cout << "Error: " << response << "\n";
    }
}

void HandleAccountPositions(const Alpacha& alpacha) {
    if (const auto [success, response] = alpacha.GetAllOpenPositions(); success) {
        PrintFormattedJson(response, "Account Positions");
    }
    else {
        std::cout << "Error: " << response << "\n";
    }
}