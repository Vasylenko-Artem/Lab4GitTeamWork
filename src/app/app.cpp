#include "app/app.h"

#include "utilities/console.h"
#include "utilities/json_handler.h"
#include "utilities/city.h"

#include <iostream>

void App::run()
{
	// Console::clear();

	vector<GenerateBusJson::BusInfo> buses;
	vector<City> cities = load_cities("database/ua.json");

	for (const auto &city : cities)
		buses.push_back({GenerateBusJson::generate_id(), city.name, city.name, "08:00-12:00", "Artemka"});

	GenerateBusJson::save_to_file("database/busInfo.json", buses);

	vector<GenerateBusJson::BusInfo> buses_from_file = GenerateBusJson::load_from_file("database/busInfo.json");

	// for (const auto &bus : buses_from_file)
	// {
	// 	cout << "ID: " << bus.id << endl;
	// 	cout << "Departure Place: " << bus.departure_place << endl;
	// 	cout << "Destination Place: " << bus.destination_place << endl;
	// 	cout << "Time Range: " << bus.time_range << endl;
	// 	cout << "Driver Name: " << bus.driver_name << endl;
	// 	cout << "------------------------\n";
	// }
}