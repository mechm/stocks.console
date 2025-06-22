#pragma once

#include <string>

struct OrderResult {
    bool success = false;
    std::string response = "";
};

OrderResult GetRequest(const std::string& url, const std::string& apiKey, const std::string& secretKey);
size_t WriteCallback(void* contents, const size_t size, size_t nmemb, void* userp);
OrderResult PlaceOrder(const  std::string& symbol, const double quantity, const  std::string& side, const  std::string& apiKey, const  std::string& secretKey, const bool paper);

class Alpacha {
private:
    std::string apiKey = "";
    std::string secretKey = "";
    bool paper = false;

public:
    Alpacha(const std::string& apiKey, const std::string& secretKey, bool paper = false);
    OrderResult GetAccount() const;

    OrderResult GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start) const;
	OrderResult GetAssets() const;

    OrderResult BuyStock(const std::string& symbol, double quantity) const;
    OrderResult SellStock(const std::string& symbol, double quantity) const;
    OrderResult StockDetail(const std::string& symbol) const;

    OrderResult GetAllOpenPositions() const;
};