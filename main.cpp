#include "parser.h"
#include "structure.h"
#include "parser.cpp"
#include "insert.cpp"


enum class Commands { // существующие команды
    EXIT,
    INSERT,
    DELETE,
    SELECT,
    ERR
};

Commands stringToCommand(const string& cmd) { // определение команд
    istringstream iss(cmd); // поток ввода для обработки строки
    string word;
    iss >> word;
    if (word == "EXIT") {
        return Commands::EXIT;
    }
    else if (word == "INSERT") {
        return Commands::INSERT;
    }
    else if (word == "DELETE") {
        return Commands::DELETE;
    }
    else if (word == "SELECT") {
        return Commands::SELECT;
    }
    else {
        return Commands::ERR;
    }
}

int main() {
    tableJson tjs;
    parsing(tjs);
    cout << "\n\n";
    string command; // строка для команды
    /*while (true) {
        cout << "Введите команду: ";
        getline(cin, command);
        if (command == "") { // если пустая строка
            continue;
        }
        Commands cmd = stringToCommand(command); // обработка введённой команды
        switch (cmd) {
            case Commands::EXIT: // выход
                return 0;
            case Commands::INSERT: // вставка
                //insert(command, tjs);
                break;
            case Commands::ERR:
                cerr << "Неизвестная команда.\n";
                break;
        }
    }*/
    cout << isLocked("table1", tjs.schemeName) << endl;
    locker("table1", tjs.schemeName);
    cout << isLocked("table1", tjs.schemeName) << endl;
    locker("table1", tjs.schemeName);
    cout << isLocked("table1", tjs.schemeName) << endl;
    return 0;
}