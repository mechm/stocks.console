#pragma once

#include <string>

struct OrderResult {
    bool success = false;
    std::string response = "";
};

OrderResult GetRequest(const std::string& url, const std::string& apiKey, const std::string& secretKey);
size_t WriteCallback(void* contents, const size_t size, size_t nmemb, void* userp);

class AlpachaT {
private:
    std::string apiKey = "";
    std::string secretKey = "";
    bool paper = false;

public:
    AlpachaT(const std::string& apiKey, const std::string& secretKey, bool paper = false);
    OrderResult GetStockAuction(const std::string& symbol) const;
    OrderResult GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start) const;
    //OrderResult GetHistoricalBars(const string& symbol, const string& timeframe, const time_t start) const;
};