#include "Structure.h"

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
// Что делает: создает узел предмета и добавляет его в конец списка предметов.
// Выход: структура обновлена.
void StudyStructure::addSubject(string subject) {
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
// Что делает: ищет предмет, затем добавляет событие в конец списка событий этого предмета.
// Выход: структура обновлена.
void StudyStructure::addEvent(string subject, string event) {
    SubjectNode* currentSubject = subjectHead;
    while (currentSubject != nullptr) {
        if (currentSubject->subjectName == subject) break;
        currentSubject = currentSubject->next;
    }

    if (currentSubject == nullptr) {
        cout << "Предмет не найден\n";
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
// Что делает: находит предмет, затем ищет событие в списке и удаляет его, перевязывая указатели.
// Выход: структура обновлена.
void StudyStructure::deleteEvent(string subject, string event) {
    SubjectNode* currentSubject = subjectHead;
    while (currentSubject != nullptr) {
        if (currentSubject->subjectName == subject) break;
        currentSubject = currentSubject->next;
    }

    if (currentSubject == nullptr) {
        cout << "Предмет не найден\n";
        return;
    }

    EventNode* current = currentSubject->eventHead;
    EventNode* previous = nullptr;

    while (current != nullptr) {
        if (current->eventName == event) break;
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
    cout << "Событие удалено\n";
}

// Вход: название предмета.
// Что делает: удаляет предмет, предварительно освобождая память всех его событий.
// Выход: структура обновлена.
void StudyStructure::deleteSubject(string subject) {
    SubjectNode* current = subjectHead;
    SubjectNode* previous = nullptr;

    while (current != nullptr) {
        if (current->subjectName == subject) break;
        previous = current;
        current = current->next;
    }

    if (current == nullptr) {
        cout << "Предмет не найден\n";
        return;
    }

    if (previous == nullptr) subjectHead = current->next;
    else previous->next = current->next;

    // Очистка событий предмета
    EventNode* event = current->eventHead;
    while (event != nullptr) {
        EventNode* temp = event;
        event = event->next;
        delete temp;
    }

    delete current;
    cout << "Предмет удален\n";
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
// Что делает: читает файл, парсит пробелы и восстанавливает структуру в памяти.
// Выход: структура наполнена данными.
void StudyStructure::loadFromFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Ошибка открытия файла\n";
        return;
    }

    string subject;
    while (file >> subject) {
        addSubject(subject);
        string line;
        getline(file, line);
        string word = "";

        for (size_t i = 0; i < line.size(); i++) {
            if (line[i] == ' ') {
                if (!word.empty()) {
                    addEvent(subject, word);
                    word = "";
                }
            }
            else {
                word += line[i];
            }
        }
        if (!word.empty()) addEvent(subject, word);
    }
    file.close();
    cout << "Загрузка завершена\n";
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