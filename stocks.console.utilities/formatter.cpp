#include <iostream>
#include <json/json.h>
#include <string>

void PrintFormattedJson(const std::string& jsonString, const std::string& title)
{
    Json::Reader reader;

    if (Json::Value root; reader.parse(jsonString, root)) {
        // Header
        std::cout << "\n====================[ " << title << " ]====================\n";

        // Formatted JSON
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";  // 4 spaces
        builder["precision"] = 2;

        const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(root, &std::cout);

        std::cout << "\n========================================================\n";
    }
    else {
        std::cout << "Invalid JSON format:\n" << jsonString << "\n";
    }
}