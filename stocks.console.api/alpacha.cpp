#include "pch.h"

#include <curl/curl.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <json/json.h>
#include <iomanip>

#include "alpacha.h"
#include "../stocks.console.utilities/string_utilities.h"
#include <algorithm>
#include "../stocks.console.utilities/date_utilities.h"

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

bool Alpacha::IsValidAccountResponse(const string& jsonResponse) {
        Json::Value root;
        Json::Reader reader;

        if (!reader.parse(jsonResponse, root)) {
            return false;
        }

        // Check for essential account fields
        return root.isMember("id") &&
            root.isMember("account_number") &&
            root.isMember("status");
    }

RequestResponse Alpacha::GetAccount() {
    try {
        // Build the URL
        const string baseUrl = paper ? paperApiUrl : liveApiUrl;
        const string url = baseUrl + "/account";

        // Log the request (optional)
#ifdef DEBUG
        cout << "Requesting account details from: " << (paper ? "paper" : "live") << " trading" << endl;
#endif

        // Make the request
        RequestResponse response = GetRequest(url);

        // Validate response
        if (!response.success) {
            cerr << "Failed to retrieve account information" << endl;
            return response;
        }

        // Optional: Validate JSON structure
        if (!IsValidAccountResponse(response.response)) {
            response.success = false;
            response.response = "Invalid account response format";
            return response;
        }

        return response;
    }
    catch (const exception& e) {
        RequestResponse errorResponse;
        errorResponse.success = false;
        errorResponse.response = "Exception in GetAccount: " + string(e.what());
        return errorResponse;
    }
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

#pragma region Market Opening/Close

#pragma endregion Market Opening/Close

// Helper method to get trading date N days ago
time_t Alpacha::GetTradingDateNDaysAgo(int daysAgo) {
    try {
        // Calculate start date (go back more days to account for weekends/holidays)
        time_t now = time(nullptr);

        // Calculate how far back to look based on trading days requested
        int calendarDaysToLookBack;

        if (daysAgo <= 5) {
            calendarDaysToLookBack = daysAgo + 7;
        }
        else if (daysAgo <= 22) { // ~1 month
            calendarDaysToLookBack = static_cast<int>(daysAgo * 1.4) + 5;
        }
        else if (daysAgo <= 65) { // ~3 months  
            calendarDaysToLookBack = static_cast<int>(daysAgo * 1.45) + 10;
        }
        else if (daysAgo <= 130) { // ~6 months
            calendarDaysToLookBack = static_cast<int>(daysAgo * 1.48) + 15;
        }
        else if (daysAgo <= 252) { // ~1 year
            calendarDaysToLookBack = static_cast<int>(daysAgo * 1.52) + 20;
        }
        else { // > 1 year of trading days
            // For periods longer than 1 trading year, use more conservative multiplier
            // 365 trading days ? 1.45 calendar years ? 530 calendar days
            calendarDaysToLookBack = static_cast<int>(daysAgo * 1.6) + 30;
        }

        // Additional safety check for very large requests
        if (daysAgo >= 365) {
            // Ensure we look back at least 1.5 years for 365+ trading days
            int minimumDays = static_cast<int>(daysAgo * 1.55);
            calendarDaysToLookBack = max(calendarDaysToLookBack, minimumDays);
        }

        time_t startDate = now - static_cast<long long>(calendarDaysToLookBack) * 24 * 60 * 60;
        time_t endDate = now;

        #ifdef DEBUG
                cout << "Requesting " << daysAgo << " trading days ago" << endl;
                cout << "Looking back " << calendarDaysToLookBack << " calendar days" << endl;
                cout << "Start date: " << StringUtilities::TimeToUtcIso8601(startDate) << endl;
        #endif

        // Get market calendar
        MarketCalendarResult calendarResult = GetMarketCalendarInfoAsObject(startDate, now);

        if (!calendarResult.success) {
            std::cerr << "Failed to get market calendar: " << calendarResult.errorMessage << std::endl;
            return 0;
        }

        if (calendarResult.calendar_days.empty()) {
            cerr << "No trading days found in the specified range" << endl;
            return 0;
        }

        // Sort calendar days by date (most recent first)
        std::vector<MarketDay> sortedDays = calendarResult.calendar_days;
        std::sort(sortedDays.begin(), sortedDays.end(),
            [](const MarketDay& a, const MarketDay& b) {
                return a.date > b.date; // Descending order
            });

        #ifdef DEBUG
                cout << "Found " << sortedDays.size() << " trading days in calendar" << endl;
        #endif

        // Check if we have enough trading days
        if (sortedDays.size() <= static_cast<size_t>(daysAgo - 1)) {
            cerr << "Insufficient trading days. Requested: " << daysAgo
                << ", Available: " << sortedDays.size() << endl;
            cerr << "Consider increasing the lookback period or checking API limits" << endl;
            return 0;
        }

        if (daysAgo > 0) {
            return DateUtilities::Iso8601ToTime(sortedDays[static_cast<std::vector<MarketDay, std::allocator<MarketDay>>::size_type>(daysAgo) - 1].date);
        }

        std::cerr << "Not enough trading days found in calendar" << std::endl;
        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in GetTradingDateNDaysAgo: " << e.what() << std::endl;
        return 0;
    }
}


MarketCalendarResult Alpacha::GetMarketCalendarInfoAsObject(const time_t& start, const time_t& end) {
    MarketCalendarResult result;
    result.success = false;

    try {
        // Get the raw JSON response
        RequestResponse apiResult = GetMarketCalendarInfo(start, end);

        if (!apiResult.success) {
            result.errorMessage = "API request failed: " + apiResult.response;
            return result;
        }

        // Parse JSON response
        Json::Value root;
        Json::Reader reader;

        if (!reader.parse(apiResult.response, root)) {
            result.errorMessage = "Failed to parse JSON response";
            return result;
        }

        // Check if root is an array (market calendar returns an array of days)
        if (!root.isArray()) {
            result.errorMessage = "Expected array response for market calendar";
            return result;
        }

        // Parse each market day
        for (const auto& dayJson : root) {
            MarketDay day;

            if (dayJson.isMember("date")) {
                day.date = dayJson["date"].asString();
            }
            if (dayJson.isMember("open")) {
                day.open = dayJson["open"].asString();
            }
            if (dayJson.isMember("close")) {
                day.close = dayJson["close"].asString();
            }
            if (dayJson.isMember("session_open")) {
                day.session_open = dayJson["session_open"].asString();
            }
            if (dayJson.isMember("session_close")) {
                day.session_close = dayJson["session_close"].asString();
            }

            result.calendar_days.push_back(day);
        }

        result.success = true;
    }
    catch (const std::exception& e) {
        result.success = false;
        result.errorMessage = "Exception occurred: " + std::string(e.what());
    }

    return result;
}

RequestResponse Alpacha::GetMarketCalendarInfo(const time_t& start, const time_t& end)
{
    std::string url = (paper ? paperApiUrl : liveApiUrl) + "/calendar";
    std::string query;

    if (start != 0) {
        query += "start=" + StringUtilities::TimeToUtcIso8601(start);
    }
    if (end != 0) {
        if (!query.empty()) query += "&";
        query += "end=" + StringUtilities::TimeToUtcIso8601(end);
    }
    if (!query.empty()) {
        url += "?" + query;
    }
    return GetRequest(url);
}

















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
        if (bar.isMember("v")) barData.volume = static_cast<long>(bar["v"].asInt64());
        if (bar.isMember("vw")) barData.vwap = bar["vw"].asDouble();
        if (bar.isMember("n")) barData.trade_count = bar["n"].asInt();

        result.bars.push_back(barData);
    }

    result.success = true;
    return result;
}

RequestResponse Alpacha::GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start)
{
    string start_time = StringUtilities::TimeToUtcIso8601(start);

    const string url = liveMarketDataApiUrl+"/stocks/bars?symbols=" + symbol + "&timeframe=" + timeframe + "&start=" + start_time + "&limit=1000&adjustment=raw&feed=sip&sort=asc";

    return GetRequest(url);
}



#pragma endregion Market Data

