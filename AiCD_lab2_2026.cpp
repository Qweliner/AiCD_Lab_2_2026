#include "UI.h"
#include "Structure.h"
#include "FileManager.h"
#include <iostream>
#include <string>
#include <vector>
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
        if (!safeInput(buffer, "\nВыбор: ", true)) break; // ESC для выхода из программы

        int state = stoi(buffer);
        system("cls");

        switch (state) {
            // 1. Добавление нового предмета
        case 1: {
            string subject = "";
            if (safeInput(subject, "Введите название предмета: ")) {
                structure.addSubject(subject);
                cout << "\nПредмет добавлен";
            }
            pressAnyKey();
            break;
        }

              // 2. Добавление события (ветки)
        case 2: {
            string subject = "", event = "";
            if (safeInput(subject, "Введите название предмета: ")) {
                if (safeInput(event, "Введите событие (зачет/экзамен): ")) {
                    structure.addEvent(subject, event);
                }
            }
            pressAnyKey();
            break;
        }

              // 3. Вывод структуры
        case 3: {
            structure.display();
            pressAnyKey();
            break;
        }

              // 4. Удаление события
        case 4: {
            string subject = "", event = "";
            if (safeInput(subject, "Введите название предмета: ")) {
                if (safeInput(event, "Введите событие для удаления: ")) {
                    structure.deleteEvent(subject, event);
                }
            }
            pressAnyKey();
            break;
        }

              // 5. Удаление предмета
        case 5: {
            string subject = "";
            if (safeInput(subject, "Введите название предмета: ")) {
                structure.deleteSubject(subject);
            }
            pressAnyKey();
            break;
        }

              // 6. Сохранение структуры в файл
        case 6: {
            string filename = "";
            if (safeInput(filename, "Введите имя файла: ")) {
                // Если забыли .txt, добавляем сами
                if (filename.find(".txt") == string::npos) filename += ".txt";

                // Авто-индексация имени (чтобы не затереть существующий файл)
                string base = filename.substr(0, filename.find('.'));
                string finalName = getIndexedName(base);

                if (finalName != filename) {
                    cout << "\n[!] Файл с таким именем есть. Сохранено как: " << finalName << endl;
                }

                structure.saveToFile(finalName);
            }
            pressAnyKey();
            break;
        }

              // 7. Загрузка из файла
        case 7: {
            vector<string> files = getAvailableFiles();
            if (files.empty()) {
                cout << "TXT файлы не найдены\n";
                pressAnyKey();
                break;
            }

            cout << "Найденные файлы:\n\n";
            for (size_t i = 0; i < files.size(); i++) {
                cout << i + 1 << ". " << files[i] << endl;
            }

            string choiceStr = "";
            if (safeInput(choiceStr, "\nВыберите файл (номер): ", true)) {
                int choice = stoi(choiceStr);
                if (choice < 1 || choice >(int)files.size()) {
                    cout << "Неверный выбор\n";
                }
                else {
                    cout << "\n1. Заменить текущую структуру\n2. Добавить к существующей\nВыбор: ";
                    string mode = "";
                    safeInput(mode, "", true);

                    if (mode == "1") structure.clear();
                    structure.loadFromFile(files[choice - 1]);
                }
            }
            pressAnyKey();
            break;
        }

              // 0. Выход
        case 0: return 0;

        default:
            cout << "Неверный пункт меню\n";
            pressAnyKey();
            break;
        }
    }
    return 0;
}