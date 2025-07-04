#pragma once

#include <string>
#include <vector>

struct RequestResponse {
    bool success = false;
    std::string response = "";
};

class Http {
    public:
        static RequestResponse GetRequest(const std::string& url, const std::vector<std::string>& headers = {}, const std::string& payload = NULL);
};