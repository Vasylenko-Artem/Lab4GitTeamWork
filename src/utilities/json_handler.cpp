
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "utilities/json_handler.h"

BusFareFetcher::BusFareFetcher(const std::string &apiKey) : apiKey(apiKey) {}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *output)
{
	size_t totalSize = size * nmemb;
	output->append((char *)contents, totalSize);
	return totalSize;
}

std::string BusFareFetcher::getApiResponse(const std::string &url)
{
	CURL *curl = curl_easy_init();
	std::string response;

	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, ("Authorization: Token " + apiKey).c_str());
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "gzip, deflate");

		CURLcode res = curl_easy_perform(curl);
		if (res != CURLE_OK)
		{
			std::cerr << "cURL Error: " << curl_easy_strerror(res) << std::endl;
		}
		curl_easy_cleanup(curl);
	}

	return response;
}

void BusFareFetcher::writeJsonToFile(const nlohmann::json &jsonData, const std::string &fileName)
{
	std::string folder = "database";
	std::filesystem::create_directories(folder);

	std::string fullPath = folder + "/" + fileName;
	std::ofstream outFile(fullPath);
	if (outFile.is_open())
	{
		outFile << jsonData.dump(4);
		outFile.close();
		std::cout << "JSON written to file: " << fullPath << std::endl;
	}
	else
	{
		std::cerr << "Failed to open file: " << fullPath << std::endl;
	}
}

void BusFareFetcher::parseAndPrintFares(const nlohmann::json &jsonData)
{
	for (const auto &fare : jsonData["fares"])
	{
		std::cout << "Fare ID: " << fare["id"] << std::endl;
		std::cout << "Departure: " << fare["departure"] << std::endl;
		std::cout << "Arrival: " << fare["arrival"] << std::endl;
		std::cout << "Price: " << fare["price_cents"].get<int>() / 100.0 << " " << fare["price_currency"] << std::endl;

		std::cout << "Legs:" << std::endl;
		for (const auto &leg : fare["legs"])
		{
			std::cout << "  Bus Number: " << leg["bus_number"] << std::endl;
			std::cout << "  Departure: " << leg["departure"] << std::endl;
			std::cout << "  Arrival: " << leg["arrival"] << std::endl;
		}
		std::cout << "-------------------------" << std::endl;
	}
}

void BusFareFetcher::fetchAndProcess(const std::string &url, const std::string &outputFile)
{
	std::string jsonResponse = getApiResponse(url);

	try
	{
		nlohmann::json parsedJson = nlohmann::json::parse(jsonResponse);
		writeJsonToFile(parsedJson, outputFile);
		parseAndPrintFares(parsedJson);
	}
	catch (const nlohmann::json::exception &e)
	{
		std::cerr << "JSON parsing error: " << e.what() << std::endl;
	}
}
