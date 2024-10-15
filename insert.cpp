#include "structure.h"

bool isLocked(const string& tableName, const string& schemeName) { // проверка блокировки таблицы
    string fileName = "/home/kali/Documents/GitHub/practice1_2024/" + schemeName + "/" + tableName + "/" + tableName + "_lock.txt";
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл.\n";
    }
    string current; // чтение текущего значения блокировки
    file >> current;
    file.close();
    if (current == "locked") {
        return true; // заблокирована
    }
    return false; // разблокирована
}

void locker(const string& tableName, const string& schemeName) { // изменение состояния блокировки
    string fileName = "/home/kali/Documents/GitHub/practice1_2024/" + schemeName + "/" + tableName + "/" + tableName + "_lock.txt";
    ifstream fileIn(fileName);
    if (!fileIn.is_open()) {
        cerr << "Не удалось открыть файл.\n";
        return;
    }
    string current; // чтение текущего значения блокировки
    fileIn >> current;
    fileIn.close();
    ofstream fileOut(fileName); // перезаписываем файл
    if (!fileOut.is_open()) {
        cerr << "Не удалось открыть файл.\n";
        return;
    }
    if (current == "locked") { // если таблица заблокирована, меняем на разблокирована
        fileOut << "unlocked";
    } else {
        fileOut << "locked"; // если была разблокирована, становится заблокирована
    }
    fileOut.close();
}