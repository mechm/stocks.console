#include "pch.h"

#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "alpacha.h"

using namespace std;

Alpacha::Alpacha(const string& apiKey, const string& secretKey, bool paper)
{    
    if (apiKey.empty() || secretKey.empty()) {
        cout << "Error: API credentials not set." << endl;
        exit(1);
    }

    this->apiKey = apiKey;
    this->secretKey = secretKey;
    this->paper = paper;
}

RequestResponse Alpacha::GetRequest(const string& url, const string& payload)
{
    vector<string> headers = {
        "accept: application/json",
        "content-type: application/json",
        "APCA-API-KEY-ID: " + apiKey,
        "APCA-API-SECRET-KEY: " + secretKey
    };

    return Http::GetRequest(url, headers, payload);
}

#pragma region Account

RequestResponse Alpacha::GetAccount()
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/account";
    return GetRequest(url);
}

RequestResponse Alpacha::GetAllOpenPositions()
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/positions";
    return GetRequest(url);
}

RequestResponse Alpacha::BuyStock(const string& symbol, const double quantity)
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/orders";
    const string payload = "{\"symbol\":\"" + symbol + "\",\"qty\":" + to_string(quantity) + ", \"side\":\""
        + "buy" + "\",\"type\":\"market\",\"time_in_force\":\"day\"}";
    return GetRequest(url, payload);
}

RequestResponse Alpacha::SellStock(const string& symbol, const double quantity)
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/orders";
    const string payload = "{\"symbol\":\"" + symbol + "\",\"qty\":" + to_string(quantity) + ", \"side\":\""
        + "sell" + "\",\"type\":\"market\",\"time_in_force\":\"day\"}";
    return GetRequest(url, payload);
}

#pragma endregion Account

#pragma region Market Data



RequestResponse Alpacha::GetAssetBySymbol(const string& symbol) 
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/assets/" + symbol;
    return GetRequest(url);
}

AssetResult Alpacha::GetAssetBySymbolAsObject(const string& symbol) {
    AssetResult result;

    try {
        RequestResponse response = GetAssetBySymbol(symbol);

        if (response.success) {
            Json::Value assetJson;
            Json::Reader reader;

            if (reader.parse(response.response, assetJson)) {
                // Parse JSON into Asset object
                result.asset.symbol = assetJson.get("symbol", "").asString();
                result.asset.name = assetJson.get("name", "").asString();
               // result.asset.assetClass = assetJson.get("class", "").asString();
                result.asset.exchange = assetJson.get("exchange", "").asString();
                result.asset.tradable = assetJson.get("tradable", false).asBool();
                result.asset.status = assetJson.get("status", "").asString();

                result.success = true;
            }
            else {
                result.success = false;
                result.errorMessage = "Failed to parse JSON response: " + response.response;
            }
        }
        else {
            result.success = false;
            result.errorMessage = "API call failed: " + response.response;
        }
    }
    catch (const exception& e) {
        result.success = false;
        result.errorMessage = "Exception occurred: " + string(e.what());
    }

    return result;
}


RequestResponse Alpacha::GetAssetsByExchange(const string& exchange)
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/assets?exchange=" + exchange;
    return GetRequest(url);
}

HistoricalBarsResult Alpacha::GetHistoricalBarsAsObjects(const std::string& symbol, const std::string& timeframe, const time_t start)
{
    HistoricalBarsResult result;
    result.success = false;
    result.symbol = symbol;

    // Get the raw JSON response
    RequestResponse apiResult = GetHistoricalBars(symbol, timeframe, start);

    if (!apiResult.success) {
        result.error_message = "API request failed";
        return result;
    }

    // Parse JSON response
    Json::Value root;
    Json::Reader reader;

    if (!reader.parse(apiResult.response, root)) {
        result.error_message = "Failed to parse JSON response";
        return result;
    }

    // Check if the response contains bars for the symbol
    if (!root.isMember("bars") || !root["bars"].isMember(symbol)) {
        result.error_message = "No bar data found for symbol: " + symbol;
        return result;
    }

    const Json::Value& bars = root["bars"][symbol];

    // Parse each bar
    for (const auto& bar : bars) {
        BarData barData;

        if (bar.isMember("t")) barData.timestamp = bar["t"].asString();
        if (bar.isMember("o")) barData.open = bar["o"].asDouble();
        if (bar.isMember("h")) barData.high = bar["h"].asDouble();
        if (bar.isMember("l")) barData.low = bar["l"].asDouble();
        if (bar.isMember("c")) barData.close = bar["c"].asDouble();
        if (bar.isMember("v")) barData.volume = bar["v"].asInt64();
        if (bar.isMember("vw")) barData.vwap = bar["vw"].asDouble();
        if (bar.isMember("n")) barData.trade_count = bar["n"].asInt();

        result.bars.push_back(barData);
    }

    result.success = true;
    return result;
}

RequestResponse Alpacha::GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start)
{
    //Convert time_t to UTC ISO 8601 format string
    struct tm utc_tm;
    gmtime_s(&utc_tm, &start);
    char time_buffer[32];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%H:%M:%SZ", &utc_tm);
    const string start_time(time_buffer);

    const string url = liveMarketDataApiUrl+"/stocks/bars?symbols=" + symbol + "&timeframe=" + timeframe + "&start=" + start_time + "&limit=1000&adjustment=raw&feed=sip&sort=asc";

    return GetRequest(url);
}



#pragma endregion Market Data