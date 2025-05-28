#include "app/app.h"
#include "utilities/console.h"
#include "utilities/json_handler.h"
#include "utilities/city.h"
#include "utilities/bus_repository.h"
#include "utilities/tree_sort.h"
#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>

// Helper for getting a string from the user in ncurses
std::string ncurses_input(int y, int x, const std::string& prompt) {
    mvprintw(y, x, "%s", prompt.c_str());
    echo();
    char buf[256];
    getnstr(buf, 255); // safely read input
    noecho();
    return std::string(buf);
}

void App::run()
{
    // All ncurses setup is handled in Console1::printMenu() and helpers

    BusRepository repo("database/busInfo.json");
    std::vector<City> cities = load_cities("database/ua.json");

    // Генерація маршрутів
    for (size_t i = 0; i < cities.size(); ++i)
        for (size_t j = 0; j < cities.size(); ++j)
        {
            if (i == j) continue;
            repo.addBus({GenerateBusJson::generate_id(),
                         cities[i].name,
                         cities[j].name,
                         "08:00-12:00",
                         "Artemka"});
        }
    repo.addBus({GenerateBusJson::generate_id(), "Kyiv", "Lviv", "08:00-14:00", "Stepan"});
    repo.addBus({GenerateBusJson::generate_id(), "Kyiv", "Lviv", "08:00-18:00", "Oleksandr"});
    repo.save();

    std::vector<std::string> menu = {
        "Filter by route",
        "Filter by driver with sort",
        "Filter by time range",
        "Exit"
    };

    while (true) {
        int selected = Menu::printMenu();

        if (selected == 3) {
            // "Exit"
            break;
        }

        clear();
        refresh();

        switch (selected) {
        case 0: { // Filter by route
            std::string city1 = ncurses_input(2, 2, "Enter departure city: ");
            std::string city2 = ncurses_input(3, 2, "Enter destination city: ");

            auto filtered = repo.filterByRoute(city1, city2);
            std::vector<std::string> lines;
            if (filtered.empty()) {
                lines.push_back("No routes found from " + city1 + " to " + city2 + ".");
            } else {
                lines.push_back("Routes from " + city1 + " to " + city2 + ":");
                for (const auto& bus : filtered) {
                    lines.push_back(bus.departure_place + " - " + bus.destination_place + " | " + bus.time_range + " | Driver: " + bus.driver_name);
                }
            }
            Menu::display(lines, -1);
            mvprintw(LINES-2, 2, "Press any key to return to the menu...");
            getch();
            break;
        }
        case 1: { // View by driver with sort
            std::string name = ncurses_input(2, 2, "Enter driver name (Artemka, Oleksandr, Stepan): ");
            auto filtered = repo.filterByDriver(name);
            if (!filtered.empty()) filtered = tree_sort(filtered, "time", 1);
            std::vector<std::string> lines;
            if (filtered.empty()) {
                lines.push_back("No routes found for driver: " + name);
            } else {
                lines.push_back("Routes for driver " + name + ":");
                for (const auto& bus : filtered) {
                    lines.push_back(bus.departure_place + " - " + bus.destination_place + " | " + bus.time_range + " | " + bus.driver_name);
                }
            }
            Menu::display(lines, -1);
            mvprintw(LINES-2, 2, "Press any key to return to the menu...");
            getch();
            break;
        }
        case 2: { // Choose by time
            std::string time_range = ncurses_input(2,2, "Enter time range (e.g., 08:00-12:00): ");
            auto filtered = repo.filterByTimeRange(time_range);
            std::vector<std::string> lines;
            if (filtered.empty()) {
                lines.push_back("No routes found for time range: " + time_range);
            } else {
                lines.push_back("Routes for time range " + time_range + ":");
                for (const auto& bus : filtered) {
                    lines.push_back(bus.departure_place + " - " + bus.destination_place + " | " + bus.time_range + " | " + bus.driver_name);
                }
            }
            Menu::display(lines, -1);
            mvprintw(LINES-2, 2, "Press any key to return to the menu...");
            getch();
            break;
        }
        }
    }

    // End ncurses mode after all UI is done
    endwin();
}