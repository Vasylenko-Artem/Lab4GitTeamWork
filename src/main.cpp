#include <iostream>
#include "app/app.h"

void clearConsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int main()
{
    clearConsole();
    App app;
    app.run();
    return 0;
}
