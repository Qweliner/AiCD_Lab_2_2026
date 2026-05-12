#include "FileManager.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <windows.h> 

// Вход: строка с именем файла.
// Что делает: пытается открыть файл для чтения. Это надежный способ проверить его наличие.
// Выход: true, если файл существует и доступен, иначе false.
bool fileExists(const string& filename) {
    ifstream file(filename.c_str());
    return file.is_open();
}

// Вход: нет.
// Что делает: сканирует папку с программой, ищет текстовые файлы (кроме инструкции).
// Выход: массив (вектор) с именами найденных .txt файлов.
vector<string> getAvailableFiles() {
    vector<string> files;
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA("*.txt", &findData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string name = findData.cFileName;
            // Инструкцию исключаем из поиска
            if (name.find("instr") == string::npos) {
                files.push_back(name);
            }
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
    return files;
}

// Вход: базовое имя файла (например, "Отчет").
// Что делает: подбирает свободное имя, добавляя цифры в скобках, если файл уже есть (Отчет(1).txt).
// Выход: уникальное имя файла, которое точно ничего не перезапишет.
string getIndexedName(const string& baseName) {
    string finalName = baseName + ".txt";
    if (!fileExists(finalName)) return finalName;
    int index = 1;
    while (true) {
        string testName = baseName + "(" + to_string(index) + ").txt";
        if (!fileExists(testName)) return testName;
        index++;
    }
}