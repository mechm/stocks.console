#include "pch.h"

#include <curl/curl.h>
#include <iostream>
using namespace std;

AlpachaT::AlpachaT(const string& apiKey, const string& secretKey, bool paper)
{    
    if (apiKey.empty() || secretKey.empty()) {
        cout << "Error: API credentials not set." << endl;
        exit(1);
    }

    this->apiKey = apiKey;
    this->secretKey = secretKey;
    this->paper = paper;
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

OrderResult AlpachaT::StockDetail(const string& symbol) const
{
    const string url = paper ? "https://paper-api.alpaca.markets/v2/assets/" + symbol : 
                               "https://api.alpaca.markets/v2/assets/" + symbol;

    //https://data.alpaca.markets/v2/stocks/auctions?symbols=AAPL&start=2024-01-03T00%3A00%3A00Z&
    //end=2024-01-04T00%3A00%3A00Z&limit=1000&feed=sip&sort=asc

    return GetRequest(url, apiKey, secretKey);
}



