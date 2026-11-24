# DataVault: In-Memory Database
DataVault iDataVault: In-Memory Database
DataVault is a small C++ command-line program that stores key-value records in a table while it is running. Each record can contain fields such as a name, age, or city. Records are stored in memory and are cleared when the program exits.

Build and run:
You need a C++ compiler that supports C++11 or later. From the folder containing main.cpp, compile the program:
g++ -std=c++11 -Wall -Wextra -pedantic main.cpp -o datavault

Run it on Linux or macOS with:
./datavault
On Windows, run:
datavault.exe

Commands:
Type a command at the > prompt and press Enter.
- INSERT adds a record.
- SELECT ALL displays all records in aligned columns.
- HELP displays the available commands.
- QUIT exits the program.

Commands are not case-sensitive. For example, "select all" and "SELECT ALL" both work.

To add a record
Enter fields as key=value, without spaces around the equals sign:
INSERT name=David age=19
For values containing spaces, use double quotes:
INSERT name="David James" age=19
After a record is added, the program confirms the insert and asks for another command.

To View records:
SELECT ALL
A dash (-) appears when a record does not have a value for a column. If the table is empty, the program says No records yet. After displaying the table, it prints ENTER HELP TO CONTINUE.

Error handling:
Invalid fields, repeated keys, unknown commands, and incorrect SELECT syntax display an error message. The program continues running so you can enter another command.s a simple C++ command-line program for adding and viewing in-memory records.
