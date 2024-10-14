#include "header.h"

void removeDirectory(const fs::path& directoryPath) { // удаление директории
    if (fs::exists(directoryPath)) { // проверка, существует ли она вообще
        fs::remove_all(directoryPath); // удаление
    }
}

void createDirectoriesAndFiles(const fs::path& schemePath, const json& structure, tableJson& tjs) { // создание полной директории и файлов
    tNode* tableHead = nullptr; // указатель на первый узел списка таблиц
    tNode* tableTail = nullptr; // указатель на последний узел списка таблиц

    for (const auto& table : structure.items()) { // цикл по всем элементам в структуре json
        fs::path tablePath = schemePath / table.key(); // key - имя таблицы
        if (!fs::create_directory(tablePath)) {
            cerr << "Не удалось создать директорию: " << tablePath << endl;
            return;
        }
        cout << "Создана директория: " << tablePath << endl;

        tNode* newTable = new tNode{table.key(), nullptr, nullptr}; // создаём таблицу
        fs::current_path(tablePath); // переходим в папку таблицы
        string lock = table.key() + "_lock.txt"; // создаём файл блокировки
        ofstream file(lock);
        if (!file.is_open()) {
            cerr << "Не удалось открыть файл.\n";
        }
        file << "unlocked"; // по умолчанию разблокировано
        file.close();
        if (tableHead == nullptr) { // добавляем таблицу в список
            tableHead = newTable; // если список пустой, таблица будет и первой, и последней одновременно
            tableTail = newTable;
        }
        else {
            tableTail->next = newTable; // иначе добавляем новую таблицу в конец списка
            tableTail = newTable; 
        }

        string keyColumn = table.key() + "_pk"; // название специальной колонки
        Node* column_pk = new Node{keyColumn, nullptr}; // создаём список, где специальная колонка - первая
        newTable->column = column_pk; // присоединяем список колонок к таблице

        fs::path csvFilePath = tablePath / "1.csv"; // создаём csv файл
        ofstream csvFile(csvFilePath);
        if (!csvFile.is_open()) {
            cerr << "Не удалось создать файл: " << csvFilePath << endl;
            return;
        }
        csvFile << keyColumn << ",";
        const auto& columns = table.value(); // запись колонок в файл, объект columns = названия
        for (size_t i = 0; i < columns.size(); ++i) { 
            csvFile << columns[i].get<string>(); // записываем названия без кавычек
            Node* newColumn = new Node{columns[i], nullptr}; // создаём новую колонку
            if (newTable->column == nullptr) { // если в таблице ещё нет колонок
                newTable->column = newColumn;
            }
            else {
                Node* lastColumn = newTable->column;
                while (lastColumn->next != nullptr) { // ищем последнюю колонку
                    lastColumn = lastColumn->next;
                }
                lastColumn->next = newColumn; // добавляем новую колонку в конец
            }
            if (i < columns.size() - 1) { // для последнего значения не нужна запятая
                csvFile << ",";
            }
        }
        csvFile << endl;
        csvFile.close();
        cout << "Создан файл: " << csvFilePath << endl;

        string pk = keyColumn + "_sequence.txt"; // создаём файл для хранения уникального первичного ключа
        ofstream filePk(pk);
        if (!filePk.is_open()) {
            cerr << "Не удалось открыть файл.\n";
        }
        filePk << "0";
        filePk.close();
    }
    tjs.tablehead = tableHead;
}

void parsing(tableJson& tjs) { // парсинг схемы
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

    tjs.schemeName = jparsed["name"]; // извлекаем имя схемы
    fs::path schemePath = fs::current_path() / tjs.schemeName; // формируем путь к директории
    removeDirectory(schemePath); // удаляем, чтобы заново создать директорию
    if (!fs::create_directory(schemePath)) { // проверка
        cerr << "Не удалось создать директорию: " << schemePath << endl;
        return;
    }
    cout << "Создана директория: " << schemePath << endl;
    if (jparsed.contains("structure")) { // наполнение директории
        createDirectoriesAndFiles(schemePath, jparsed["structure"], tjs);
    }
    tjs.tableSize = jparsed["tuples_limit"]; // вытаскиваем ограничения по строкам
}

tNode* createTable(const string& tableName) { // создание новой таблицы
    tNode* newTable = new tNode{tableName, nullptr, nullptr};
    return newTable;
}

/*bool isLocked(const string& table, const string& schemeName) { // проверка состояния блокировки
    string tablePath = "/home/kali/Documents/GitHub/practice1_2024/" + schemeName + "/" + table + "/" + table + "_lock.txt";
    ifstream locker(tablePath);
    string line;
    if (locker.is_open()) {
        getline(locker, line);
    }
    locker.close();
    if (line == "locked"){
        return true;
    }
    return false;
}*/

/*void locker(const string& table, const string& schemeName) { // изменение состояния блокировки
    string tablePath = "/home/kali/Documents/GitHub/practice1_2024/" + schemeName + "/" + table + "/" + table + "_lock.txt";
    ofstream file(tablePath);
    if (isLocked(table, schemeName)){
        file << "unlocked";
    }
    if (!isLocked(table, schemeName)){
        file << "locked";
    }
    file.close();
}*/

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

string pKey(const string& table, const string& schemeName) { // изменение текущего ключа
    string tablePath = "/home/kali/Documents/GitHub/practice1_2024/" + schemeName + "/" + table + "/" + table + "_pk_sequence.txt";
    ifstream pkFile(tablePath); // чтение текущего ключа
    if (!pkFile.is_open()) {
        cerr << "Не удалось открыть файл.\n";
    }
    string pk;
    pkFile >> pk;
    pkFile.close();
    ofstream newPk(tablePath); // запись нового ключа
    if (!newPk.is_open()) {
        cerr << "Не удалось открыть файл.\n";
    }
    newPk << to_string(stoi(pk) + 1); // прибавляем 1
    newPk.close();
    return to_string(stoi(pk) + 1);
}

string columnsFile(const string& filePath) { // получение строки с колонками
    ifstream file(filePath);
    string columns; // строка для записи колонок
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл.\n";
        return "";
    }
    getline(file, columns);
    file.close();
    return columns;
}

void insert(const string& command, tableJson& tjs) { // вставка
    istringstream iss(command); // создание объекта для разбора команды
    string word; // текущее слово
    string tableName; // имя таблицы
    int pos = 1; // текущая позиция слова в команде
    string vals;
    while (iss >> word) {
        pos++;
        if (pos == 3 && word != "INTO") { // проверка: 2 слово INTO
            cerr << "Неизвестная команда.\n";
            return;
        }
        if (pos == 4) {
            if (isTableExist(tjs.tablehead, word) == false) { // проверка: заданная таблица существует
                cerr << "Такой таблицы не существует.\n";
                return;
            }
            else {
                tableName = word;
            }
        }
        if (pos == 5 && word != "VALUES") { // проверка: 4 слово VALUES
            cerr << "Неизвестная команда.\n";
            return;
        }
        if (pos == 6) {
            vals = word; // сохраняем строку значений
            while (iss >> word) {
                vals += " " + word; // добавляем оставшиеся слова к строке значений
            }
            if (vals.front() != '(' || vals.back() != ')') { // правильность введённых значений
                cerr << "Неизвестная команда.\n";
                return;
            }
        }
        if (pos == 6) {
            /*if (isLocked(tableName, tjs.schemeName) == 1) { // проверка на доступность текущей таблицы
                cerr << "Таблица уже используется.\n";
                return;
            }

            locker(tableName, tjs.schemeName); // блокируем текущую таблицу*/
            string currentPk = pKey(tableName, tjs.schemeName); // получаем новый ключ
            string values = currentPk + ","; // строка со всеми значениями
            for (int i = 0; i < vals.size(); i++) {
                if (vals[i] == '\'') { // если встречается кавычка, начинаем записывать слово в val
                    i++;
                    string val;
                    while (vals[i] != '\'') {
                        val += vals[i]; // записываем слово
                        i++;
                    }
                    if (vals[i + 1] == ')') { // если слово последнее
                        values += val + "\n";
                    }
                    else { // если не последнее
                        values += val + ",";
                    }
                }
            }
            int csvNumber = 1; // номер файла csv
            while (true) {
                string filePath = "/home/kali/Documents/GitHub/practice1_2024/" + tjs.schemeName + "/" + tableName + "/" + to_string(csvNumber) + ".csv";
                ifstream csv(filePath); // открываем/создаём csv файл
                if (!csv.is_open()) {
                    ofstream newCsv(filePath);
                    if (!newCsv.is_open()) {
                        cerr << "Не удалось создать файл.\n";
                        return;
                    }
                    newCsv.close();
                    csv.open(filePath); // Повторно открываем файл для чтения
                }
                csv.close();
                rapidcsv::Document csvfile(filePath); // считываем содержимое в csvfile 
                if (csvfile.GetRowCount() < tjs.tableSize) { 
                    break;
                }
                csvNumber++; // если лимит строк превышен, переходим к файлу со следующим порядковым номером
            }

            string filePath = "/home/kali/Documents/GitHub/practice1_2024/" + tjs.schemeName + "/" + tableName + "/" + to_string(csvNumber) + ".csv";
            ifstream fileCsv(filePath); // открываем файл для чтения
            ofstream file(filePath, ios::app); // открываем файл для записи, ios::app - содержимое будет добавляться в конец
            if (!fileCsv.is_open()) {
                cerr << "Не удалось открыть файл.\n";
                return;
            }
            string stroka;
            getline(fileCsv, stroka);
            if (stroka.size() < 1) { // если файл пуст
                string columns = columnsFile("/home/kali/Documents/GitHub/practice1_2024/" + tjs.schemeName + "/" + tableName + "/1.csv");
                file << columns << endl; // записываем колонки
            }
            fileCsv.close();
            if (!file.is_open()) {
                cerr << "Не удалось открыть файл.\n";
                return;
            }
            file << values; // записываем значения
            file.close();
        }
    }
    //locker(tableName, tjs.schemeName);
}