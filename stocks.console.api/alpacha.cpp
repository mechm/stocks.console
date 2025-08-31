#include "pch.h"

#include "alpacha.h"

#include "../stocks.console.utilities/string_utilities.h"
#include "../stocks.console.utilities/date_utilities.h"

#include <curl/curl.h>
#include <json/json.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>

Alpacha::Alpacha(const std::string& apiKey, const std::string& secretKey, bool paper)
{    
    if (apiKey.empty() || secretKey.empty()) {
        std::cout << "Error: API credentials not set." << std::endl;
        exit(1);
    }

    this->apiKey = apiKey;
    this->secretKey = secretKey;
    this->paper = paper;
}

RequestResponse Alpacha::GetRequest(const std::string& url, const std::string& payload) const {
    const std::vector<std::string> headers = {
        "accept: application/json",
        "content-type: application/json",
        "APCA-API-KEY-ID: " + apiKey,
        "APCA-API-SECRET-KEY: " + secretKey
    };

    return Http::GetRequest(url, headers, payload);
}

#pragma region Account

bool Alpacha::IsValidAccountResponse(const std::string& jsonResponse) {
        Json::Value root;

        if (Json::Reader reader; !reader.parse(jsonResponse, root)) {
            return false;
        }

        // Check for essential account fields
        return root.isMember("id") &&
            root.isMember("account_number") &&
            root.isMember("status");
    }

RequestResponse Alpacha::GetAccount() const {
    try {
        // Build the URL
        const std::string baseUrl = paper ? paperApiUrl : liveApiUrl;
        const std::string url = baseUrl + "/account";

        // Log the request (optional)
#ifdef DEBUG
        cout << "Requesting account details from: " << (paper ? "paper" : "live") << " trading" << endl;
#endif

        // Make the request
        RequestResponse response = GetRequest(url);

        // Validate response
        if (!response.success) {
            std::cerr << "Failed to retrieve account information" << std::endl;
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
    catch (const std::exception& e) {
        RequestResponse errorResponse;
        errorResponse.success = false;
        errorResponse.response = "Exception in GetAccount: " + std::string(e.what());
        return errorResponse;
    }
}

RequestResponse Alpacha::GetAllOpenPositions() const {
    const std::string url = (paper ? paperApiUrl : liveApiUrl) + "/positions";
    return GetRequest(url);
}

RequestResponse Alpacha::BuyStock(const std::string& symbol, const double quantity) const {
    const std::string url = (paper ? paperApiUrl : liveApiUrl) + "/orders";
    const std::string payload = R"({"symbol":")" + symbol + R"(","qty":)" + std::to_string(quantity) + R"(, "side":")"
        + "buy" + R"(","type":"market","time_in_force":"day"})";
    return GetRequest(url, payload);
}

RequestResponse Alpacha::SellStock(const std::string& symbol, const double quantity) const {
    const std::string url = (paper ? paperApiUrl : liveApiUrl) + "/orders";
    const std::string payload = R"({"symbol":")" + symbol + R"(","qty":)" + std::to_string(quantity) + R"(, "side":")"
        + "sell" + R"(","type":"market","time_in_force":"day"})";
    return GetRequest(url, payload);
}

#pragma endregion Account

#pragma region Market Opening/Close



// Helper method to get trading date N days ago
time_t Alpacha::GetTradingDateNDaysAgo(int daysAgo) {
    try {
        // Calculate start date (go back more days to account for weekends/holidays)
        const time_t now = time(nullptr);

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
            const int minimumDays = static_cast<int>(daysAgo * 1.55);
            calendarDaysToLookBack = max(calendarDaysToLookBack, minimumDays);
        }

        time_t startDate = now - static_cast<long long>(calendarDaysToLookBack) * 24 * 60 * 60;

#ifdef DEBUG
                cout << "Requesting " << daysAgo << " trading days ago" << endl;
                cout << "Looking back " << calendarDaysToLookBack << " calendar days" << endl;
                cout << "Start date: " << StringUtilities::TimeToUtcIso8601(startDate) << endl;
        #endif

        // Get market calendar
        auto [success, errorMessage, calendar_days] = GetMarketCalendarInfoAsObject(startDate, now);

        if (!success) {
            std::cerr << "Failed to get market calendar: " << errorMessage << std::endl;
            return 0;
        }

        if (calendar_days.empty()) {
            std::cerr << "No trading days found in the specified range" << std::endl;
            return 0;
        }

        // Sort calendar days by date (most recent first)
        std::vector<MarketDay> sortedDays = calendar_days;
        std::ranges::sort(sortedDays,
                     [](const MarketDay& a, const MarketDay& b) {
                         return a.date > b.date; // Descending order
                     });

        #ifdef DEBUG
                cout << "Found " << sortedDays.size() << " trading days in calendar" << endl;
        #endif

        // Check if we have enough trading days
        if (sortedDays.size() <= static_cast<size_t>(daysAgo - 1)) {
            std::cerr << "Insufficient trading days. Requested: " << daysAgo
                << ", Available: " << sortedDays.size() << std::endl;
            std::cerr << "Consider increasing the lookback period or checking API limits" << std::endl;
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
        auto [success, response] = GetMarketCalendarInfo(start, end);

        if (!success) {
            result.errorMessage = "API request failed: " + response;
            return result;
        }

        // Parse JSON response
        Json::Value root;

        if (Json::Reader reader; !reader.parse(response, root)) {
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

RequestResponse Alpacha::GetMarketCalendarInfo(const time_t& start, const time_t& end) const {
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

#pragma endregion Market Opening/Close

#pragma region Market Data

RequestResponse Alpacha::GetAssetBySymbol(const std::string& symbol) const {
    const std::string url = (paper ? paperApiUrl : liveApiUrl) + "/assets/" + symbol;
    return GetRequest(url);
}

RequestResponse Alpacha::GetAssetByExchange(const std::string& exchange) const {
    const std::string url = (paper ? paperApiUrl : liveApiUrl) + "/assets?status=active&exchange=" + exchange;
    return GetRequest(url);
}

AssetResult Alpacha::GetAssetBySymbolAsObject(const std::string& symbol) const {
    AssetResult result;

    try {
        if (auto [success, response] = GetAssetBySymbol(symbol); success) {
            Json::Value assetJson;

            if (Json::Reader reader; reader.parse(response, assetJson)) {
                // Parse JSON into Asset object
                result.asset.symbol = assetJson.get("symbol", "").asString();
                result.asset.name = assetJson.get("name", "").asString();                
                result.asset.exchange = assetJson.get("exchange", "").asString();
                result.asset.tradable = assetJson.get("tradable", false).asBool();
                result.asset.status = assetJson.get("status", "").asString();

                result.success = true;
            }
            else {
                result.success = false;
                result.errorMessage = "Failed to parse JSON response: " + response;
            }
        }
        else {
            result.success = false;
            result.errorMessage = "API call failed: " + response;
        }
    }
    catch (const std::exception& e) {
        result.success = false;
        result.errorMessage = "Exception occurred: " + std::string(e.what());
    }

    return result;
}


RequestResponse Alpacha::GetAssetsByExchange(const std::string& exchange) const {
    const std::string url = (paper ? paperApiUrl : liveApiUrl) + "/assets?exchange=" + exchange;
    return GetRequest(url);
}

HistoricalBarsResult Alpacha::GetHistoricalBarsAsObjects(const std::string& symbol, const std::string& timeframe, const time_t start) const
{
    HistoricalBarsResult result;
    result.success = false;
    result.symbol = symbol;

    // Get the raw JSON response
    auto [success, response] = GetHistoricalBars(symbol, timeframe, start);

    if (!success) {
        result.error_message = "API request failed";
        return result;
    }

    // Parse JSON response
    Json::Value root;

    if (Json::Reader reader; !reader.parse(response, root)) {
        result.error_message = "Failed to parse JSON response";
        return result;
    }

    // Check if the response contains bars for the symbol
    if (!root.isMember("bars") || !root["bars"].isMember(symbol)) {
        result.error_message = "No bar data found for symbol: " + symbol;
        return result;
    }

    // Parse each bar
    for (const Json::Value& bars = root["bars"][symbol]; const auto& bar : bars) {
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

HistoricalClosedPrices Alpacha::GetHistoricalClosedPrices(const std::string& symbol, const std::string& timeframe, const time_t start) const
{
    HistoricalClosedPrices result;
    result.success = false;
    result.symbol = symbol;

    // Get the raw JSON response
    auto [success, response] = GetHistoricalBars(symbol, timeframe, start);

    if (!success) {
        result.error_message = "API request failed";
        return result;
    }

    // Parse JSON response
    Json::Value root;

    if (Json::Reader reader; !reader.parse(response, root)) {
        result.error_message = "Failed to parse JSON response";
        return result;
    }

    // Check if the response contains bars for the symbol
    if (!root.isMember("bars") || !root["bars"].isMember(symbol)) {
        result.error_message = "No bar data found for symbol: " + symbol;
        return result;
    }

    // Parse each bar
    for (const Json::Value& bars = root["bars"][symbol]; const auto& bar : bars) {
        result.prices.push_back(bar["c"].asDouble());
    }

    result.success = true;
    return result;
}

RequestResponse Alpacha::GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start) const {
    const std::string start_time = StringUtilities::TimeToUtcIso8601(start);

    const std::string url = liveMarketDataApiUrl+"/stocks/bars?symbols=" + symbol + "&timeframe=" + timeframe + "&start=" + start_time + "&limit=1000&adjustment=raw&feed=sip&sort=asc";

    return GetRequest(url);
}

#pragma endregion Market Data