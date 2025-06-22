#include "pch.h"

#include "alpacha.h"

#include <curl/curl.h>
#include <iostream>
#include <ctime>
#include <format>


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

OrderResult Alpacha::GetAccount() const
{
    const string url = paper ? "https://paper-api.alpaca.markets/v2/account" : 
                               "https://api.alpaca.markets/v2/account";

    return GetRequest(url, apiKey, secretKey);
}

OrderResult Alpacha::GetAllOpenPositions() const
{
    const string url = paper ? "https://paper-api.alpaca.markets/v2/positions" : "https://api.alpaca.markets/v2/positions";
    return GetRequest(url, apiKey, secretKey);
}

OrderResult Alpacha::StockDetail(const string& symbol) const
{
    const string url = paper ? "https://paper-api.alpaca.markets/v2/assets/" + symbol : "https://api.alpaca.markets/v2/assets/" + symbol;
    return GetRequest(url, apiKey, secretKey);
}

OrderResult Alpacha::BuyStock(const string& symbol, const double quantity) const
{
    return PlaceOrder(symbol, quantity, "buy", apiKey, secretKey, paper);
}

OrderResult Alpacha::SellStock(const string& symbol, const double quantity) const
{
    return PlaceOrder(symbol, quantity, "sell", apiKey, secretKey, paper);
}


OrderResult Alpacha::GetHistoricalBars(const std::string& symbol, const std::string& timeframe, const time_t start) const
{
    //Convert time_t to UTC ISO 8601 format string
    struct tm utc_tm;
    gmtime_s(&utc_tm, &start);
    char time_buffer[32];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%dT%H:%M:%SZ", &utc_tm);
    string start_time(time_buffer);

    const string url ="https://data.alpaca.markets/v2/stocks/bars?symbols=" + symbol + "&timeframe=" + timeframe + "&start=" + start_time + "&limit=1000&adjustment=raw&feed=sip&sort=asc";  

    return GetRequest(url, apiKey, secretKey);
}

OrderResult Alpacha::GetAssets() const
{
    const string url = paper ? "https://paper-api.alpaca.markets/v2/assets?exchange=NASDAQ%2CNYSE" : "https://api.alpaca.markets/v2/assets?exchange=NASDAQ%2CNYSE";

    return GetRequest(url, apiKey, secretKey);
}

static OrderResult PlaceOrder(const string& symbol, const double quantity, const string& side, const string& apiKey, const string& secretKey, const bool paper)
{
    OrderResult result;
    result.success = false;

    string url = paper ? "https://paper-api.alpaca.markets/v2/orders" : "https://api.alpaca.markets/v2/orders";

    string payload = "{\"symbol\":\"" + symbol + "\",\"qty\":" + to_string(quantity) + ", \"side\":\""
        + side + "\",\"type\":\"market\",\"time_in_force\":\"day\"}";

    CURL* curl = nullptr;
    CURLcode res{};
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        // Create a list of headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "accept: application/json");
        headers = curl_slist_append(headers, "content-type: application/json");
        headers = curl_slist_append(headers, ("APCA-API-KEY-ID: " + apiKey).c_str());
        headers = curl_slist_append(headers, ("APCA-API-SECRET-KEY: " + secretKey).c_str());

        // Set URL, POST data and headers
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set up response handling
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);
        result.success = (res == CURLE_OK);
        result.response = readBuffer;

        if (result.success) {
            std::cout << "Request successful. Response: " << readBuffer << std::endl;
        }
        else {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return result;
}

static OrderResult GetRequest(const string& url, const string& apiKey, const string& secretKey)
{
    OrderResult result;
    result.success = false;

    CURL* curl = nullptr;
    CURLcode res{};
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        // Create a list of headers
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "accept: application/json");
        headers = curl_slist_append(headers, "content-type: application/json");
        headers = curl_slist_append(headers, ("APCA-API-KEY-ID: " + apiKey).c_str());
        headers = curl_slist_append(headers, ("APCA-API-SECRET-KEY: " + secretKey).c_str());

        // Set URL, POST data and headers
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set up response handling
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request
        res = curl_easy_perform(curl);
        result.success = (res == CURLE_OK);
        result.response = readBuffer;

        if (result.success) {
            std::cout << "Request successful. Response: " << readBuffer << std::endl;
        }
        else {
            std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        }

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return result;
}

static size_t WriteCallback(void* contents, const size_t size, size_t nmemb, void* userp)
{
    static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}
