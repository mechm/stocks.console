#include <iostream>
#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "../stocks.console.api/alpacha.h"
#include "../stocks.console.utilities/formatter.h"

#include "account_details.h"

void HandleAccountDetails(Alpacha& alpacha) {
    RequestResponse account = alpacha.GetAccount();
    if (account.success) {
        PrintFormattedJson(account.response, "Account Details");
    }
    else {
        std::cout << "Error: " << account.response << "\n";
    }
}