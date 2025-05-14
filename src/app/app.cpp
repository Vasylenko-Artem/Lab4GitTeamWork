#include "app/app.h"

#include "utilities/console.h"
#include "utilities/json_handler.h"
#include "utilities/city.h"
#include "utilities/bus_repository.h"

#include <iostream>

void App::run()
{
	// Console::clear();
	BusRepository repo("database/busInfo.json");

	std::vector<City> cities = load_cities("database/ua.json");

	// Генерація маршрутів
	for (size_t i = 0; i < cities.size(); ++i)
		for (size_t j = 0; j < cities.size(); ++j)
		{
			if (i == j)
				continue; // Вилучаємо повтори

			repo.addBus({GenerateBusJson::generate_id(),
						 cities[i].name, // Звідки
						 cities[j].name, // Куди
						 "08:00-12:00",	 // Час
						 "Artemka"});	 // Ім'я водія
		}

	repo.addBus({GenerateBusJson::generate_id(),
				 "Kyiv",		// Звідки
				 "Lviv",		// Куди
				 "08:00-12:00", // Час
				 "Stepan"});	// Ім'я водія

	repo.addBus({GenerateBusJson::generate_id(),
				 "Kyiv",		// Звідки
				 "Lviv",		// Куди
				 "08:00-12:00", // Час
				 "Oleksandr"}); // Ім'я водія
	//

	repo.save();

	// Приклад
	auto filtered = repo.filterByRoute("Kyiv", "Lviv");
	for (const auto &bus : filtered)
	{
		std::cout << bus.departure_place << " - " << bus.destination_place << " | " << bus.time_range << " | " << bus.driver_name << '\n';
	}
}
