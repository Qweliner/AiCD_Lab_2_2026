//AiCD_lab2_2026.cpp
#include "UI.h"
#include "Structure.h"
#include <windows.h>
#include <iostream>

using namespace std;

// Вход: нет.
// Что делает: главная функция программы, управляет циклом вызова меню и переходами между операциями.
// Выход: код завершения 0.
int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    StudyStructure structure;
    bool needClear = true;

    while (true) {
        if (needClear) {
            system("cls");
            Greeting();
        }
        printMenu();

        string raw = readLine("\nВыбор пункта: ");
        string choice = trim(raw);

        if (choice == "0") {
            break;
        }

        // Защита от спама: пустой ввод (Enter) просто игнорируется
        if (choice.empty()) {
            needClear = false;
            continue;
        }

        if (choice == "1") {
            system("cls");
            handleAddSubject(structure);
            needClear = true;
        }
        else if (choice == "2") {
            system("cls");
            handleAddEvent(structure);
            needClear = true;
        }
        else if (choice == "3") {
            system("cls");
            handleDisplayStructure(structure);
            needClear = true;
        }
        else if (choice == "4") {
            system("cls");
            handleDeleteEvent(structure);
            needClear = true;
        }
        else if (choice == "5") {
            system("cls");
            handleDeleteSubject(structure);
            needClear = true;
        }
        else if (choice == "6") {
            system("cls");
            handleSaveToFile(structure);
            needClear = true;
        }
        else if (choice == "7") {
            system("cls");
            handleLoadFromFile(structure);
            needClear = true;
        }
        else if (choice == "8") {
            system("cls");
            handleSortSubjects(structure);
            needClear = true;
        }
        else if (choice == "9") {
            system("cls");
            handleSearchByEvent(structure);
            needClear = true;
        }
        else {
            cout << "Неверный пункт меню. Введите число от 0 до 9.\n\n";
            needClear = false;
        }
    }
    return 0;
}