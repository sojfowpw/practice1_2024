#include "delete.h"

bool isColumnExist(const string& tableName, const string& columnName, tableJson& tjs) { // поиск колонки
    string filePath = "/home/kali/Documents/GitHub/practice1_2024/" + tjs.schemeName + "/" + tableName + "/1.csv";
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл.\n";
    }
    string columns; // строка для чтения всех колонок
    getline(file, columns);
    file.close();
    string currentColumn; // текущая колонка
    for (size_t i = 0; i < columns.size(); i++) { // перебор колонок из строки
        if (columns[i] == ',') {
            if (currentColumn == columnName) {
                return true;
            }
            currentColumn = ""; // обновляем текущую строку
            continue;
        }
        currentColumn += columns[i];
    }
    return false;
}

void del(const string& command, tableJson& tjs) { // удаление
    istringstream iss(command); // поток ввода для обработки строки команды
    string word;
    iss >> word; // "DELETE"
    iss >> word; // "FROM"
    if (word != "FROM") {
        cerr << "Некорректная команда.\n";
        return;
    }
    string tableName; // имя таблицы из команды
    iss >> word; // название таблицы
    if (isTableExist(word, tjs.tablehead) == false) {
        cerr << "Такой таблицы нет.\n";
        return;
    }
    tableName = word;
    string secondCmd; // вторая строчка команды delete
    getline(cin, secondCmd);
    istringstream iss2(secondCmd); // поток ввода для обработки строки команды
    string word2;
    iss2 >> word2; // "WHERE"
    if (word2 != "WHERE") {
        cerr << "Некорректная команда.\n";
        return;
    }


    iss2 >> word2; // таблица и колонка
    string table, column; // названия таблицы и колонки из команды
    bool dot = false; // поиск точки 
    for (size_t i = 0; i < word2.size(); i++) {
        if (word2[i] == '.') {
            dot = true;
            continue;
        }
        if (!dot) { // разделяем таблицу и колонку
            table += word2[i];
        }
        else {
            column += word2[i];
        }
    }
    if (!dot) { // если точка не найдена
        cerr << "Некорректная команда.\n";
        return;
    }
    if (table != tableName) { // правильность таблицы
        cerr << "Некорректная команда.\n";
        return;
    }
    if (isColumnExist(tableName, column, tjs) == false) { // проверка на существование таблицы
        cerr << "Такой колонки нет.\n";
        return;
    }
    iss2 >> word2; // "="
    if (word2 != "=") {
        cerr << "Некорректная команда.\n";
        return;
    }
    string value; // удаляемое значение
    iss2 >> word2;
    if (word2[0] != '\'' || word2[word2.size() - 1] != '\'') { // проверка на кавычки
        cerr << "Некорректная команда.\n";
        return;
    }
    for (size_t i = 1; i < word2.size() - 1; i++) { // вытаскиваем значение из кавычек
        value += word2[i];
    }


    int amountCsv = 1; // ищем количество созданных csv файлов
    while (true) {
        string filePath = "/home/kali/Documents/GitHub/practice1_2024/" + tjs.schemeName + "/" + tableName + "/" + to_string(amountCsv) + ".csv";
        ifstream file(filePath);
        if (!file.is_open()) { // если файл нельзя открыть, его нет
            break;
        }
        file.close();
        amountCsv++;
    }
    for (size_t iCsv = 1; iCsv < amountCsv; iCsv++) { // просматриваем все созданные файлы csv
        string filePath = "/home/kali/Documents/GitHub/practice1_2024/" + tjs.schemeName + "/" + tableName + "/" + to_string(iCsv) + ".csv";
        rapidcsv::Document doc(filePath); // открываем файл
        int columnIndex = doc.GetColumnIdx(column); // считываем индекс искомой колонки
        size_t amountRow = doc.GetRowCount(); // считываем количество строк в файле
        for (size_t i = 0; i < amountRow; ++i) { // проходимся по строкам
            if (doc.GetCell<string>(columnIndex, i) == value) { // извлекаем значение (индекс колонки, номер строки)
                doc.RemoveRow(i); // удаляем строку
                --amountRow; // уменьшаем количество строк
                --i;
            }
        }
        doc.Save(filePath); 
    }
}