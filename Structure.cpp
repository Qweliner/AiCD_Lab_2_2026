//Structure.cpp
#include "Structure.h"
#include <sstream>

// Вход: символ.
// Что делает: переводит символ CP1251 в верхний регистр.
// Выход: символ в верхнем регистре.
char toUpperChar(unsigned char c) {
    if (c >= 'a' && c <= 'z') return (char)(c - 32);
    if (c >= 0xE0 && c <= 0xFF) return (char)(c - 32); // а-я в CP1251
    if (c == 0xB8) return (char)0xA8;
    return (char)c;
}

// Вход: символ.
// Что делает: переводит символ CP1251 в нижний регистр.
// Выход: символ в нижнем регистре.
char toLowerChar(unsigned char c) {
    if (c >= 'A' && c <= 'Z') return (char)(c + 32);
    if (c >= 0xC0 && c <= 0xDF) return (char)(c + 32); // А-Я в CP1251
    if (c == 0xA8) return (char)0xB8;
    return (char)c;
}

// Вход: строка.
// Что делает: переводит все буквы строки в верхний регистр.
// Выход: строка в верхнем регистре.
string toUpperString(string s) {
    for (size_t i = 0; i < s.length(); i++) {
        s[i] = toUpperChar((unsigned char)s[i]);
    }
    return s;
}

// Вход: две строки.
// Что делает: посимвольно сравнивает строки без учета регистра.
// Выход: true если строки равны, иначе false.
bool iequals(const string& a, const string& b) {
    if (a.length() != b.length()) return false;
    for (size_t i = 0; i < a.length(); i++) {
        if (toLowerChar((unsigned char)a[i]) != toLowerChar((unsigned char)b[i])) {
            return false;
        }
    }
    return true;
}

// Вход: нет.
// Что делает: конструктор, обнуляет указатель на начало структуры.
// Выход: объект инициализирован.
StudyStructure::StudyStructure() {
    subjectHead = nullptr;
}

// Вход: нет.
// Что делает: деструктор, вызывает полную очистку памяти.
// Выход: память освобождена.
StudyStructure::~StudyStructure() {
    clear();
}

// Вход: название предмета.
// Что делает: выделяет память под узел предмета и вставляет его в конец списка.
// Выход: структура обновлена.
void StudyStructure::addSubject(string subject) {
    if (hasSubject(subject)) return;

    SubjectNode* newNode = new SubjectNode;
    newNode->subjectName = subject;
    newNode->eventHead = nullptr;
    newNode->next = nullptr;

    if (subjectHead == nullptr) {
        subjectHead = newNode;
        return;
    }

    SubjectNode* cur = subjectHead;
    while (cur->next != nullptr) {
        cur = cur->next;
    }
    cur->next = newNode;
}

// Вход: название предмета, форма контроля.
// Что делает: переводит форму контроля в верхний регистр и добавляет в конец ветвей предмета.
// Выход: структура обновлена.
void StudyStructure::addEvent(string subject, string event) {
    SubjectNode* curSubj = findSubject(subject);
    if (curSubj == nullptr) return;

    event = toUpperString(event);

    if (hasEvent(subject, event)) return;

    EventNode* newEvent = new EventNode;
    newEvent->eventName = event;
    newEvent->next = nullptr;

    if (curSubj->eventHead == nullptr) {
        curSubj->eventHead = newEvent;
        return;
    }

    EventNode* curEvent = curSubj->eventHead;
    while (curEvent->next != nullptr) {
        curEvent = curEvent->next;
    }
    curEvent->next = newEvent;
}

// Вход: название предмета.
// Что делает: проверяет существование предмета в списке.
// Выход: true если найден, иначе false.
bool StudyStructure::hasSubject(string subject) {
    return findSubject(subject) != nullptr;
}

// Вход: название предмета, форма контроля.
// Что делает: проверяет наличие формы контроля у конкретного предмета.
// Выход: true если найдена, иначе false.
bool StudyStructure::hasEvent(string subject, string event) {
    SubjectNode* curSubj = findSubject(subject);
    if (curSubj == nullptr) return false;

    EventNode* curEvent = curSubj->eventHead;
    while (curEvent != nullptr) {
        if (iequals(curEvent->eventName, event)) return true;
        curEvent = curEvent->next;
    }
    return false;
}

// Вход: название предмета.
// Что делает: ищет предмет по имени без учета регистра.
// Выход: указатель на узел предмета либо nullptr.
SubjectNode* StudyStructure::findSubject(string subject) {
    SubjectNode* cur = subjectHead;
    while (cur != nullptr) {
        if (iequals(cur->subjectName, subject)) return cur;
        cur = cur->next;
    }
    return nullptr;
}

// Вход: нет.
// Что делает: возвращает голову списка предметов.
// Выход: указатель на первый предмет.
SubjectNode* StudyStructure::getSubjectHead() const {
    return subjectHead;
}

// Вход: нет.
// Что делает: выводит структуру на консоль (предметы вниз, формы контроля вправо).
// Выход: нет.
void StudyStructure::display() {
    if (subjectHead == nullptr) {
        cout << "Структура пуста.\n";
        return;
    }

    SubjectNode* curSubj = subjectHead;
    while (curSubj != nullptr) {
        cout << "[" << curSubj->subjectName << "]";

        EventNode* curEvent = curSubj->eventHead;
        while (curEvent != nullptr) {
            cout << " ---> [" << curEvent->eventName << "]";
            curEvent = curEvent->next;
        }
        cout << "\n";

        if (curSubj->next != nullptr) {
            cout << "  |\n";
        }

        curSubj = curSubj->next;
    }
}

// Вход: название предмета, форма контроля.
// Что делает: перевязывает указатели и удаляет узел формы контроля из ветви предмета.
// Выход: структура обновлена.
void StudyStructure::deleteEvent(string subject, string event) {
    SubjectNode* curSubj = findSubject(subject);
    if (curSubj == nullptr) return;

    EventNode* cur = curSubj->eventHead;
    EventNode* prev = nullptr;

    while (cur != nullptr) {
        if (iequals(cur->eventName, event)) break;
        prev = cur;
        cur = cur->next;
    }

    if (cur == nullptr) return;

    if (prev == nullptr) {
        curSubj->eventHead = cur->next;
    }
    else {
        prev->next = cur->next;
    }

    string name = cur->eventName;
    delete cur;
    cout << "Форма контроля \"" << name << "\" удалена.\n";
}

// Вход: название предмета.
// Что делает: удаляет предмет из ствола и очищает память всех связанных форм контроля.
// Выход: структура обновлена.
void StudyStructure::deleteSubject(string subject) {
    SubjectNode* cur = subjectHead;
    SubjectNode* prev = nullptr;

    while (cur != nullptr) {
        if (iequals(cur->subjectName, subject)) break;
        prev = cur;
        cur = cur->next;
    }

    if (cur == nullptr) return;

    if (prev == nullptr) {
        subjectHead = cur->next;
    }
    else {
        prev->next = cur->next;
    }

    EventNode* ev = cur->eventHead;
    while (ev != nullptr) {
        EventNode* temp = ev;
        ev = ev->next;
        delete temp;
    }

    string name = cur->subjectName;
    delete cur;
    cout << "Предмет \"" << name << "\" удален из структуры.\n";
}

// Вход: имя файла.
// Что делает: сохраняет структуру в текстовый файл построчно.
// Выход: нет.
void StudyStructure::saveToFile(string filename) {
    ofstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Ошибка: не удалось открыть файл для записи.\n";
        return;
    }

    SubjectNode* curSubj = subjectHead;
    while (curSubj != nullptr) {
        file << curSubj->subjectName;
        EventNode* curEvent = curSubj->eventHead;
        while (curEvent != nullptr) {
            file << " " << curEvent->eventName;
            curEvent = curEvent->next;
        }
        file << "\n";
        curSubj = curSubj->next;
    }

    file.close();
    cout << "Данные сохранены в файл: " << filename << "\n";
}

// Вход: имя файла.
// Что делает: парсит строки из файла и восстанавливает динамическую структуру в памяти.
// Выход: структура заполнена.
void StudyStructure::loadFromFile(string filename) {
    ifstream file(filename.c_str());
    if (!file.is_open()) {
        cout << "Ошибка: файл \"" << filename << "\" не найден.\n";
        return;
    }

    string line;
    int count = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string subject;
        if (!(ss >> subject)) continue;

        addSubject(subject);
        string event;
        while (ss >> event) {
            addEvent(subject, event);
        }
        count++;
    }

    file.close();
    if (count > 0) {
        cout << "Успешно загружено предметов: " << count << "\n";
    }
    else {
        cout << "Файл пуст или содержит некорректные данные.\n";
    }
}

// Вход: нет.
// Что делает: обходит все уровни структуры и вызывает delete для каждого узла.
// Выход: память освобождена.
void StudyStructure::clear() {
    while (subjectHead != nullptr) {
        SubjectNode* tempSubj = subjectHead;
        subjectHead = subjectHead->next;

        EventNode* curEvent = tempSubj->eventHead;
        while (curEvent != nullptr) {
            EventNode* tempEvent = curEvent;
            curEvent = curEvent->next;
            delete tempEvent;
        }
        delete tempSubj;
    }
}

// Вход: флаг направления сортировки (true - по возрастанию, false - по убыванию).
// Что делает: сортирует предметы методом выбора лексикографически с перемещением их форм контроля.
// Выход: структура упорядочена.
void StudyStructure::sortSubjects(bool ascending) {
    if (subjectHead == nullptr || subjectHead->next == nullptr) {
        return; // 0 или 1 элемент — сортировать нечего
    }

    SubjectNode* i = subjectHead;
    while (i != nullptr) {
        SubjectNode* target = i;
        SubjectNode* j = i->next;

        while (j != nullptr) {
            // Лексикографическое (регистрозависимое) сравнение названий предметов
            if (ascending) {
                if (j->subjectName < target->subjectName) {
                    target = j;
                }
            }
            else {
                if (j->subjectName > target->subjectName) {
                    target = j;
                }
            }
            j = j->next;
        }

        if (target != i) {
            // Обмен названия предмета
            string tempName = i->subjectName;
            i->subjectName = target->subjectName;
            target->subjectName = tempName;

            // Обмен указателя на формы контроля (ветви перемещаются вместе со своим предметом)
            EventNode* tempHead = i->eventHead;
            i->eventHead = target->eventHead;
            target->eventHead = tempHead;
        }

        i = i->next;
    }
}

// Вход: название формы контроля.
// Что делает: приводит запрос к верхнему регистру и выводит все предметы, содержащие данную форму контроля.
// Выход: количество найденных предметов.
int StudyStructure::findSubjectsByEvent(string event) {
    event = toUpperString(event);
    int foundCount = 0;

    SubjectNode* cur = subjectHead;
    while (cur != nullptr) {
        if (hasEvent(cur->subjectName, event)) {
            if (foundCount == 0) {
                cout << "\nПо форме контроля \"" << event << "\" найдены предметы:\n";
            }
            cout << "  • " << cur->subjectName << "\n";
            foundCount++;
        }
        cur = cur->next;
    }

    return foundCount;
}