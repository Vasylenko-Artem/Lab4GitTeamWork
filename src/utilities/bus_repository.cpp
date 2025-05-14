#include "utilities/bus_repository.h"

#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>

using namespace std;

BusRepository::BusRepository(const string &filePath)
	: filePath(filePath)
{
	load();
}

void BusRepository::load()
{
	ofstream file(filePath);
	if (!file.is_open())
		return;

	nlohmann::json j;

	buses.clear();
	for (const auto &item : j)
	{
		GenerateBusJson::BusInfo bus;
		bus.id = item["id"];
		bus.departure_place = item["departure_place"];
		bus.destination_place = item["destination_place"];
		bus.time_range = item["time_range"];
		bus.driver_name = item["driver_name"];
		buses.push_back(bus);
	}
}

void BusRepository::save() const
{
	ofstream file(filePath);
	nlohmann::json j;
	for (const auto &bus : buses)
		j.push_back(bus.to_json());

	file << j.dump(4);
}

void BusRepository::addBus(const GenerateBusJson::BusInfo &bus) { buses.push_back(bus); }

void BusRepository::updateBus(const GenerateBusJson::BusInfo &bus)
{
	for (auto &b : buses)
		if (b.id == bus.id)
		{
			b = bus;
			break;
		}
}

void BusRepository::deleteBus(const GenerateBusJson::BusInfo &bus)
{
	for (auto it = buses.begin(); it != buses.end(); ++it)
		if (it->id == bus.id)
		{
			buses.erase(it);
			break;
		}
}

const vector<GenerateBusJson::BusInfo> &BusRepository::getAll() const
{
	return buses;
}

vector<GenerateBusJson::BusInfo> BusRepository::filterByRoute(const string &from, const string &to) const
{
	vector<GenerateBusJson::BusInfo> filtered;
	for (const auto &bus : buses)
		if (bus.departure_place == from && bus.destination_place == to)
			filtered.push_back(bus);

	return filtered;
}

vector<GenerateBusJson::BusInfo> BusRepository::filterByDriver(const string &driverName) const
{
	vector<GenerateBusJson::BusInfo> filtered;
	for (const auto &bus : buses)
		if (bus.driver_name == driverName)
			filtered.push_back(bus);

	return filtered;
}

vector<GenerateBusJson::BusInfo> BusRepository::filterByTimeRange(const string &timeRange) const
{
	vector<GenerateBusJson::BusInfo> filtered;
	for (const auto &bus : buses)
		if (bus.time_range == timeRange)
			filtered.push_back(bus);

	return filtered;
}
