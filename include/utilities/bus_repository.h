#pragma once

#include "utilities/json_handler.h"

using namespace std;

class BusRepository
{
private:
	string filePath;
	vector<GenerateBusJson::BusInfo> buses;

public:
	BusRepository(const string &filePath);

	void load();	   // Загружаємо маршрути з файлу
	void save() const; // Зберігаємо маршрути у файл

	void addBus(const GenerateBusJson::BusInfo &bus);	 // Додавання маршруту
	void updateBus(const GenerateBusJson::BusInfo &bus); // Оновлення маршруту (по ID)
	void deleteBus(const GenerateBusJson::BusInfo &bus); // Видалення маршруту (по ID)

	const vector<GenerateBusJson::BusInfo> &getAll() const; // Отримання всіх маршрутів

	// Фільтрація маршрутів
	vector<GenerateBusJson::BusInfo> filterByRoute(const string &from, const string &to) const; // Фільтрація маршрутів за місцем відправлення та призначення
	vector<GenerateBusJson::BusInfo> filterByDriver(const string &driverName) const;			// Фільтрація маршрутів за ім'ям водія
	vector<GenerateBusJson::BusInfo> filterByTimeRange(const string &timeRange) const;			// Фільтрація маршрутів за часом
};
