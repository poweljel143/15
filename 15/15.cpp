#include <iostream>
#include <ctime>
#include <cstdlib>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <windows.h>

using namespace std;

const int CONSOLE_WIDTH = 80;
const int CONSOLE_HEIGHT = 25;

void print_centered(const string& message) {
    int padding = (CONSOLE_WIDTH - message.size()) / 2;
    cout << string(padding, ' ') << message << endl;
}

void print_field(int field[][6], int size, HANDLE hConsole) {
    COORD coord = { 0, 0 };
    SetConsoleCursorPosition(hConsole, coord);
    int field_width = size * 5 + 1;
    int padding = (CONSOLE_WIDTH - field_width) / 2;

    for (int i = 0; i < size; i++) {
        cout << string(padding, ' ');
        for (int j = 0; j < size; j++) {
            cout << "+----";
        }
        cout << "+" << endl;

        cout << string(padding, ' ');
        for (int j = 0; j < size; j++) {
            if (field[i][j] == 0)
                cout << "|    ";
            else
                cout << "| " << (field[i][j] < 10 ? " " : "") << field[i][j] << " ";
        }
        cout << "|" << endl;
    }
    cout << string(padding, ' ');
    for (int j = 0; j < size; j++) {
        cout << "+----";
    }
    cout << "+" << endl;
}

bool move(int field[][6], int size, int& zero_i, int& zero_j) {
    if (_kbhit()) {
        char key = _getch();
        int new_i = zero_i, new_j = zero_j;
        switch (key) {
        case 72: new_i--; break; // стрелка вверх
        case 80: new_i++; break; // стрелка вниз
        case 75: new_j--; break; // стрелка влево
        case 77: new_j++; break; // стрелка вправо
        default: return false;
        }
        if (new_i >= 0 && new_i < size && new_j >= 0 && new_j < size) {
            field[zero_i][zero_j] = field[new_i][new_j];
            field[new_i][new_j] = 0;
            zero_i = new_i;
            zero_j = new_j;
            return true;
        }
    }
    return false;
}

bool check_win(int field[][6], int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (field[i][j] != (i * size + j + 1) % (size * size)) {
                return false;
            }
        }
    }
    return true;
}

void create_field(int field[][6], int size, int& zero_i, int& zero_j) {
    vector<int> tiles(size * size);
    for (int i = 0; i < size * size; i++) {
        tiles[i] = i;
    }

    random_shuffle(tiles.begin(), tiles.end());

    for (int i = 0; i < size * size; i++) {
        field[i / size][i % size] = tiles[i];
        if (tiles[i] == 0) {
            zero_i = i / size;
            zero_j = i % size;
        }
    }
}

int select_option(const vector<string>& options, const string& message) {
    int selected = 0;
    while (true) {
        system("cls");
        print_centered(message);
        for (int i = 0; i < options.size(); i++) {
            if (i == selected) {
                print_centered("> " + options[i] + " <");
            }
            else {
                print_centered("  " + options[i] + "  ");
            }
        }
        char key = _getch();
        switch (key) {
        case 72: selected = (selected - 1 + options.size()) % options.size(); break; // стрелка вверх
        case 80: selected = (selected + 1) % options.size(); break; // стрелка вниз
        case 13: return selected; // Enter
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    srand(time(0));
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = false; // скрыть курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    while (true) {
        vector<string> size_options = { "4x4", "5x5", "6x6" };
        int selected_size_option = select_option(size_options, "Выберете размер игрового поля:");
        int size = selected_size_option + 4;

        system("cls");
        int field[6][6];
        int zero_i, zero_j;
        create_field(field, size, zero_i, zero_j);

        while (true) {
            print_field(field, size, hConsole);
            if (check_win(field, size)) {
                vector<string> win_options = { "Да", "Нет" };
                int selected_win_option = select_option(win_options, "Вы победили! Хотите продолжить?");
                if (selected_win_option == 0) {
                    break;
                }
                else {
                    cursorInfo.bVisible = true; // вернуть курсор
                    SetConsoleCursorInfo(hConsole, &cursorInfo);
                    return 0;
                }
            }
            move(field, size, zero_i, zero_j);
        }
    }

    cursorInfo.bVisible = true; // вернуть курсор
    SetConsoleCursorInfo(hConsole, &cursorInfo);
    return 0;
}
