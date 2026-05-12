#include "UI.h"
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
    cout << "========================================\n";
    cout << " Лабораторная работа по АиСД\n";
    cout << " Динамическая структура\n";
    cout << "========================================\n";
}

// Вывод меню
void printMenu() {
    cout << "1. Добавить ствол\n2. Добавить ветку\n3. Вывод структуры\n4. Удалить ветку\n5. Удалить ствол\n6. Сохранить в файл\n7. Загрузить из файла\n0. Выход";
}

// Ожидание нажатия от пользователя
void pressAnyKey() {
    cout << "\nНажмите любую клавишу...";
    _getch();
}

