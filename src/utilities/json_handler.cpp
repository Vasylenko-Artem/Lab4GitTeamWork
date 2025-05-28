#include "utilities/json_handler.h"

#include <fstream>
#include <iostream>

using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;

int GenerateBusJson::id = 1;

GenerateBusJson::GenerateBusJson(const string &dep_place, const string &dest_place, const string &time_range, const string &driver_name)
{
	busInfo = {generate_id(), dep_place, dest_place, time_range, driver_name};
}

string GenerateBusJson::generate_id() { return to_string(id++); }

void GenerateBusJson::save_to_file(const string &filename, const vector<BusInfo> &buses)
{
	// Перевіряємо, чи існує папка для збереження файлу
	fs::path directory = fs::path(filename).parent_path();

	// Якщо папка не існує, створюємо її
	if (!fs::exists(directory))
		try
		{
			fs::create_directories(directory);
			cout << "Directory '" << directory.string() << "' created.\n";
		}
		catch (const fs::filesystem_error &e)
		{
			cerr << "Error creating directory: " << e.what() << endl;
			return;
		}

	// Створюємо JSON-масив
	json j_array = json::array();
	for (const auto &bus : buses)
		j_array.push_back(bus.to_json());

	// Відкриваємо файл для запису
	ofstream file(filename);

	if (file.is_open())
	{
		// Записуємо JSON-масив у файл
		file << j_array.dump(4); // 4 - кількість пробілів для форматування
		file.close();
		cout << "Route data is written to a file: " << filename << endl;
	}
	else
	{
		cerr << "Failed to open file for writing: " << filename << endl;
	}
}

vector<GenerateBusJson::BusInfo> GenerateBusJson::load_from_file(const string &filename)
{
	ifstream file(filename);
	json j;
	file >> j;

	vector<BusInfo> buses;
	for (const auto &item : j)
	{
		BusInfo bus;
		bus.id = item.at("id").get<string>();
		bus.departure_place = item.at("departure_place").get<string>();
		bus.destination_place = item.at("destination_place").get<string>();
		bus.time_range = item.at("time_range").get<string>();
		bus.driver_name = item.at("driver_name").get<string>();
		buses.push_back(bus);
	}

	return buses;
}