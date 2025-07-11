#pragma once

#include <string>
#include <vector>

#include "http.h"

using namespace std;

struct BarData {
    std::string timestamp;
    double open;
    double high;
    double low;
    double close;
    long volume;
    double vwap;
    int trade_count;
};

struct HistoricalBarsResult {
    bool success;
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
    bool tradable;
    bool marginable;
    bool shortable;
    bool easy_to_borrow;
    bool fractionable;
};

struct AssetsResult {
    bool success;
    std::vector<Asset> assets;
    std::string error_message;
};

struct AssetResult {
    bool success = false;
    Asset asset;
    string errorMessage;
};

class Alpacha : public Http {
protected:
    std::string apiKey = "";
    std::string secretKey = "";
    bool paper = false;
    const std::string paperApiUrl = "https://paper-api.alpaca.markets/v2";
    const std::string liveApiUrl = "https://api.alpaca.markets/v2";   

public:
    Alpacha(const string& apiKey, const std::string& secretKey, bool paper = false);
    RequestResponse GetRequest(const std::string& url, const std::string& payload = "");

    RequestResponse GetAccount();
    RequestResponse GetAllOpenPositions();
    RequestResponse BuyStock(const std::string& symbol, double quantity);
    RequestResponse SellStock(const std::string& symbol, double quantity);


    RequestResponse GetAssetBySymbol(const string& symbol);
    AssetResult GetAssetBySymbolAsObject(const string& symbol);
    RequestResponse GetAssetsByExchange(const string& exchange = "NASDAQ%2CNYSE");
    HistoricalBarsResult GetHistoricalBarsAsObjects(const std::string& symbol, const std::string& timeframe, const time_t start);
    RequestResponse GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start);
};