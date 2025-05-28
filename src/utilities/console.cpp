#include "utilities/console.h"
#include <ncurses.h>
#include <vector>
#include <string>

void Menu::clear() {
    ::clear();
}

void Menu::consoleHandle() {
    // Optional: move cursor to (0,0), or do nothing (ncurses handles this)
    move(0, 0);
}

void Menu::setCursorVisible(bool visible) {
    curs_set(visible ? 1 : 0);
}

void Menu::DisableScrollBars() {
    // Not needed in ncurses; left empty for compatibility
}

void Menu::display(const std::vector<std::string>& lines, int highlight) {
    clear();
    for (size_t i = 0; i < lines.size(); ++i) {
        if ((int)i == highlight) {
            attron(COLOR_PAIR(1));
            mvprintw(static_cast<int>(i), 2, "%s", lines[i].c_str());
            attroff(COLOR_PAIR(1));
        } else {
            mvprintw(static_cast<int>(i), 2, "%s", lines[i].c_str());
        }
    }
    refresh();
}

int Menu::printMenu() {
    initscr();
    noecho();
    cbreak();
    setCursorVisible(false);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    std::vector<std::string> header = {
        "  _________________",
        " |  _  _  _  _  _ \\\\__",
        " | |_||_||_||_||_| ||_|",
        " |--(0)--------|_|-(0)-|",
        "",
        " Menu:"
    };

    std::vector<std::string> menu = {
        "Filter by route",
        "Filter by driver with sort",
        "Filter by time range",
        "Exit"
    };

    int selected = 0;
    int total_lines = static_cast<int>(header.size() + menu.size());

    while (true) {
        clear();
        // Show header
        for (size_t i = 0; i < header.size(); ++i)
            mvprintw(static_cast<int>(i), 2, "%s", header[i].c_str());
        // Show menu
        for (size_t i = 0; i < menu.size(); ++i) {
            int y = static_cast<int>(i + header.size());
            if ((int)i == selected) {
                attron(COLOR_PAIR(1));
                mvprintw(y, 4, "[<] %s", menu[i].c_str());
                attroff(COLOR_PAIR(1));
            } else {
                mvprintw(y, 4, "[ ] %s", menu[i].c_str());
            }
        }
        refresh();

        int ch = getch();
        if (ch == KEY_UP) {
            selected = (selected - 1 + menu.size()) % menu.size();
        } else if (ch == KEY_DOWN) {
            selected = (selected + 1) % menu.size();
        } else if (ch == '\n' || ch == KEY_ENTER) {
            break;
        }
    }

    endwin(); // Quit ncurses before returning
    return selected;
}