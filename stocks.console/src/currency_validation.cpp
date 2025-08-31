#include "../include/currency_validation.h"
#include <regex>
#include <sstream>
#include <iostream>

using namespace std;

double CurrencyInput::GetCurrencyAmount(const string& prompt) {
    double amount = 0.0;
    string input;
    bool validInput = false;

    while (!validInput) {
        cout << prompt;
        getline(cin, input);

        // Handle empty input
        if (input.empty()) {
            cout << "Error: Please enter an amount." << endl;
            continue;
        }

        // Clean and validate the input
        if (string cleanInput = CleanCurrencyInput(input); ValidateCurrencyFormat(cleanInput)) {
            try {
                amount = ParseCurrencyString(cleanInput);

                // Validate reasonable range
                if (amount < 0) {
                    cout << "Error: Amount cannot be negative." << endl;
                    continue;
                }

                if (amount > 1000000.0) {
                    cout << "Error: Amount too large (max $1,000,000.00)." << endl;
                    continue;
                }

                // Round to 2 decimal places
                amount = round(amount * 100.0) / 100.0;

                // Confirm the amount
                cout << "Confirmed amount: " << FormatCurrency(amount) << endl;
                validInput = true;

            }
            catch ([[maybe_unused]] const exception& e) {
                cout << "Error: Invalid amount format. Please try again." << endl;
            }
        }
        else {
            cout << "Error: Invalid currency format. Please enter a valid amount (e.g., 123.45, $123.45, or 123)." << endl;
            cout << "Examples: 100, 100.50, $100.50, 1,234.56" << endl;
        }
    }

    return amount;
}

double CurrencyInput::GetCurrencyAmountWithRange(const string& prompt, const double minAmount, const double maxAmount) {
    double amount = 0.0;
    string input;
    bool validInput = false;

    cout << "Valid range: " << FormatCurrency(minAmount) << " - " << FormatCurrency(maxAmount) << endl;

    while (!validInput) {
        cout << prompt;
        getline(cin, input);

        if (input.empty()) {
            cout << "Error: Please enter an amount." << endl;
            continue;
        }

        if (string cleanInput = CleanCurrencyInput(input); ValidateCurrencyFormat(cleanInput)) {
            try {
                amount = ParseCurrencyString(cleanInput);
                amount = round(amount * 100.0) / 100.0; // Round to 2 decimal places

                if (amount < minAmount) {
                    cout << "Error: Amount too small. Minimum: " << FormatCurrency(minAmount) << endl;
                    continue;
                }

                if (amount > maxAmount) {
                    cout << "Error: Amount too large. Maximum: " << FormatCurrency(maxAmount) << endl;
                    continue;
                }

                cout << "Confirmed amount: " << FormatCurrency(amount) << endl;
                validInput = true;

            }
            catch ([[maybe_unused]] const exception& e) {
                cout << "Error: Invalid amount format. Please try again." << endl;
            }
        }
        else {
            cout << "Error: Invalid currency format." << endl;
            cout << "Valid examples: " << FormatCurrency(minAmount) << ", " << FormatCurrency(maxAmount) << endl;
        }
    }

    return amount;
}

bool CurrencyInput::ValidateCurrencyFormat(const string& input) {
    if (input.empty()) return false;

    // Regex pattern for currency validation
    // Matches: 123, 123.45, 1,234.56, etc.
    const regex currencyPattern(R"(^\d{1,3}(?:,\d{3})*(?:\.\d{1,2})?$|^\d+(?:\.\d{1,2})?$)");

    return regex_match(input, currencyPattern);
}

string CurrencyInput::FormatCurrency(const double amount) {
    stringstream ss;
    ss << "$" << fixed << setprecision(2) << amount;

    string result = ss.str();

    // Add commas for thousands separator
    size_t decimalPos = result.find('.');
    if (decimalPos == string::npos) decimalPos = result.length();

    for (int i = decimalPos - 3; i > 1; i -= 3) {
        result.insert(i, ",");
    }

    return result;
}

double CurrencyInput::ParseCurrencyString(const string& input) {
    string cleanInput = input;

    // Remove commas
    erase(cleanInput, ',');

    // Convert to double
    return stod(cleanInput);
}

string CurrencyInput::CleanCurrencyInput(const string& input) {
    string cleaned = input;

    // Remove leading/trailing whitespace
    cleaned.erase(0, cleaned.find_first_not_of(" \t"));
    cleaned.erase(cleaned.find_last_not_of(" \t") + 1);

    // Remove dollar sign if present
    if (!cleaned.empty() && cleaned[0] == '$') {
        cleaned = cleaned.substr(1);
    }

    return cleaned;
}

bool CurrencyInput::IsValidCurrencyCharacter(const char c) {
    return isdigit(c) || c == '.' || c == ',' || c == '$';
}
