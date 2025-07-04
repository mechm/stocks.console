#include "pch.h"

#include "http.h"

#include <curl/curl.h>
#include <iostream>

using namespace std;

RequestResponse Http::GetRequest(const string& url, const vector<string>& headers, const string& payload)
{
    RequestResponse result;
    result.success = false;

    CURL* curl = nullptr;
    CURLcode res{};
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        // Create a list of headers
        struct curl_slist* curl_headers = nullptr;
        //curl_headers = curl_slist_append(curl_headers, "accept: application/json");
        //curl_headers = curl_slist_append(curl_headers, "content-type: application/json");

        // Add provided headers
        for (const auto& header : headers) {
            curl_headers = curl_slist_append(curl_headers, header.c_str());
        }

        //  headers = curl_slist_append(headers, ("APCA-API-KEY-ID: " + apiKey).c_str());
        //  headers = curl_slist_append(headers, ("APCA-API-SECRET-KEY: " + secretKey).c_str());

          // Set URL, POST data and headers
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        if (!payload.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
        }

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
        curl_slist_free_all(curl_headers);
        curl_easy_cleanup(curl);
    }

    return result;
}

static size_t WriteCallback(void* contents, const size_t size, size_t nmemb, void* userp)
{
    static_cast<std::string*>(userp)->append(static_cast<char*>(contents), size * nmemb);
    return size * nmemb;
}