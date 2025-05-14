#include "app/app.h"

#include "utilities/console.h"
#include "utilities/json_handler.h"

#include <iostream>

using namespace std;

#include <fstream>
#include <vector>

struct City
{
	string name;
	string admin_name;
};

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

void App::run()
{
	// Console::clear();

	vector<GenerateBusJson::BusInfo> buses;
	vector<City> cities = load_cities("database/ua.json");

	for (const auto &city : cities)
		buses.push_back({GenerateBusJson::generate_id(), city.name, city.name, "08:00-12:00", "Artemka"});

	GenerateBusJson::save_to_file("database/busInfo.json", buses);

	vector<GenerateBusJson::BusInfo> buses_from_file = GenerateBusJson::load_from_file("database/busInfo.json");

	for (const auto &bus : buses_from_file)
	{
		cout << "ID: " << bus.id << endl;
		cout << "Departure Place: " << bus.departure_place << endl;
		cout << "Destination Place: " << bus.destination_place << endl;
		cout << "Time Range: " << bus.time_range << endl;
		cout << "Driver Name: " << bus.driver_name << endl;
		cout << "------------------------\n";
	}
}