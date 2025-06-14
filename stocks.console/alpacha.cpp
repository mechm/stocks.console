#include <iostream>
using namespace std;

#include <curl/curl.h>
#include <sstream>
#include "alpacha.h"

Alpacha::Alpacha(const string &apiKey, const string &secretKey, const bool paper)
{
    this->apiKey = apiKey;
    this->secretKey = secretKey;
    this->paper = paper;
}

static size_t WriteCallback(void* contents, const size_t size, size_t nmemb, void* userp)
{
    static_cast<std::string *>(userp)->append(static_cast<char *>(contents), size * nmemb);
    return size * nmemb;
}

static OrderResult PlaceOrder(const string &symbol, const double quantity, const string &side, const string &apiKey, const string &secretKey, const bool paper)
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

static OrderResult GetRequest(const string &url, const string &apiKey, const string &secretKey)
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

OrderResult Alpacha::BuyStock(const string &symbol, const double quantity) const
{
    return PlaceOrder(symbol, quantity, "buy", apiKey, secretKey, paper);
}

OrderResult Alpacha::SellStock(const string &symbol, const double quantity) const
{
    return PlaceOrder(symbol, quantity, "sell", apiKey, secretKey, paper);
}

OrderResult Alpacha::GetAccount() const
{
    const string url = paper ? "https://paper-api.alpaca.markets/v2/account" : "https://api.alpaca.markets/v2/account";
    return GetRequest(url, apiKey, secretKey);
}

OrderResult Alpacha::GetAllOpenPositions() const 
{
    const string url = paper ? "https://paper-api.alpaca.markets/v2/positions" : "https://api.alpaca.markets/v2/positions";
    return GetRequest(url, apiKey, secretKey);
}

OrderResult Alpacha::StockDetail(const string &symbol) const
{
    const string url = paper ? "https://paper-api.alpaca.markets/v2/assets/" + symbol : "https://api.alpaca.markets/v2/assets/" + symbol;
    return GetRequest(url, apiKey, secretKey);
}

