#include "select.h"

void splitDot(const string& word, string& table, string& column, tableJson& tjs) {
    bool dot = false; // поиск точки
    for (size_t i = 0; i < word.size(); i++) {
        if (word[i] == '.') {
            dot = true;
            continue;
        }
        if (word[i] == ',') {
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

int countCsv(tableJson& tjs, string table) {
    int amountCsv = 1; // ищем количество созданных csv файлов
    while (true) {
        string filePath = "/home/kali/Documents/GitHub/practice1_2024/" + tjs.schemeName + "/" + table + "/" + to_string(amountCsv) + ".csv";
        ifstream file(filePath);
        if (!file.is_open()) { // если файл нельзя открыть, его нет
            break;
        }
        file.close();
        amountCsv++;
    }
    return amountCsv;
}

void select(const string& command, tableJson& tjs) { // выбор данных
    istringstream iss(command); // поток ввода для обработки строки команды
    string word;
    iss >> word; // "SELECT"
    iss >> word; // "таблица 1"
    string table1, column1; // строка для 1 таблицы и колонки
    splitDot(word, table1, column1, tjs); // разделяем таблицу и колонку
    iss >> word; // "таблица 2"
    string table2, column2; // строка для 2 таблицы и колонки
    splitDot(word, table2, column2, tjs); // разделяем таблицу и колонку

    string secondCmd; // вторая часть команды с FROM
    getline(cin, secondCmd); 
    istringstream iss2(secondCmd);
    iss2 >> word; // "FROM"
    if (word != "FROM") {
        cerr << "Некорректная команда.\n";
        return;
    }
    iss2 >> word; // таблица 1
    string tab1;
    for (size_t i = 0; i < word.size(); i++) {
        if (word[i] != ',') {
            tab1 += word[i];
        }
    }
    if (tab1 != table1) {
        cerr << "Некорректная команда.\n";
        return;
    }
    iss2 >> word; // таблица 2
    if (word != table2) {
        cerr << "Некорректная команда.\n";
        return;
    }

    int amountCsv1 = countCsv(tjs, table1);
    int amountCsv2 = countCsv(tjs, table2);
    for (size_t iCsv1 = 1; iCsv1 < amountCsv1; iCsv1++) {
        string filePath1 = "/home/kali/Documents/GitHub/practice1_2024/" + tjs.schemeName + "/" + table1 + "/" + to_string(iCsv1) + ".csv";
        rapidcsv::Document doc1(filePath1); // открываем файл
        int columnIndex1 = doc1.GetColumnIdx(column1); // считываем индекс искомой колонки
        size_t amountRow1 = doc1.GetRowCount(); // считываем количество строк в файле
        for (size_t i = 0; i < amountRow1; ++i) { // проходимся по строкам
            for (size_t iCsv2 = 1; iCsv2 < amountCsv2; iCsv2++) {
                string filePath2 = "/home/kali/Documents/GitHub/practice1_2024/" + tjs.schemeName + "/" + table2 + "/" + to_string(iCsv2) + ".csv";
                rapidcsv::Document doc2(filePath2); // открываем файл
                int columnIndex2 = doc2.GetColumnIdx(column2); // считываем индекс искомой колонки
                size_t amountRow2 = doc2.GetRowCount(); // считываем количество строк в файле
                for (size_t j = 0; j < amountRow2; ++j) {
                    cout << doc1.GetCell<string>(0, i) << ": ";
                    cout << doc1.GetCell<string>(columnIndex1, i) << "  |   ";
                    cout << doc2.GetCell<string>(0, j) << ": ";
                    cout << doc2.GetCell<string>(columnIndex2, j) << endl;
                }
            }
        }
    }
}