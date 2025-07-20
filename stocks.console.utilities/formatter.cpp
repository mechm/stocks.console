#include "pch.h"

#include <iostream>
#include <fstream>
#include <json/json.h>
#include <iomanip>
#include <string>

void PrintFormattedJson(const std::string& jsonString, const std::string& title) 
{
    Json::Value root;
    Json::Reader reader;

    if (reader.parse(jsonString, root)) {
        // Header
        std::cout << "\n" << std::string(50, '=') << "\n";
        std::cout << std::setw(25 + title.length() / 2) << title << "\n";
        std::cout << std::string(50, '=') << "\n\n";

        // Formatted JSON
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";  // 4 spaces
        builder["precision"] = 2;

        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(root, &std::cout);

        std::cout << std::string(50, '=') << "\n";
    }
    else {
        std::cout << "Invalid JSON format:\n" << jsonString << "\n";
    }
}