#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include "parsing_json.hpp"

using namespace std;
namespace fs = filesystem;

/*struct valueNode { // односвязный список значений
    string value; // значение
    valueNode* next; // указатель на следующее значение
};*/

struct Node { // односвязный список колонок
    string column; // колонка
    //valueNode* values; // указатель на первое значение колонки
    Node* next; // указатель на следующую колонку
};

struct tNode { // односвязный список таблиц
    string table; // значение таблицы
    Node* column; // указатель на первую колонку
    tNode* next; // указатель на следующую таблицу
};

tNode* createTable(const string& tableName) { // создание новой таблицы
    tNode* newTable = new tNode{tableName, nullptr, nullptr};
    return newTable;
}

int isLocked(const string& table, const fs::path& tablePath) { // проверка состояния блокировки
    ifstream file(tablePath);
    if (!file.is_open()) { // проверка
        cerr << "Не удалось открыть таблицу.\n";
        return -1;
    }
    string line;
    file >> line; // чтение данных из файла
    file.close();
    if (line == "locked") {
        return 1;
    }
    return 0;
}

void locker(const string& table, const fs::path& tablePath) { // изменение состояния блокировки
    ofstream file(tablePath);
    if (!file.is_open()) { // проверка
        cerr << "Не удалось открыть файл.\n";
        return;
    }
    if (isLocked(table, tablePath) == 1) { // если таблица была заблокирована
        file << "unlocked";
    }
    if (isLocked(table, tablePath) == 0) { // если таблица была разблокирована
        file << "locked";
    }
    file.close();
}

bool isTableExist(tNode* head, const string& tableName) { // проверка на существование таблицы
    tNode* current = head; // указатель на первый узел
    while (current) {
        if (current->table == tableName) { // поиск заданной таблицы
            return true;
        }
        current = current->next;
    }
    return false;
}

void insert(const string& command) { // вставка
    istringstream iss(command); // создание объекта для разбора команды
    string word; // текущее слово
    int pos = 1; // текущая позиция слова в команде
    while (iss >> word) {
        pos++;
        if (pos == 2 && word != "INTO") {
            cerr << "Неизвестная команда.\n";
            return;
        }
    }
}
