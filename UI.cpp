//UI.cpp
#include "UI.h"
#include <iostream>
#include <fstream>
#include <conio.h>

using namespace std;

// Вход: исходная строка.
// Что делает: удаляет пробелы и символы перевода строки по краям строки.
// Выход: очищенная строка.
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

// Вход: символ.
// Что делает: проверяет, является ли символ буквой (латиница или кириллица CP1251).
// Выход: true если буква, иначе false.
bool isLetter(unsigned char c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return true;
    if (c >= 0xC0 && c <= 0xFF) return true;
    if (c == 0xA8 || c == 0xB8) return true;
    return false;
}

// Вход: название предмета, ссылка на строку для текста ошибки.
// Что делает: проверяет длину (2-50 символов), запрет пробелов, отсутствие цифр и спецсимволов.
// Выход: true если название корректно, иначе false.
bool isValidSubjectName(const string& str, string& errorMsg) {
    if (str.length() < 2) {
        errorMsg = "Ошибка: название слишком короткое (минимум 2 буквы).";
        return false;
    }
    if (str.length() > 50) {
        errorMsg = "Ошибка: название слишком длинное (максимум 50 символов).";
        return false;
    }

    if (str.find(' ') != string::npos) {
        errorMsg = "Ошибка: пробелы запрещены, так как они разделяют поля в файле. Используйте дефис (например: Мат-анализ).";
        return false;
    }

    for (size_t i = 0; i < str.length(); i++) {
        if (isdigit((unsigned char)str[i])) {
            errorMsg = "Ошибка: название предмета не может содержать цифры.";
            return false;
        }
    }

    for (size_t i = 0; i < str.length(); i++) {
        unsigned char c = (unsigned char)str[i];
        if (!isLetter(c) && c != '-') {
            errorMsg = "Ошибка: название содержит недопустимые спецсимволы.";
            return false;
        }
    }

    if (str.find("--") != string::npos) {
        errorMsg = "Ошибка: нельзя использовать несколько дефисов подряд.";
        return false;
    }

    if (!isLetter((unsigned char)str[0]) || !isLetter((unsigned char)str[str.length() - 1])) {
        errorMsg = "Ошибка: название должно начинаться и заканчиваться буквой.";
        return false;
    }

    return true;
}

// Вход: название формы контроля, ссылка на строку для текста ошибки.
// Что делает: проверяет длину (2-30 символов), запрет пробелов, отсутствие цифр и спецсимволов.
// Выход: true если название корректно, иначе false.
bool isValidEventName(const string& str, string& errorMsg) {
    if (str.length() < 2) {
        errorMsg = "Ошибка: форма контроля слишком короткая (минимум 2 буквы).";
        return false;
    }
    if (str.length() > 30) {
        errorMsg = "Ошибка: форма контроля слишком длинная (максимум 30 символов).";
        return false;
    }

    if (str.find(' ') != string::npos) {
        errorMsg = "Ошибка: пробелы запрещены. Используйте аббревиатуру или дефис (например: Диф-зачет).";
        return false;
    }

    for (size_t i = 0; i < str.length(); i++) {
        if (isdigit((unsigned char)str[i])) {
            errorMsg = "Ошибка: форма контроля не может содержать цифры.";
            return false;
        }
    }

    for (size_t i = 0; i < str.length(); i++) {
        unsigned char c = (unsigned char)str[i];
        if (!isLetter(c) && c != '-') {
            errorMsg = "Ошибка: форма контроля содержит недопустимые символы.";
            return false;
        }
    }

    if (str.find("--") != string::npos) {
        errorMsg = "Ошибка: нельзя использовать несколько дефисов подряд.";
        return false;
    }

    if (!isLetter((unsigned char)str[0]) || !isLetter((unsigned char)str[str.length() - 1])) {
        errorMsg = "Ошибка: форма контроля должна начинаться и заканчиваться буквой.";
        return false;
    }

    return true;
}

// Вход: имя файла, ссылка на строку для текста ошибки.
// Что делает: проверяет имя на пустую строку, запрещенные символы и имена устройств Windows.
// Выход: true если имя файла допустимо, иначе false.
bool isValidFileName(const string& filename, string& errorMsg) {
    if (filename.empty()) {
        errorMsg = "Ошибка: имя файла не может быть пустым.";
        return false;
    }
    if (filename.length() > 100) {
        errorMsg = "Ошибка: имя файла слишком длинное.";
        return false;
    }

    string badChars = "<>:\"/\\|?*";
    for (size_t i = 0; i < filename.length(); i++) {
        char c = filename[i];
        if (badChars.find(c) != string::npos || (unsigned char)c < 32) {
            errorMsg = "Ошибка: имя файла содержит запрещенные символы (<>:\"/\\|?*).";
            return false;
        }
    }

    string base = filename;
    size_t dotPos = base.find('.');
    if (dotPos != string::npos) {
        base = base.substr(0, dotPos);
    }
    string upperBase = toUpperString(base);
    if (upperBase == "CON" || upperBase == "PRN" || upperBase == "AUX" ||
        upperBase == "NUL" || upperBase == "COM1" || upperBase == "LPT1") {
        errorMsg = "Ошибка: имя файла является зарезервированным системным именем.";
        return false;
    }

    return true;
}

// Вход: строка приглашения.
// Что делает: выводит приглашение один раз и ожидает реального ввода (защита от спама на Enter).
// Выход: введенная строка.
string readLine(const string& prompt) {
    cout << prompt;
    string res;
    while (getline(cin, res)) {
        if (!trim(res).empty() || prompt.find("Enter -") != string::npos) {
            return res;
        }
    }
    return "";
}

// Вход: имя файла.
// Что делает: проверяет доступность файла для чтения на диске.
// Выход: true если файл существует, иначе false.
bool fileExists(const string& filename) {
    ifstream file(filename.c_str());
    return file.is_open();
}

// Вход: базовое имя файла.
// Что делает: подбирает свободное имя с числовым индексом в скобках.
// Выход: уникальное имя файла.
string getIndexedFilename(const string& filename) {
    string base = filename;
    string ext = ".txt";

    size_t dotPos = filename.rfind('.');
    if (dotPos != string::npos) {
        base = filename.substr(0, dotPos);
        ext = filename.substr(dotPos);
    }

    int index = 1;
    while (true) {
        string candidate = base + "(" + to_string(index) + ")" + ext;
        if (!fileExists(candidate)) {
            return candidate;
        }
        index++;
    }
}

// Вход: нет.
// Что делает: выводит заголовок программы.
// Выход: нет.
void Greeting() {
    cout << "Учет успеваемости: Учебные предметы и формы контроля\n";
    cout << "Лабораторная работа по АиСД (Вариант 8)\n\n";
}

// Вход: нет.
// Что делает: выводит перечень пунктов главного меню.
// Выход: нет.
void printMenu() {
    cout << "Доступные операции:\n"
        << "  1. Добавить учебный предмет\n"
        << "  2. Добавить форму контроля к предмету\n"
        << "  3. Вывести текущую структуру ведомости\n"
        << "  4. Удалить форму контроля у предмета\n"
        << "  5. Удалить учебный предмет\n"
        << "  6. Сохранить данные в файл\n"
        << "  7. Загрузить данные из файла\n"
        << "  8. Сортировка предметов\n"
        << "  9. Поиск предметов по форме контроля\n"
        << "  0. Выход из программы\n";
}

// Вход: нет.
// Что делает: ожидает нажатия любой клавиши клавиатуры.
// Выход: нет.
void pressAnyKey() {
    cout << "\nНажмите любую клавишу для продолжения...";
    int key = _getch();
    if (key == 0 || key == 224) {
        if (_kbhit()) {
            _getch();
        }
    }
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: циклически запрашивает и добавляет новые предметы до ввода 0.
// Выход: структура обновлена.
void handleAddSubject(StudyStructure& structure) {
    cout << "\nДобавление предметов\n";
    cout << "Внимание, нажатие на enter при пустом вводе приведет к переходу на следущую строку, а не ко вводу пустой строки!\n";
    cout << "0 - возврат на шаг назад\n\n";
    while (true) {
        string raw = readLine("Введите название предмета (0 - назад в главное меню): ");
        string subject = trim(raw);

        if (subject == "0") {
            cout << "-> Возврат в главное меню.\n";
            break;
        }

        string errorMsg;
        if (!isValidSubjectName(subject, errorMsg)) {
            cout << errorMsg << "\n\n";
            continue;
        }

        if (structure.hasSubject(subject)) {
            cout << "Предмет \"" << subject << "\" уже есть в списке.\n\n";
        }
        else {
            structure.addSubject(subject);
            cout << "Предмет \"" << subject << "\" добавлен.\n\n";
        }
    }

    pressAnyKey();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: находит предмет и циклически добавляет к нему формы контроля до ввода 0.
// Выход: структура обновлена.
void handleAddEvent(StudyStructure& structure) {
    if (structure.getSubjectHead() == nullptr) {
        cout << "\nВ структуре еще нет ни одного предмета.\n";
        cout << "Сначала добавьте предмет через пункт 1.\n";
        pressAnyKey();
        return;
    }

    cout << "\nДобавление форм контроля\n";
    cout << "0 - возврат на шаг назад\n\n";

    // Вывод списка доступных предметов
    SubjectNode* cur = structure.getSubjectHead();
    cout << "Список доступных предметов:\n";
    while (cur != nullptr) {
        cout << "  • " << cur->subjectName << "\n";
        cur = cur->next;
    }
    cout << "\n";

    while (true) {
        string raw = readLine("Введите название предмета (0 - назад в главное меню): ");
        string subject = trim(raw);

        if (subject == "0") {
            cout << "-> Возврат в главное меню.\n";
            break;
        }

        SubjectNode* subjNode = structure.findSubject(subject);
        if (subjNode == nullptr) {
            cout << "Ошибка: предмет \"" << subject << "\" не найден в структуре.\n\n";
            continue;
        }

        cout << "\nПредмет: " << subjNode->subjectName << "\n";
        cout << "0 - возврат на шаг назад (к выбору предмета)\n\n";

        while (true) {
            EventNode* ev = subjNode->eventHead;
            if (ev != nullptr) {
                cout << "Текущие формы контроля: ";
                while (ev != nullptr) {
                    cout << "[" << ev->eventName << "] ";
                    ev = ev->next;
                }
                cout << "\n";
            }

            string rawEvent = readLine("Введите форму контроля (Экзамен, Зачет, КП и т.д., 0 - назад к выбору предмета): ");
            string event = trim(rawEvent);

            if (event == "0") {
                cout << "-> Возврат на шаг назад (к выбору предмета).\n\n";
                break;
            }

            string errorMsg;
            if (!isValidEventName(event, errorMsg)) {
                cout << errorMsg << "\n\n";
                continue;
            }

            event = toUpperString(event);

            if (structure.hasEvent(subjNode->subjectName, event)) {
                cout << "Форма контроля \"" << event << "\" уже добавлена к этому предмету.\n\n";
            }
            else {
                structure.addEvent(subjNode->subjectName, event);
                cout << "Форма контроля \"" << event << "\" успешно добавлена.\n\n";
            }
        }
    }

    pressAnyKey();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: отображает всю текущую ведомость предметов и форм контроля.
// Выход: нет.
void handleDisplayStructure(StudyStructure& structure) {
    cout << "\nТекущая структура ведомости:\n\n";
    structure.display();
    pressAnyKey();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: циклически запрашивает предмет и форму контроля для удаления до ввода 0.
// Выход: структура обновлена.
void handleDeleteEvent(StudyStructure& structure) {
    if (structure.getSubjectHead() == nullptr) {
        cout << "\nВ структуре нет предметов для удаления.\n";
        pressAnyKey();
        return;
    }

    cout << "\nУдаление формы контроля\n";
    cout << "0 - возврат на шаг назад\n\n";

    while (true) {
        SubjectNode* cur = structure.getSubjectHead();
        if (cur == nullptr) {
            cout << "Список предметов пуст.\n";
            break;
        }

        // Вывод списка предметов с их формами контроля перед выбором
        cout << "Доступные учебные предметы:\n";
        while (cur != nullptr) {
            cout << "  • " << cur->subjectName;
            if (cur->eventHead != nullptr) {
                cout << " (формы контроля: ";
                EventNode* ev = cur->eventHead;
                while (ev != nullptr) {
                    cout << "[" << ev->eventName << "] ";
                    ev = ev->next;
                }
                cout << ")";
            }
            else {
                cout << " (нет форм контроля)";
            }
            cout << "\n";
            cur = cur->next;
        }
        cout << "\n";

        string raw = readLine("Введите название предмета (0 - назад в главное меню): ");
        string subject = trim(raw);

        if (subject == "0") {
            cout << "-> Возврат в главное меню.\n";
            break;
        }

        SubjectNode* subjNode = structure.findSubject(subject);
        if (subjNode == nullptr) {
            cout << "Ошибка: предмет \"" << subject << "\" не найден.\n\n";
            continue;
        }

        EventNode* ev = subjNode->eventHead;
        if (ev == nullptr) {
            cout << "У предмета \"" << subjNode->subjectName << "\" нет форм контроля.\n\n";
            continue;
        }

        while (true) {
            if (subjNode->eventHead == nullptr) {
                cout << "У предмета \"" << subjNode->subjectName << "\" не осталось форм контроля.\n\n";
                break;
            }

            cout << "\nПредмет: " << subjNode->subjectName << "\n";
            cout << "Доступные для удаления формы контроля: ";
            EventNode* curEv = subjNode->eventHead;
            while (curEv != nullptr) {
                cout << "[" << curEv->eventName << "] ";
                curEv = curEv->next;
            }
            cout << "\n";

            string rawEvent = readLine("Введите форму контроля для удаления (0 - назад к выбору предмета): ");
            string event = trim(rawEvent);

            if (event == "0") {
                cout << "-> Возврат на шаг назад (к выбору предмета).\n\n";
                break;
            }

            event = toUpperString(event);

            if (!structure.hasEvent(subjNode->subjectName, event)) {
                cout << "Ошибка: форма контроля \"" << event << "\" не найдена.\n\n";
            }
            else {
                structure.deleteEvent(subjNode->subjectName, event);
            }
        }
    }

    pressAnyKey();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: циклически удаляет предметы с подтверждением при наличии форм контроля до ввода 0.
// Выход: структура обновлена.
void handleDeleteSubject(StudyStructure& structure) {
    cout << "\nУдаление предмета\n";
    cout << "0 - возврат на шаг назад\n\n";

    while (true) {
        SubjectNode* cur = structure.getSubjectHead();
        if (cur == nullptr) {
            cout << "Список предметов пуст.\n";
            break;
        }

        cout << "Доступные для удаления учебные предметы:\n";
        while (cur != nullptr) {
            cout << "  • " << cur->subjectName;
            if (cur->eventHead != nullptr) {
                cout << " (формы контроля: ";
                EventNode* ev = cur->eventHead;
                while (ev != nullptr) {
                    cout << "[" << ev->eventName << "] ";
                    ev = ev->next;
                }
                cout << ")";
            }
            else {
                cout << " (нет форм контроля)";
            }
            cout << "\n";
            cur = cur->next;
        }
        cout << "\n";

        string raw = readLine("Введите предмет для удаления (0 - назад в главное меню): ");
        string subject = trim(raw);

        if (subject == "0") {
            cout << "-> Возврат в главное меню.\n";
            break;
        }

        SubjectNode* targetNode = structure.findSubject(subject);
        if (targetNode == nullptr) {
            cout << "Ошибка: предмет \"" << subject << "\" не найден.\n\n";
            continue;
        }

        if (targetNode->eventHead != nullptr) {
            cout << "\nВнимание: у предмета \"" << targetNode->subjectName << "\" есть связанные формы контроля.\n";
            string confirm = trim(readLine("Удалить предмет вместе со всеми его формами контроля? (1 - да, 0 - отмена): "));
            if (confirm != "1") {
                cout << "Удаление отменено.\n\n";
                continue;
            }
        }

        structure.deleteSubject(subject);
        cout << "\n";
    }

    pressAnyKey();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: валидирует имя файла, проверяет перезапись и сохраняет данные на диск.
// Выход: нет.
void handleSaveToFile(StudyStructure& structure) {
    if (structure.getSubjectHead() == nullptr) {
        cout << "\nСтруктура пуста. Сохранение отменено.\n";
        pressAnyKey();
        return;
    }

    cout << "\nСохранение в файл\n";
    cout << "0 - возврат на шаг назад\n\n";

    while (true) {
        string raw = readLine("Введите имя файла (Enter - data.txt, 0 - отмена): ");
        string filename = trim(raw);

        if (filename == "0") {
            cout << "-> Сохранение отменено. Возврат в главное меню.\n";
            pressAnyKey();
            return;
        }

        if (filename.empty()) filename = "data.txt";
        if (filename.find(".txt") == string::npos) filename += ".txt";

        string errorMsg;
        if (!isValidFileName(filename, errorMsg)) {
            cout << errorMsg << "\n\n";
            continue;
        }

        if (fileExists(filename)) {
            string freeName = getIndexedFilename(filename);

            cout << "\nФайл \"" << filename << "\" уже существует на диске.\n";
            cout << "Выберите действие:\n";
            cout << "  1. Перезаписать файл\n";
            cout << "  2. Создать новый файл с названием \"" << freeName << "\"\n";
            cout << "  0. Отмена\n";

            string choice = trim(readLine("Ваш выбор: "));

            if (choice == "1") {
                structure.saveToFile(filename);
                pressAnyKey();
                return;
            }
            else if (choice == "2") {
                structure.saveToFile(freeName);
                pressAnyKey();
                return;
            }
            else {
                cout << "-> Сохранение отменено. Возврат в главное меню.\n";
                pressAnyKey();
                return;
            }
        }
        else {
            structure.saveToFile(filename);
            pressAnyKey();
            return;
        }
    }
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: проверяет существование файла в цикле, запрашивает режим импорта и заполняет структуру.
// Выход: структура обновлена.
void handleLoadFromFile(StudyStructure& structure) {
    cout << "\nЗагрузка из файла\n";
    cout << "0 - возврат на шаг назад\n\n";

    while (true) {
        string raw = readLine("Введите имя файла (Enter - test.txt, 0 - отмена): ");
        string filename = trim(raw);

        if (filename == "0") {
            cout << "-> Загрузка отменена. Возврат в главное меню.\n";
            pressAnyKey();
            return;
        }

        if (filename.empty()) filename = "test.txt";
        if (filename.find(".txt") == string::npos) filename += ".txt";

        string errorMsg;
        if (!isValidFileName(filename, errorMsg)) {
            cout << errorMsg << "\n\n";
            continue;
        }

        if (!fileExists(filename)) {
            cout << "Ошибка: файл \"" << filename << "\" не найден на диске.\n\n";
            continue;
        }

        while (true) {
            cout << "\nВыберите режим загрузки:\n";
            cout << "  1. Перезаписать текущую ведомость\n";
            cout << "  2. Дополнить текущую ведомость (слияние без дубликатов)\n";
            cout << "  0. Отмена\n";

            string mode = trim(readLine("Ваш выбор: "));
            if (mode == "0") {
                cout << "-> Загрузка отменена. Возврат в главное меню.\n";
                pressAnyKey();
                return;
            }

            if (mode == "1") {
                structure.clear();
                structure.loadFromFile(filename);
                pressAnyKey();
                return;
            }
            else if (mode == "2") {
                structure.loadFromFile(filename);
                pressAnyKey();
                return;
            }
            else {
                cout << "Неверный выбор режима, попробуйте снова.\n";
            }
        }
    }
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: запрашивает направление (по возрастанию/убыванию) и сортирует предметы.
// Выход: нет.
void handleSortSubjects(StudyStructure& structure) {
    if (structure.getSubjectHead() == nullptr) {
        cout << "\nВ структуре нет предметов для сортировки.\n";
        pressAnyKey();
        return;
    }

    cout << "\nСортировка учебных предметов (лексикографически)\n";
    cout << "0 - возврат на шаг назад\n\n";

    while (true) {
        cout << "Выберите направление сортировки:\n";
        cout << "  1. По возрастанию\n";
        cout << "  2. По убыванию\n";
        cout << "  0. Назад в главное меню\n";

        string choice = trim(readLine("Ваш выбор: "));
        if (choice == "0") {
            cout << "-> Возврат в главное меню.\n";
            break;
        }

        if (choice == "1") {
            structure.sortSubjects(true);
            cout << "\nПредметы успешно отсортированы по возрастанию.\n";
            break;
        }
        else if (choice == "2") {
            structure.sortSubjects(false);
            cout << "\nПредметы успешно отсортированы по убыванию.\n";
            break;
        }
        else {
            cout << "Неверный выбор. Введите 1, 2 или 0.\n\n";
        }
    }

    pressAnyKey();
}

// Вход: ссылка на структуру StudyStructure.
// Что делает: циклически запрашивает форму контроля, приводит к верхнему регистру и ищет предметы до ввода 0.
// Выход: нет.
void handleSearchByEvent(StudyStructure& structure) {
    if (structure.getSubjectHead() == nullptr) {
        cout << "\nВ структуре нет предметов для поиска.\n";
        pressAnyKey();
        return;
    }

    cout << "\nПоиск предметов по форме контроля\n";
    cout << "0 - возврат на шаг назад\n\n";

    while (true) {
        string rawEvent = readLine("Введите форму контроля для поиска (0 - назад в главное меню): ");
        string event = trim(rawEvent);

        if (event == "0") {
            cout << "-> Возврат в главное меню.\n";
            break;
        }

        string errorMsg;
        if (!isValidEventName(event, errorMsg)) {
            cout << errorMsg << "\n\n";
            continue;
        }

        // Автоматическое приведение к верхнему регистру
        event = toUpperString(event);

        int count = structure.findSubjectsByEvent(event);
        if (count == 0) {
            cout << "\nПредметы с формой контроля \"" << event << "\" не найдены.\n\n";
        }
        else {
            cout << "Всего найдено предметов: " << count << "\n\n";
        }
    }

    pressAnyKey();
}