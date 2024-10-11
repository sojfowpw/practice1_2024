#pragma once
#include <iostream>
#include <fstream>
#include <filesystem> // работа с файловой системой
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

void parsing() {
    string filename = "schema.json"; // название файла
    ifstream file(filename); // открываем
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл: " << filename << endl;
        return;
    }
    string json_content; // содержимое
    string line; // строка
    while(getline(file, line)) { // чтение из файла
        json_content += line;
    }
    file.close();

    json jparsed; // хранение результата парсинга
    jparsed = json::parse(json_content); // парсинг в объект jparsed
    cout << "Парсинг json:" << endl;
    cout << jparsed.dump(4) << endl; // вывод с форматированием

    string schemeName = jparsed["name"]; // извлекаем имя схемы
    fs::path schemePath = fs::current_path() / schemeName; // формируем путь к директории
    if (!fs::create_directory(schemePath)) { // проверка
        cerr << "Не удалось создать директорию: " << schemePath << endl;
        return;
    }
    cout << "Создана директория: " << schemePath << endl;
    return;
}