#pragma once

#include <string>
#include <iomanip>

class CurrencyInput {
public:
    static double GetCurrencyAmount(const std::string& prompt = "Enter the amount: ");
    static double GetCurrencyAmountWithRange(const std::string& prompt, double minAmount, double maxAmount);
    static bool ValidateCurrencyFormat(const std::string& input);
    static std::string FormatCurrency(double amount);
    static double ParseCurrencyString(const std::string& input);

private:
    static bool IsValidCurrencyCharacter(char c);
    static std::string CleanCurrencyInput(const std::string& input);
};