#include <iostream>
#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "../stocks.console.utilities/formatter.h"

#include "account_details.h"

void HandleAccountDetails(Alpacha& alpacha) {
    if (const auto [success, response] = alpacha.GetAccount(); success) {
        PrintFormattedJson(response, "Account Details");
    }
    else {
        std::cout << "Error: " << response << "\n";
    }
}