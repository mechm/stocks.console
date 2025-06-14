#pragma once

#include <string>

struct OrderResult {
    bool success = false;
    std::string response = "";
};

class Alpacha {
private:
    std::string apiKey = "";
    std::string secretKey = "";
    bool paper = false;

public:
    Alpacha(const std::string &apiKey, const std::string &secretKey, bool paper = false);
    OrderResult BuyStock(const std::string &symbol, double quantity) const;
    OrderResult SellStock(const std::string &symbol, double quantity) const;
    OrderResult StockDetail(const std::string &symbol) const;
    OrderResult GetAccount() const;
    OrderResult GetAllOpenPositions() const;
};