//Structure.cpp
#include "Structure.h"
#include <sstream>

// Вспомогательная функция для перевода символов CP1251 в нижний регистр
char tolowerCP1251(unsigned char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    if (c >= 0xC0 && c <= 0xDF) return c + 32;
    if (c == 0xA8) return 0xB8; // 'Ё' -> 'ё'
    return c;
}

// Вспомогательная функция для регистронезависимого сравнения строк
bool iequals(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (tolowerCP1251(static_cast<unsigned char>(a[i])) != tolowerCP1251(static_cast<unsigned char>(b[i]))) {
            return false;
        }
    }
    return true;
}

// Вход: нет.
// Что делает: инициализирует пустую структуру.
// Выход: указатель на начало списка равен nullptr.
StudyStructure::StudyStructure() {
    subjectHead = nullptr;
}

// Вход: нет.
// Что делает: вызывает очистку памяти при удалении объекта.
// Выход: память освобождена.
StudyStructure::~StudyStructure() {
    clear();
}

// Вход: название предмета.
// Что делает: создает узел предмета и добавляет его в конец списка, если такого предмета еще нет.
// Выход: структура обновлена.
void StudyStructure::addSubject(string subject) {
    // Если предмет уже существует, новый узел не создаем
    if (hasSubject(subject)) {
        return;
    }

    SubjectNode* newNode = new SubjectNode;
    newNode->subjectName = subject;
    newNode->eventHead = nullptr;
    newNode->next = nullptr;

    if (subjectHead == nullptr) {
        subjectHead = newNode;
        return;
    }

    SubjectNode* current = subjectHead;
    while (current->next != nullptr) {
        current = current->next;
    }
    current->next = newNode;
}

// Вход: название предмета, тип события.
// Что делает: ищет предмет, затем добавляет уникальное событие в конец списка событий этого предмета.
// Выход: структура обновлена.
void StudyStructure::addEvent(string subject, string event) {
    SubjectNode* currentSubject = findSubject(subject);
    if (currentSubject == nullptr) {
        cout << "Предмет не найден\n";
        return;
    }

    // Проверяем, существует ли уже такое событие у данного предмета
    if (hasEvent(subject, event)) {
        return;
    }

    EventNode* newEvent = new EventNode;
    newEvent->eventName = event;
    newEvent->next = nullptr;

    if (currentSubject->eventHead == nullptr) {
        currentSubject->eventHead = newEvent;
        return;
    }

    EventNode* currentEvent = currentSubject->eventHead;
    while (currentEvent->next != nullptr) {
        currentEvent = currentEvent->next;
    }
    currentEvent->next = newEvent;
}

// Вход: название предмета.
// Что делает: проверяет существование предмета в структуре (без учета регистра).
// Выход: true, если найден, иначе false.
bool StudyStructure::hasSubject(string subject) {
    return findSubject(subject) != nullptr;
}

// Вход: название предмета, название события.
// Что делает: проверяет, привязано ли уже такое событие к указанному предмету.
// Выход: true, если событие найдено, иначе false.
bool StudyStructure::hasEvent(string subject, string event) {
    return findEvent(subject, event) != nullptr;
}

// Вход: название предмета.
// Что делает: выполняет регистронезависимый поиск предмета.
// Выход: указатель на узел предмета, либо nullptr в случае отсутствия совпадений.
SubjectNode* StudyStructure::findSubject(string subject) {
    SubjectNode* current = subjectHead;
    while (current != nullptr) {
        if (iequals(current->subjectName, subject)) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Вход: название предмета, название события.
// Что делает: выполняет регистронезависимый поиск события у указанного предмета.
// Выход: указатель на узел события, либо nullptr в случае отсутствия совпадений.
EventNode* StudyStructure::findEvent(string subject, string event) {
    SubjectNode* subj = findSubject(subject);
    if (subj == nullptr) return nullptr;

    EventNode* current = subj->eventHead;
    while (current != nullptr) {
        if (iequals(current->eventName, event)) {
            return current;
        }
        current = current->next;
    }
    return nullptr;
}

// Вход: нет.
// Что делает: проходит по всем предметам и их событиям, выводя их в консоль.
// Выход: нет (вывод в консоль).
void StudyStructure::display() {
    if (subjectHead == nullptr) {
        cout << "Структура пустая\n";
        return;
    }

    SubjectNode* currentSubject = subjectHead;
    int index = 1;
    while (currentSubject != nullptr) {
        cout << "[" << index << "] " << currentSubject->subjectName << endl;
        cout << "     |\n";

        EventNode* currentEvent = currentSubject->eventHead;
        while (currentEvent != nullptr) {
            cout << "     ---> " << currentEvent->eventName << endl;
            currentEvent = currentEvent->next;
        }
        cout << endl;
        currentSubject = currentSubject->next;
        index++;
    }
}

// Вход: название предмета, тип события.
// Что делает: находит предмет, затем ищет событие в списке (без учета регистра) и удаляет его, перевязывая указатели.
// Выход: структура обновлена.
void StudyStructure::deleteEvent(string subject, string event) {
    SubjectNode* currentSubject = findSubject(subject);
    if (currentSubject == nullptr) {
        cout << "Предмет не найден\n";
        return;
    }

    EventNode* current = currentSubject->eventHead;
    EventNode* previous = nullptr;

    while (current != nullptr) {
        if (iequals(current->eventName, event)) break;
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Событие не найдено\n";
        return;
    }

    if (previous == nullptr) currentSubject->eventHead = current->next;
    else previous->next = current->next;

    delete current;
    cout << "Событие \"" << event << "\" удалено\n";
}

// Вход: название предмета.
// Что делает: удаляет предмет (без учета регистра), предварительно освобождая память всех его событий.
// Выход: структура обновлена.
void StudyStructure::deleteSubject(string subject) {
    SubjectNode* current = findSubject(subject);
    if (current == nullptr) {
        cout << "Предмет не найден\n";
        return;
    }

    // Если удаляемый узел является головой списка
    if (subjectHead == current) {
        subjectHead = current->next;
    }
    else {
        SubjectNode* temp = subjectHead;
        while (temp != nullptr && temp->next != current) {
            temp = temp->next;
        }
        if (temp != nullptr) {
            temp->next = current->next;
        }
    }

    // Очистка событий предмета
    EventNode* event = current->eventHead;
    while (event != nullptr) {
        EventNode* temp = event;
        event = event->next;
        delete temp;
    }

    string deletedName = current->subjectName;
    delete current;
    cout << "Предмет \"" << deletedName << "\" удален\n";
}

// Вход: имя файла.
// Что делает: сохраняет структуру в текстовый вид.
// Выход: нет (запись в файл).
void StudyStructure::saveToFile(string filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла\n";
        return;
    }

    SubjectNode* currentSubject = subjectHead;
    while (currentSubject != nullptr) {
        file << currentSubject->subjectName;
        EventNode* currentEvent = currentSubject->eventHead;
        while (currentEvent != nullptr) {
            file << " " << currentEvent->eventName;
            currentEvent = currentEvent->next;
        }
        file << endl;
        currentSubject = currentSubject->next;
    }
    file.close();
    cout << "Сохранение завершено\n";
}

// Вход: имя файла.
// Что делает: читает файл построчно, разбирает слова и восстанавливает структуру в памяти. 
//             Игнорирует пустые строки и не создает пустые элементы.
// Выход: структура наполнена данными.
void StudyStructure::loadFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла\n";
        return;
    }

    string line;
    bool loadedAny = false;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string subject;
        if (!(ss >> subject)) continue;

        addSubject(subject);
        loadedAny = true;

        string event;
        while (ss >> event) {
            addEvent(subject, event);
        }
    }
    file.close();
    if (loadedAny) {
        cout << "Загрузка завершена\n";
    }
    else {
        cout << "Файл пуст или не содержит корректных данных\n";
    }
}

// Вход: нет.
// Что делает: проходит по всем уровням структуры и вызывает delete для каждого узла.
// Выход: память очищена.
void StudyStructure::clear() {
    while (subjectHead != nullptr) {
        SubjectNode* tempSubject = subjectHead;
        subjectHead = subjectHead->next;

        EventNode* currentEvent = tempSubject->eventHead;
        while (currentEvent != nullptr) {
            EventNode* tempEvent = currentEvent;
            currentEvent = currentEvent->next;
            delete tempEvent;
        }
        delete tempSubject;
    }
}