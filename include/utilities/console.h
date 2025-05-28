#pragma once
#include <string>
#include <vector>

class Menu
{
public:
    static void clear();
    static int printMenu();
    static void consoleHandle();               // Resets cursor position (implementation depends)
    static void setCursorVisible(bool visible);
    static void DisableScrollBars();           // Not needed for ncurses, but kept for compatibility
    static void display(const std::vector<std::string>& lines, int highlight = -1);
};