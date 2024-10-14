#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem> // работа с файловой системой
#include "json.hpp"
#include "rapidcsv.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;

struct Node { // односвязный список колонок
    string column; // колонка
    Node* next; // указатель на следующую колонку
};

struct tNode { // односвязный список таблиц
    string table; // значение таблицы
    Node* column; // указатель на первую колонку
    tNode* next; // указатель на следующую таблицу
};

struct tableJson {
    int tableSize; // размер колонок
    string schemeName; // имя схемы
    tNode* tablehead; // указатель на голову таблицы
};
