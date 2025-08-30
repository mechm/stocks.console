#pragma once

#include <string>
#include <vector>

#include "http.h"

struct BarData {
    std::string timestamp;
    double open = 0.0;
    double high = 0.0;
    double low = 0.0;
    double close = 0.0;
    long volume = 0;
    double vwap = 0.0;
    int trade_count = 0;
};

struct HistoricalClosedPrices {
    bool success = false;
    std::string symbol;
    std::vector<double> prices;
    std::string error_message;
};

struct HistoricalBarsResult {  
    bool success = false;
    std::string symbol;
    std::vector<BarData> bars;  
    std::string error_message;
};

struct Asset {  
    std::string id;  
    std::string class_type;  
    std::string exchange;  
    std::string symbol;  
    std::string name;  
    std::string status;  
    bool tradable = false;  
    bool marginable = false;  
    bool shortable = false;  
    bool easy_to_borrow = false;  
    bool fractionable = false;  
};

struct AssetsResult {
    bool success;
    std::vector<Asset> assets;
    std::string error_message;
};

struct AssetResult {
    bool success = false;
    Asset asset;
    std::string errorMessage;
};

struct MarketDay {
    std::string date;
    std::string open;
    std::string close;
    std::string session_open;
    std::string session_close;
};

struct MarketCalendarResult {
    bool success = false;
    std::string errorMessage;
    std::vector<MarketDay> calendar_days;
};

class Alpacha : public Http {
private:
    static bool IsValidAccountResponse(const std::string & jsonResponse);

protected:
    std::string apiKey;
    std::string secretKey;
    bool paper = false;
    const std::string paperApiUrl = "https://paper-api.alpaca.markets/v2";
    const std::string liveApiUrl = "https://api.alpaca.markets/v2";
    const std::string liveMarketDataApiUrl = "https://data.alpaca.markets/v2";

public:
    Alpacha(const std::string& apiKey, const std::string& secretKey, bool paper = false);
    RequestResponse GetRequest(const std::string& url, const std::string& payload = "") const;

    RequestResponse GetAccount() const;
    RequestResponse GetAllOpenPositions() const;
    RequestResponse BuyStock(const std::string& symbol, double quantity) const;
    RequestResponse SellStock(const std::string& symbol, double quantity) const;

    time_t GetTradingDateNDaysAgo(int daysAgo);
    MarketCalendarResult GetMarketCalendarInfoAsObject(const time_t& start, const time_t& end);
    RequestResponse GetMarketCalendarInfo(const time_t& start, const time_t& end);

    RequestResponse GetAssetBySymbol(const std::string& symbol) const;
    RequestResponse GetAssetByExchange(const std::string& exchange) const;
    AssetResult GetAssetBySymbolAsObject(const std::string& symbol) const;
    RequestResponse GetAssetsByExchange(const std::string& exchange = "NASDAQ%2CNYSE") const;
    HistoricalClosedPrices GetHistoricalClosedPrices(const std::string& symbol, const std::string& timeframe, const time_t start) const;
    HistoricalBarsResult GetHistoricalBarsAsObjects(const std::string& symbol, const std::string& timeframe, const time_t start) const;
    RequestResponse GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start) const;
};