#pragma once
#include <iostream>
#include "parser.h"

using namespace std;

struct Node { // односвязный список колонок
    string column; // колонка
    Node* next; // указатель на следующую колонку
};

struct tNode { // односвязный список таблиц
    string table; // значение таблицы
    Node* column; // указатель на первую колонку
    tNode* next; // указатель на следующую таблицу
};

bool isLocked(const string& tableName, const string& schemeName); // проверка блокировки таблицы
void locker(const string& tableName, const string& schemeName); // изменение состояния блокировки