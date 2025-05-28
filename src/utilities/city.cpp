#include "utilities/city.h"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

vector<City> load_cities(const string &filename)
{
	ifstream file(filename);
	json j;
	file >> j;

	vector<City> cities;
	for (const auto &item : j)
	{
		City city;
		city.name = item["city"];
		city.admin_name = item["admin_name"];
		cities.push_back(city);
	}

	return cities;
}

vector<City> filter_cities(const vector<City> &cities, const string &admin_name)
{
	vector<City> filtered_cities;
	for (const auto &city : cities)
		if (city.admin_name == admin_name)
			filtered_cities.push_back(city);

	return filtered_cities;
}