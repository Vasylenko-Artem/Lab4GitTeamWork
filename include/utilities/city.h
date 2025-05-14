#pragma once

#include <vector>

using namespace std;

struct City
{
	string name;
	string admin_name;
};

vector<City> load_cities(const string &filename);
vector<City> filter_cities(const vector<City> &cities, const string &admin_name);