#pragma once

#include <string>
#include <vector>

struct RequestResponse {
    bool success = false;
    std::string response = "";
};

class Http {
    public:
        static size_t WriteCallback(void* contents, const size_t size, size_t nmemb, void* userp);
        static RequestResponse GetRequest(const std::string& url, const std::vector<std::string>& headers = {}, const std::string& payload = NULL);
};