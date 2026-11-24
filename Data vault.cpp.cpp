#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using Record = std::map<std::string, std::string>;

std::string trim(const std::string& value) {
    const std::string whitespace = " \t\r\n";
    const std::size_t first = value.find_first_not_of(whitespace);

    if (first == std::string::npos) {
        return "";
    }

    const std::size_t last = value.find_last_not_of(whitespace);
    return value.substr(first, last - first + 1);
}

std::vector<std::string> splitTokens(const std::string& line) {
    std::vector<std::string> tokens;
    std::string token;
    bool inQuotes = false;

    for (char ch : line) {
        if (ch == '"') {
            inQuotes = !inQuotes;
            token += ch;
        } else if ((ch == ' ' || ch == '\t') && !inQuotes) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += ch;
        }
    }

    if (!token.empty()) {
        tokens.push_back(token);
    }

    return tokens;
}

bool parseField(const std::string& token, std::string& key, std::string& value) {
    const std::size_t equals = token.find('=');

    if (equals == std::string::npos) {
        return false;
    }

    key = trim(token.substr(0, equals));
    value = trim(token.substr(equals + 1));

    if (key.empty() || value.empty()) {
        return false;
    }

    if (value.front() == '"') {
        if (value.size() < 2 || value.back() != '"') {
            return false;
        }
        value = value.substr(1, value.size() - 2);
    } else if (value.find('"') != std::string::npos) {
        return false;
    }

    return !value.empty();
}

void printHelp() {
    std::cout << "Available commands:\n"
              << "  INSERT     (add a record)\n"
              << "  SELECT ALL (show all records)\n"
              << "  HELP       (show this command list)\n"
              << "  QUIT       (exit the program)\n"
              << "\nExample: INSERT name=Asha age=21\n"
              << "For values with spaces, use double quotes, like "
                 "name=\"Asha Bello\".\n";
}

void printRecords(const std::vector<Record>& records) {
    if (records.empty()) {
        std::cout << "No records yet.\n";
        return;
    }

    std::set<std::string> columnSet;

    for (const Record& record : records) {
        for (const auto& field : record) {
            columnSet.insert(field.first);
        }
    }

    std::vector<std::string> columns(columnSet.begin(), columnSet.end());
    std::vector<std::size_t> widths;

    for (const std::string& column : columns) {
        std::size_t width = column.size();

        for (const Record& record : records) {
            auto field = record.find(column);

            if (field != record.end()) {
                width = std::max(width, field->second.size());
            } else {
                width = std::max(width, std::string("-").size());
            }
        }

        widths.push_back(width);
    }

    for (std::size_t i = 0; i < columns.size(); ++i) {
        std::cout << "| " << std::left
                  << std::setw(static_cast<int>(widths[i]))
                  << columns[i] << ' ';
    }
    std::cout << "|\n";

    for (std::size_t i = 0; i < columns.size(); ++i) {
        std::cout << "+-" << std::string(widths[i], '-') << '-';
    }
    std::cout << "+\n";

    for (const Record& record : records) {
        for (std::size_t i = 0; i < columns.size(); ++i) {
            auto field = record.find(columns[i]);
            const std::string value =
                field == record.end() ? "-" : field->second;

            std::cout << "| " << std::left
                      << std::setw(static_cast<int>(widths[i]))
                      << value << ' ';
        }
        std::cout << "|\n";
    }
}

int main() {
    std::vector<Record> table;
    std::string line;

    std::cout << "DataVault in-memory table. Type HELP for commands.\n";

    while (true) {
        std::cout << "> ";

        if (!std::getline(std::cin, line)) {
            std::cout << "\nGoodbye.\n";
            break;
        }

        std::vector<std::string> tokens = splitTokens(trim(line));

        if (tokens.empty()) {
            continue;
        }

        std::string command = tokens[0];
        std::transform(command.begin(), command.end(), command.begin(),
                       [](unsigned char ch) {
                           return static_cast<char>(std::toupper(ch));
                       });

        if (command == "QUIT") {
            if (tokens.size() != 1) {
                std::cout << "Error: QUIT does not take any arguments.\n";
                continue;
            }

            std::cout << "Goodbye.\n";
            break;
        } else if (command == "HELP") {
            if (tokens.size() != 1) {
                std::cout << "Error: HELP does not take any arguments.\n";
                continue;
            }

            printHelp();
        } else if (command == "SELECT") {
            std::string selection = tokens.size() > 1 ? tokens[1] : "";
            std::transform(selection.begin(), selection.end(), selection.begin(),
                           [](unsigned char ch) {
                               return static_cast<char>(std::toupper(ch));
                           });

            if (tokens.size() != 2 || selection != "ALL") {
                std::cout << "Error: use SELECT ALL.\n";
                continue;
            }

            printRecords(table);
            std::cout << "ENTER HELP TO CONTINUE\n";
        } else if (command == "INSERT") {
            if (tokens.size() < 2) {
                std::cout << "Error: INSERT needs at least one key=value pair.\n";
                continue;
            }

            

            Record record;
            bool valid = true;
            std::string badToken;

            for (std::size_t i = 1; i < tokens.size(); ++i) {
                std::string key;
                std::string value;

                if (!parseField(tokens[i], key, value)) {
                    valid = false;
                    badToken = tokens[i];
                    break;
                }

                if (record.find(key) != record.end()) {
                    valid = false;
                    badToken = key;
                    break;
                }

                record[key] = value;
            }

            if (!valid) {
                std::cout << "Error: invalid or repeated field '" << badToken
                          << "'. Use key=value, with quotes around values "
                             "that contain spaces.\n";
            } else {
                table.push_back(record);
                std::cout << "Record inserted.\n Enter a new command:\n";
            }
        } else {
            std::cout << "Error: unknown command. Type HELP to see the commands.\n";
        }
    }

    return 0;
}