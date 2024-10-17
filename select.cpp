#include "select.h"

void splitDot(const string& word, string& table, string& column, tableJson& tjs) {
    bool dot = false; // поиск точки
    for (size_t i = 0; i < word.size(); i++) {
        if (word[i] == '.') {
            dot = true;
            continue;
        }
        if (!dot) { // разделяем таблицу и колонку
            table += word[i];
        }
        else {
            column += word[i];
        }
    }
    if (!dot) { // если точка не найдена
        cerr << "Некорректная команда.\n";
        return;
    }
    if (isTableExist(table, tjs.tablehead) == false) { // проверка на существование таблицы
        cerr << "Такой таблицы нет.\n";
        return;
    }
    if (isColumnExist(table, column, tjs.tablehead) == false) { // проверка на сущетсвование колонки
        cerr << "Такой колонки нет.\n";
        return;
    }
}

void select(const string& command, tableJson& tjs) { // выбор данных
    istringstream iss(command); // поток ввода для обработки строки команды
    string word;
    iss >> word; // "SELECT"
    iss >> word; // "таблица 1"
    string table1, column1; // строка для 1 таблицы и колонки
    splitDot(word, table1, column1, tjs); // разделяем таблицу и колонку
}