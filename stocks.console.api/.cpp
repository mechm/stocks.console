#include "pch.h"
#include "alpacha.h"
#include <time.h>
#include <json/json.h>

using namespace std;

AlpachaMarketData::AlpachaMarketData(const string& apiKey, const string& secretKey, bool paper) : Alpacha(apiKey, secretKey, paper)
{
}

RequestResponse AlpachaMarketData::GetAssetBySymbol(const string& symbol)
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/assets/" + symbol;
    return GetRequest(url);
}

RequestResponse AlpachaMarketData::GetAssetsByExchange(const string& exchange)
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/assets?exchange=" + exchange;
    return GetRequest(url);
}

HistoricalBarsResult AlpachaMarketData::GetHistoricalBarsAsObjects(const std::string& symbol, const std::string& timeframe, const time_t start) 
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

RequestResponse AlpachaMarketData::GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start)
{
    //Convert time_t to UTC ISO 8601 format string
    struct tm utc_tm;
    gmtime_s(&utc_tm, &start);
    char time_buffer[32];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%H:%M:%SZ", &utc_tm);
    const string start_time(time_buffer);

    const string url = "https://data.alpaca.markets/v2/stocks/bars?symbols=" + symbol + "&timeframe=" + timeframe + "&start=" + start_time + "&limit=1000&adjustment=raw&feed=sip&sort=asc";

    return GetRequest(url);
}

//AssetsResult AlpachaMarketData::GetAssetsAsObjects() {
//    AssetsResult result;
//
//    RequestResponse response = GetAssets();
//
//    if (!response.success) {
//        result.success = false;
//        result.error_message = "Failed to get assets from API";
//        return result;
//    }
//
//    try {
//        Json::Value root;
//        Json::Reader reader;
//
//        if (!reader.parse(response.response, root)) {
//            result.success = false;
//            result.error_message = "Failed to parse JSON response";
//            return result;
//        }
//
//        for (const auto& assetJson : root) {
//            Asset asset;
//            asset.id = assetJson.get("id", "").asString();
//            asset.class_type = assetJson.get("class", "").asString();
//            asset.exchange = assetJson.get("exchange", "").asString();
//            asset.symbol = assetJson.get("symbol", "").asString();
//            asset.name = assetJson.get("name", "").asString();
//            asset.status = assetJson.get("status", "").asString();
//            asset.tradable = assetJson.get("tradable", false).asBool();
//            asset.marginable = assetJson.get("marginable", false).asBool();
//            asset.shortable = assetJson.get("shortable", false).asBool();
//            asset.easy_to_borrow = assetJson.get("easy_to_borrow", false).asBool();
//            asset.fractionable = assetJson.get("fractionable", false).asBool();
//
//            result.assets.push_back(asset);
//        }
//
//        result.success = true;
//    }
//    catch (const exception& e) {
//        result.success = false;
//        result.error_message = "Exception while parsing assets: " + string(e.what());
//    }
//
//    return result;
//}
