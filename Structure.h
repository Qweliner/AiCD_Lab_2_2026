//Structure.h
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

// Узел для хранения предмета
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

    // Добавляет новый предмет в конец списка предметов (с проверкой на уникальность)
    void addSubject(string subject);

    // Добавляет событие к существующему предмету (с проверкой на уникальность)
    void addEvent(string subject, string event);

    // Выводит всю структуру на экран
    void display();

    // Удаляет событие у указанного предмета
    void deleteEvent(string subject, string event);

    // Удаляет предмет целиком вместе со всеми его событиями
    void deleteSubject(string subject);

    // Проверяет существование предмета в списке
    bool hasSubject(string subject);

    // Проверяет существование события у конкретного предмета
    bool hasEvent(string subject, string event);

    // Ищет предмет (регистронезависимо) и возвращает указатель на него, либо nullptr
    SubjectNode* findSubject(string subject);

    // Ищет событие у конкретного предмета (регистронезависимо) и возвращает указатель на него, либо nullptr
    EventNode* findEvent(string subject, string event);

    // Возвращает указатель на начало списка предметов
    SubjectNode* getSubjectHead() const { return subjectHead; }

    // Записывает структуру в файл
    void saveToFile(string filename);

    // Читает структуру из файла
    void loadFromFile(string filename);

    // Полная очистка памяти
    void clear();
};