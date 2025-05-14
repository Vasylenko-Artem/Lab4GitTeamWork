#pragma once

#include <string>
#include <nlohmann/json.hpp>

using namespace std;

class GenerateBusJson
{
public:
	struct BusInfo
	{
		string id;				  // ID
		string departure_place;	  // місце відправлення
		string destination_place; // пункт призначення
		string time_range;		  // Час
		string driver_name;		  // ім'я водія

		nlohmann::json to_json() const
		{
			return nlohmann::json{
				{"id", id},
				{"departure_place", departure_place},
				{"destination_place", destination_place},
				{"time_range", time_range},
				{"driver_name", driver_name}};
		}
	};

	GenerateBusJson(const string &dep_place, const string &dest_place, const string &time_range, const string &driver_name);

	static string generate_id();

	static void save_to_file(const string &filename, const vector<BusInfo> &buses);
	static vector<BusInfo> load_from_file(const string &filename);

private:
	static int id;
	BusInfo busInfo;
};
