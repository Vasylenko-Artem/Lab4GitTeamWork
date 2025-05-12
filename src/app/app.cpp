#include "app/app.h"
#include "utilities/json_handler.h"

App::App()
{
}

App::~App()
{
}

void App::run()
{
	BusFareFetcher fetcher("ksOviXZrFms0tZP6CLxWkQ");
	fetcher.fetchAndProcess("https://bus-api.blablacar.com/v1/fares?date=2025-05-13", "fares_data.json");
}