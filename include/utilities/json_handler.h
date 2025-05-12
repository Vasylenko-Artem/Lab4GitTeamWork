#pragma once

#include <string>
#include <nlohmann/json.hpp>

class BusFareFetcher
{
public:
	BusFareFetcher(const std::string &apiKey);
	void fetchAndProcess(const std::string &url, const std::string &outputFile);

private:
	std::string apiKey;

	std::string getApiResponse(const std::string &url);
	void writeJsonToFile(const nlohmann::json &jsonData, const std::string &fileName);
	void parseAndPrintFares(const nlohmann::json &jsonData);
};
