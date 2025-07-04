#include "pch.h"
#include "alpacha.h"

using namespace std;

AlpachaAccount::AlpachaAccount(const string& apiKey, const string& secretKey, const bool paper) : Alpacha(apiKey, secretKey, paper)
{
}

RequestResponse AlpachaAccount::GetAccount()
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/account";
    return Alpacha::GetRequest(url);
}

RequestResponse AlpachaAccount::GetAllOpenPositions()
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/positions";
    return Alpacha::GetRequest(url);
}

RequestResponse AlpachaAccount::BuyStock(const string& symbol, const double quantity)
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/orders";
    const string payload = "{\"symbol\":\"" + symbol + "\",\"qty\":" + to_string(quantity) + ", \"side\":\""
        + "buy" + "\",\"type\":\"market\",\"time_in_force\":\"day\"}";
    return Alpacha::GetRequest(url, payload);
}

RequestResponse AlpachaAccount::SellStock(const string& symbol, const double quantity)
{
    const string url = (paper ? paperApiUrl : liveApiUrl) + "/orders";
    const string payload = "{\"symbol\":\"" + symbol + "\",\"qty\":" + to_string(quantity) + ", \"side\":\""
        + "sell" + "\",\"type\":\"market\",\"time_in_force\":\"day\"}";
    return Alpacha::GetRequest(url, payload);
}