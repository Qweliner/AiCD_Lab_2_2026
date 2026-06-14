//UI.h
#pragma once
#include <string>
#include <vector>
#include "Structure.h" // Подключаем для передачи ссылки на StudyStructure в функции

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

// ФУНКЦИИ-ОБРАБОТЧИКИ ПУНКТОВ МЕНЮ (вынесенная из main логика)

// Запускает цикл добавления новых учебных предметов
void handleAddSubject(StudyStructure& structure);

// Запускает цикл добавления новых форм контроля к существующим предметам
void handleAddEvent(StudyStructure& structure);

// Выводит текущую ведомость предметов на экран
void handleDisplayStructure(StudyStructure& structure);

// Запускает цикл удаления отдельных форм контроля у предметов
void handleDeleteEvent(StudyStructure& structure);

// Запускает цикл полного удаления учебных предметов
void handleDeleteSubject(StudyStructure& structure);

// Запускает диалог сохранения структуры в текстовый файл
void handleSaveToFile(StudyStructure& structure);

// Запускает диалог загрузки данных из текстовых файлов
void handleLoadFromFile(StudyStructure& structure);