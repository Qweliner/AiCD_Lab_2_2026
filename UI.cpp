//UI.cpp
#include "UI.h"
#include "FileManager.h"
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>

using namespace std;

// Безопасный ввод: перехватывает нажатия клавиш, блокирует запрещенные символы 
// например, для имен файлов и обрабатывает выход через ESC.
// prompt - текст перед вводом пользователя, buffer - введенное пользователем
bool safeInput(string& buffer, const string& prompt, bool onlyDigits) {
    cout << prompt << buffer;
    while (true) {
        int c = _getch();
        if (c == 27) return false; // ESC
        if (c == 13) { cout << endl; return true; } // Enter
        if (c == 8) { if (!buffer.empty()) { buffer.pop_back(); cout << "\b \b"; } }
        else if (c >= 32 && c <= 255) {
            if (onlyDigits && !isdigit(c)) continue;
            buffer += (char)c; cout << (char)c;
        }
    }
}

// Вывод приветственного меню в консоль.
void Greeting() {
    cout << "====================================================\n";
    cout << "  Учет успеваемости: Учебные предметы и формы контроля\n";
    cout << "  Лабораторная работа по АиСД (Вариант 8)\n";
    cout << "====================================================\n";
}

// Вывод меню
void printMenu() {
    cout << "Доступные операции:\n"
        << "  1. Добавить новый учебный предмет\n"
        << "  2. Добавить форму контроля к предмету\n"
        << "  3. Вывести текущую структуру ведомости на экран\n"
        << "  4. Удалить форму контроля у предмета\n"
        << "  5. Удалить учебный предмет\n"
        << "  6. Сохранить текущие данные в файл\n"
        << "  7. Загрузить данные из файла\n"
        << "  0. Завершить работу программы\n";
}

// Ожидание нажатия от пользователя
void pressAnyKey() {
    cout << "\nНажмите любую клавишу...";
    _getch();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: запускает интерактивный цикл добавления новых предметов.
// Выход: нет (структура обновлена).
void handleAddSubject(StudyStructure& structure) {
    cout << "========================================================\n";
    cout << "           ДОБАВЛЕНИЕ УЧЕБНЫХ ПРЕДМЕТОВ\n";
    cout << " * Чтобы завершить ввод, оставьте поле пустым и нажмите [Enter].\n";
    cout << " * Чтобы отменить операцию и выйти в меню, нажмите клавишу [Esc].\n";
    cout << "========================================================\n\n";
    while (true) {
        string subject = "";
        if (!safeInput(subject, "Введите название учебного предмета: ")) {
            break; // Нажат Esc
        }
        if (subject.empty()) {
            cout << " -> Ввод предметов успешно завершен.\n";
            break; // Нажат Enter на пустой строке
        }

        SubjectNode* existingNode = structure.findSubject(subject);
        if (existingNode != nullptr) {
            if (existingNode->subjectName != subject) {
                cout << "\n[!] Обнаружено различие в регистре букв!\n";
                cout << "    В системе уже записано: \"" << existingNode->subjectName << "\"\n";
                cout << "    Вы только что ввели:    \"" << subject << "\"\n";

                string correct = "";
                cout << "\n * Оставьте поле пустым для автоматического выбора системного варианта.\n";
                if (safeInput(correct, "Введите окончательный эталонный вариант названия: ")) {
                    if (!correct.empty()) {
                        existingNode->subjectName = correct;
                        cout << " -> Название предмета в ведомости обновлено на: \"" << correct << "\"\n\n";
                    }
                    else {
                        cout << " -> Оставлен текущий системный вариант: \"" << existingNode->subjectName << "\"\n\n";
                    }
                }
                else {
                    break; // Нажат Esc
                }
            }
            else {
                cout << " -> Предупреждение: Предмет \"" << subject << "\" уже присутствует в ведомости.\n\n";
            }
        }
        else {
            structure.addSubject(subject);
            cout << " -> Предмет \"" << subject << "\" успешно добавлен.\n\n";
        }
    }
    pressAnyKey();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: запускает многоуровневый интерактивный цикл добавления форм контроля.
// Выход: нет (структура обновлена).
void handleAddEvent(StudyStructure& structure) {
    while (true) {
        system("cls");
        cout << "========================================================\n";
        cout << "          ДОБАВЛЕНИЕ ФОРМ КОНТРОЛЯ К ПРЕДМЕТАМ\n";
        cout << " * Чтобы отменить операцию и выйти в меню, нажмите клавишу [Esc].\n";
        cout << "========================================================\n\n";

        string subject = "";
        if (!safeInput(subject, "Шаг 1. Введите название учебного предмета: ")) break;
        if (subject.empty()) {
            cout << " -> Поле пустое. Возврат в главное меню.\n";
            pressAnyKey();
            break;
        }

        SubjectNode* subjNode = structure.findSubject(subject);
        if (subjNode == nullptr) {
            cout << "\n[Ошибка] Предмет \"" << subject << "\" не найден в ведомости.\n";
            cout << "Сначала добавьте этот предмет через пункт 1 главного меню.\n";
            pressAnyKey();
            continue;
        }
        else {
            while (true) {
                system("cls");
                cout << "Предмет найден: \"" << subjNode->subjectName << "\"\n";
                cout << "--------------------------------------------------------\n";
                cout << "Шаг 2. Добавление форм контроля к предмету \"" << subjNode->subjectName << "\"\n";
                cout << " * Чтобы завершить добавление, оставьте поле пустым и нажмите [Enter].\n";
                cout << " * Чтобы отменить текущий ввод, нажмите клавишу [Esc].\n";
                cout << "--------------------------------------------------------\n\n";

                EventNode* currEvent = subjNode->eventHead;
                if (currEvent == nullptr) {
                    cout << "У предмета \"" << subjNode->subjectName << "\" пока нет зарегистрированных форм контроля.\n\n";
                }
                else {
                    cout << "Уже добавленные формы контроля:\n";
                    while (currEvent != nullptr) {
                        cout << "  • " << currEvent->eventName << "\n";
                        currEvent = currEvent->next;
                    }
                    cout << "\n";
                }

                string event = "";
                if (!safeInput(event, "Введите новую форму контроля (например: Экзамен, Зачет, КР, КП): ")) break;
                if (event.empty()) {
                    cout << "\n -> Добавление форм контроля для предмета \"" << subjNode->subjectName << "\" успешно завершено.\n";
                    pressAnyKey();
                    break;
                }

                EventNode* existingEvent = structure.findEvent(subjNode->subjectName, event);
                if (existingEvent != nullptr) {
                    if (existingEvent->eventName != event) {
                        cout << "\n[!] Обнаружено различие в регистре букв для формы контроля!\n";
                        cout << "    В системе записано: \"" << existingEvent->eventName << "\"\n";
                        cout << "    Вы ввели:           \"" << event << "\"\n";

                        string correct = "";
                        cout << "\n * Оставьте поле пустым для автоматического выбора системного варианта.\n";
                        if (safeInput(correct, "Введите окончательный эталонный вариант названия: ")) {
                            if (!correct.empty()) {
                                existingEvent->eventName = correct;
                                cout << " -> Название формы контроля успешно обновлено на: \"" << correct << "\"\n\n";
                            }
                            else {
                                cout << " -> Оставлен текущий системный вариант: \"" << existingEvent->eventName << "\"\n\n";
                            }
                        }
                        else {
                            break; // Нажат Esc
                        }
                    }
                    else {
                        cout << " -> [Пропущено] Форма контроля \"" << event << "\" уже существует для этого предмета. Повторная запись не сохранена.\n\n";
                        pressAnyKey();
                    }
                }
                else {
                    structure.addEvent(subjNode->subjectName, event);
                    cout << " -> Форма контроля \"" << event << "\" успешно добавлена к предмету \"" << subjNode->subjectName << "\".\n\n";
                    pressAnyKey();
                }
            }
        }
    }
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: вызывает системный вывод структуры на экран.
// Выход: нет.
void handleDisplayStructure(StudyStructure& structure) {
    cout << "========================================================\n";
    cout << "            ТЕКУЩАЯ СТРУКТУРА ВЕДОМОСТИ\n";
    cout << "========================================================\n\n";
    structure.display();
    pressAnyKey();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: запускает интерактивный цикл пошагового удаления форм контроля.
// Выход: нет (структура обновлена).
void handleDeleteEvent(StudyStructure& structure) {
    while (true) {
        system("cls");
        cout << "========================================================\n";
        cout << "           УДАЛЕНИЕ ФОРМ КОНТРОЛЯ У ПРЕДМЕТОВ\n";
        cout << " * Чтобы отменить операцию и выйти в меню, нажмите клавишу [Esc].\n";
        cout << "========================================================\n\n";

        string subject = "";
        if (!safeInput(subject, "Шаг 1. Введите название предмета: ")) break;
        if (subject.empty()) {
            cout << " -> Поле пустое. Возврат в главное меню.\n";
            pressAnyKey();
            break;
        }

        SubjectNode* subjNode = structure.findSubject(subject);
        if (subjNode == nullptr) {
            cout << "\n[Ошибка] Предмет \"" << subject << "\" не найден в ведомости.\n";
            pressAnyKey();
            continue;
        }
        else {
            while (true) {
                system("cls");
                cout << "Предмет найден: \"" << subjNode->subjectName << "\"\n";
                cout << "--------------------------------------------------------\n";
                cout << "Шаг 2. Удаление форм контроля у предмета \"" << subjNode->subjectName << "\"\n";
                cout << " * Чтобы завершить удаление, оставьте поле пустым и нажмите [Enter].\n";
                cout << " * Чтобы отменить текущий ввод, нажмите клавишу [Esc].\n";
                cout << "--------------------------------------------------------\n\n";

                EventNode* currEvent = subjNode->eventHead;
                if (currEvent == nullptr) {
                    cout << "У предмета \"" << subjNode->subjectName << "\" больше нет зарегистрированных форм контроля.\n\n";
                    cout << " -> Процесс удаления форм контроля завершен. Предмет \"" << subjNode->subjectName << "\" сохранен в ведомости.\n";
                    pressAnyKey();
                    break;
                }
                else {
                    cout << "Доступные для удаления формы контроля:\n";
                    while (currEvent != nullptr) {
                        cout << "  • " << currEvent->eventName << "\n";
                        currEvent = currEvent->next;
                    }
                    cout << "\n";
                }

                string event = "";
                if (!safeInput(event, "Введите форму контроля для удаления: ")) break;
                if (event.empty()) {
                    cout << "\n -> Процесс удаления форм контроля завершен. Предмет \"" << subjNode->subjectName << "\" сохранен в ведомости.\n";
                    pressAnyKey();
                    break;
                }

                if (!structure.hasEvent(subjNode->subjectName, event)) {
                    cout << "\n -> [Ошибка] Форма контроля \"" << event << "\" не найдена у предмета \"" << subjNode->subjectName << "\".\n\n";
                    pressAnyKey();
                }
                else {
                    structure.deleteEvent(subjNode->subjectName, event);
                    cout << " -> Форма контроля \"" << event << "\" успешно удалена.\n\n";
                    pressAnyKey();
                }
            }
        }
    }
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: запускает интерактивный цикл удаления учебных предметов.
// Выход: нет (структура обновлена).
void handleDeleteSubject(StudyStructure& structure) {
    while (true) {
        system("cls");
        cout << "========================================================\n";
        cout << "           УДАЛЕНИЕ УЧЕБНЫХ ПРЕДМЕТОВ\n";
        cout << " * Внимание: предмет удаляется вместе со всеми его формами контроля!\n";
        cout << " * Чтобы завершить удаление, оставьте поле пустым и нажмите [Enter].\n";
        cout << " * Чтобы отменить операцию и выйти в меню, нажмите клавишу [Esc].\n";
        cout << "========================================================\n\n";

        SubjectNode* currSubj = structure.getSubjectHead();
        if (currSubj == nullptr) {
            cout << "В ведомости больше нет зарегистрированных предметов.\n\n";
            pressAnyKey();
            break;
        }
        else {
            cout << "Доступные для удаления учебные предметы:\n";
            while (currSubj != nullptr) {
                cout << "  • " << currSubj->subjectName << "\n";
                currSubj = currSubj->next;
            }
            cout << "\n";
        }

        string subject = "";
        if (!safeInput(subject, "Введите название предмета для полного удаления: ")) break;
        if (subject.empty()) {
            cout << " -> Удаление предметов успешно завершено.\n";
            pressAnyKey();
            break;
        }

        if (!structure.hasSubject(subject)) {
            cout << " -> [Ошибка] Предмет \"" << subject << "\" не найден в ведомости.\n\n";
            pressAnyKey();
        }
        else {
            structure.deleteSubject(subject);
            pressAnyKey();
        }
    }
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: запрашивает имя и сохраняет текущие данные структуры в файл.
// Выход: нет.
void handleSaveToFile(StudyStructure& structure) {
    cout << "========================================================\n";
    cout << "           СОХРАНЕНИЕ ДАННЫХ В ТЕКСТОВЫЙ ФАЙЛ\n";
    cout << " * Чтобы отменить операцию и выйти в меню, нажмите клавишу [Esc].\n";
    cout << "========================================================\n\n";

    string filename = "";
    if (safeInput(filename, "Введите имя файла для записи: ")) {
        if (filename.empty()) {
            cout << " -> Имя файла пустое. Сохранение отменено.\n";
        }
        else {
            if (filename.find(".txt") == string::npos) filename += ".txt";

            string base = filename.substr(0, filename.find('.'));
            string finalName = getIndexedName(base);

            if (finalName != filename) {
                cout << "\n[!] Файл с таким именем существует на диске.\n";
                cout << "    Данные автоматически записаны в новый файл: \"" << finalName << "\"\n";
            }

            structure.saveToFile(finalName);
        }
    }
    pressAnyKey();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: запускает циклический диалог импорта ведомости из файлов.
// Выход: нет (структура обновлена).
void handleLoadFromFile(StudyStructure& structure) {
    while (true) {
        system("cls");
        vector<string> files = getAvailableFiles();
        cout << "========================================================\n";
        cout << "           ЗАГРУЗКА ДАННЫХ ИЗ ТЕКСТОВЫХ ФАЙЛОВ\n";
        cout << " * Вы можете загрузить структуру из предложенного списка\n";
        cout << "   или указать имя файла вручную.\n";
        cout << " * Чтобы выйти в главное меню, нажмите клавишу [Esc] или\n";
        cout << "   введите цифру 0 на шаге выбора.\n";
        cout << "========================================================\n\n";

        cout << "Найденные файлы в директории программы:\n";
        for (size_t i = 0; i < files.size(); i++) {
            cout << "  " << i + 1 << ". " << files[i] << endl;
        }
        cout << "  0. Ввести имя файла вручную\n";

        string choiceStr = "";
        if (!safeInput(choiceStr, "\nСделайте выбор (номер пункта): ", true)) break;
        if (choiceStr.empty()) break;

        int choice = stoi(choiceStr);
        string filenameToLoad = "";

        if (choice == 0) {
            string manualName = "";
            cout << "\n--------------------------------------------------------\n";
            cout << " * Введите полное название файла.\n";
            cout << " * Если расширение файла (.txt) не указано, оно добавится автоматически.\n";
            cout << " * Нажмите клавишу [Esc] для отмены ручного ввода.\n";
            cout << "--------------------------------------------------------\n\n";

            if (safeInput(manualName, "Введите имя файла для загрузки: ")) {
                if (manualName.empty()) continue;
                if (manualName.find(".txt") == string::npos) {
                    manualName += ".txt";
                }
                filenameToLoad = manualName;
            }
            else {
                break;
            }
        }
        else if (choice >= 1 && choice <= (int)files.size()) {
            filenameToLoad = files[choice - 1];
        }
        else {
            cout << "\n[Ошибка] Неверный выбор пункта меню. Пожалуйста, повторите ввод.\n";
            pressAnyKey();
            continue;
        }

        if (!filenameToLoad.empty()) {
            if (!fileExists(filenameToLoad)) {
                cout << "\n[Ошибка] Файл \"" << filenameToLoad << "\" не найден в директории программы.\n";
                cout << "Убедитесь, что имя указано верно и файл находится в папке с исполняемым .exe файлом.\n";
            }
            else {
                cout << "\nВыберите режим обработки данных:\n";
                cout << "  1. Сбросить текущую ведомость и загрузить заново (перезаписать)\n";
                cout << "  2. Дополнить текущую ведомость данными из файла (слияние без дубликатов)\n";
                cout << "  * Нажмите клавишу [Esc] для отмены загрузки.\n\n";

                string mode = "";
                if (!safeInput(mode, "Ваш выбор (1 или 2): ", true)) break;

                if (mode == "1") {
                    structure.clear();
                    structure.loadFromFile(filenameToLoad);
                }
                else if (mode == "2") {
                    structure.loadFromFile(filenameToLoad);
                }
                else {
                    cout << "\n[!] Операция загрузки файла отменена пользователем.\n";
                }
            }
        }
        pressAnyKey();
    }
}