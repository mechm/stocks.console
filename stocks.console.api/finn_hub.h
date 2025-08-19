#pragma once  

#include "http.h"  

class FinnHub : public Http { 
public:
    explicit FinnHub(const std::string& apiKey);
    RequestResponse GetSymbolLookup(const std::string& stock) const;
  
protected:
    std::string apiKey = "";
   
    RequestResponse GetRequest(const std::string& url, const std::string& payload = "") const;
   
};