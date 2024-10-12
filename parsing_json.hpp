#pragma once
#include <iostream>
#include <fstream>
#include <filesystem> // работа с файловой системой
#include "json.hpp"

using namespace std;
using json = nlohmann::json;
namespace fs = filesystem;

int tableSize; // размер колонок

void removeDirectory(const fs::path& directoryPath) { // удаление директории
    if (fs::exists(directoryPath)) { // проверка, существует ли она вообще
        fs::remove_all(directoryPath); // удаление
    }
}

void createDirectoriesAndFiles(const fs::path& schemePath, const json& structure) { // создание полной директории и файлов
    for (const auto& table : structure.items()) { // цикл по всем элементам в структуре json
        fs::path tablePath = schemePath / table.key(); // key - имя таблицы
        if (!fs::create_directory(tablePath)) {
            cerr << "Не удалось создать директорию: " << tablePath << endl;
            return;
        }
        cout << "Создана директория: " << tablePath << endl;

        fs::path csvFilePath = tablePath / (table.key() + ".csv"); // создаём csv файлы с названием таблиц
        ofstream csvFile(csvFilePath);
        if (!csvFile.is_open()) {
            cerr << "Не удалось создать файл: " << csvFilePath << endl;
            return;
        }
        csvFile.close();
        cout << "Создан файл: " << csvFilePath << endl;
    }
}

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
    removeDirectory(schemePath); // удаляем, чтобы заново создать директорию
    if (!fs::create_directory(schemePath)) { // проверка
        cerr << "Не удалось создать директорию: " << schemePath << endl;
        return;
    }
    cout << "Создана директория: " << schemePath << endl;
    if (jparsed.contains("structure")) { // наполнение директории
        createDirectoriesAndFiles(schemePath, jparsed["structure"]);
    }
    tableSize = jparsed["tuples_limit"]; // вытаскиваем ограничения по строкам
    cout << endl << tableSize;
}