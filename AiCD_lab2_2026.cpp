//main.cpp
#include "UI.h"
#include "Structure.h"
#include <windows.h>

using namespace std;

int main() {
    // Настройка кодировки для русского языка
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    StudyStructure structure;

    while (true) {
        system("cls");
        Greeting();
        printMenu();

        string buffer;
        // Используем safeInput с флагом onlyDigits=true для выбора пункта меню
        if (!safeInput(buffer, "\nВыбор (введите номер пункта): ", true)) break; // ESC для выхода из программы

        int state = stoi(buffer);
        system("cls");

        switch (state) {
        case 1:
            handleAddSubject(structure);
            break;
        case 2:
            handleAddEvent(structure);
            break;
        case 3:
            handleDisplayStructure(structure);
            break;
        case 4:
            handleDeleteEvent(structure);
            break;
        case 5:
            handleDeleteSubject(structure);
            break;
        case 6:
            handleSaveToFile(structure);
            break;
        case 7:
            handleLoadFromFile(structure);
            break;
        case 0:
            return 0;
        default:
            cout << "Неверный пункт меню\n";
            pressAnyKey();
            break;
        }
    }
    return 0;
}