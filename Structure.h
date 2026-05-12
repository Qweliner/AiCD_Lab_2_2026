#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Узел для хранения события (экзамен, зачет, КП)
struct EventNode {
    string eventName;
    EventNode* next;
};

// Узел для хранения предмета (ствол)
struct SubjectNode {
    string subjectName;
    EventNode* eventHead;
    SubjectNode* next;
};

// Класс для управления динамической структурой
class StudyStructure {
private:
    SubjectNode* subjectHead;

public:
    StudyStructure();
    ~StudyStructure();

    // Добавляет новый предмет в конец списка предметов
    void addSubject(string subject);

    // Добавляет событие (ветку) к существующему предмету
    void addEvent(string subject, string event);

    // Выводит всю структуру на экран
    void display();

    // Удаляет событие у указанного предмета
    void deleteEvent(string subject, string event);

    // Удаляет предмет целиком вместе со всеми его событиями
    void deleteSubject(string subject);

    // Записывает структуру в файл
    void saveToFile(string filename);

    // Читает структуру из файла
    void loadFromFile(string filename);

    // Полная очистка памяти
    void clear();
};