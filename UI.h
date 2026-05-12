#pragma once
#include <string>
#include <vector>

// Безопасный ввод: перехватывает нажатия клавиш, блокирует запрещенные символы 
// например, для имен файлов и обрабатывает выход через ESC.
// prompt - текст перед вводом пользователя, buffer - введенное пользователем
bool safeInput(std::string& buffer, const std::string& prompt, bool onlyDigits = false);

// Вывод приветственного меню в консоль.
void Greeting();
// Вывод меню
void printMenu();
// Ожидание нажатия от пользователя
void pressAnyKey();