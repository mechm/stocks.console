#include "../pch.h"

#include <string>
#include <iostream>

#include "../include/finn_hub.h"

FinnHub::FinnHub(const std::string& apiKey)
{
    if (apiKey.empty()) {
        std::cout << "Error: API credentials not set." << std::endl;
        exit(1);
    }

    this->apiKey = apiKey;
};

RequestResponse FinnHub::GetSymbolLookup(const std::string& stock) const 
{
    const std::string url = "https://finnhub.io/api/v1/search?q=" + stock +"&exchange=US";

    return GetRequest(url);
}

RequestResponse FinnHub::GetRequest(const std::string& url, const std::string& payload) const 
{
    const std::vector<std::string> headers = {
        "accept: application/json",
        R"(content-type: application/json)",
        "X-Finnhub-Token: " + apiKey
    };
    return Http::GetRequest(url, headers, payload);
}
