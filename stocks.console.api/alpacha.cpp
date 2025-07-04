#include "pch.h"

#include <curl/curl.h>
#include <iostream>

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