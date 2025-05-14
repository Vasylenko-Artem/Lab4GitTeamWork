#include "utilities/console.h"

#include <iostream>

void Console::clear()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
