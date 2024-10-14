#include "parsing_json.hpp"
#include "structure.hpp"

enum class Commands { // существующие команды
    EXIT,
    INSERT,
    DELETE,
    SELECT,
    UNKNOWN
};

Commands stringToCommand(const string& str) { // определение команд
    if (str == "EXIT") {
        return Commands::EXIT;
    }
    else if (str == "INSERT") {
        return Commands::INSERT;
    }
    else if (str == "DELETE") {
        return Commands::DELETE;
    }
    else if (str == "SELECT") {
        return Commands::SELECT;
    }
    else {
        cerr << "Неизвестная команда.\n";
        return Commands::UNKNOWN;
    }
}

int main() {
    parsing();
    cout << "\n\n";
    string command; // строка для команды
    while (true) {
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
                insert(command);
                break;
        }
    }
    return 0;
}