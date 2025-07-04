#pragma once

#include <string>
#include <vector>

#include "http.h"

using namespace std;

size_t WriteCallback(void* contents, const size_t size, size_t nmemb, void* userp);

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

// Add this structure near the top of your file, after the includes
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
};

class AlpachaAccount : public Alpacha {
public:
    AlpachaAccount(const std::string& apiKey, const std::string& secretKey, bool paper = false);

    RequestResponse GetAccount();
    RequestResponse GetAllOpenPositions();
    RequestResponse BuyStock(const std::string& symbol, double quantity);
    RequestResponse SellStock(const std::string& symbol, double quantity);
};

class AlpachaMarketData : public Alpacha {
public:
    AlpachaMarketData(const std::string& apiKey, const std::string& secretKey, bool paper = false);
    
    //AssetsResult GetAssetsAsObjects();
    RequestResponse GetAssetBySymbol(const string& symbol);
    RequestResponse GetAssetsByExchange(const string& exchange = "NASDAQ%2CNYSE");

    HistoricalBarsResult GetHistoricalBarsAsObjects(const std::string& symbol, const std::string& timeframe, const time_t start);
    RequestResponse GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start);
};

