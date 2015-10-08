#pragma once
#pragma region Includes/Defines
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <locale>
#include <string>
#include <vector>
using namespace std;

#define NEWLINE cout << endl;

#ifdef _WIN32 // Windows compatibility
#include <conio.h>
#include <Windows.h>
#define CLEAR system("cls"); // Clear the comand prompt (screen)
#define SLEEP Sleep(1000); // Make program pause for one second before continuing
#define KEYPRESS cout << "Press any key to continue..."; _getch(); // "Press any key to continue..."
#define GETCH _getch
#elif __linux // Linux compatibility
#include <ncurses.h>
#define CLEAR system("clear"); // Clear the comand prompt (screen)
#define SLEEP sleep(1); // Make program pause for one second before continuing
#define KEYPRESS cout << "Press [Enter] to continue..."; cin.ignore(); // "Press any key to continue..."
#define GETCH getch
#endif
#pragma endregion

#pragma region Classes
class Node
{
public:
	string name;
	Node(string);
};
class Edge
{
public:
	string start;
	string end;
	double length;
	Edge(string, string);
};
#pragma endregion

#pragma region Text Color
namespace Color {
	enum Code {
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_DEFAULT = 39,
		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49
	};
	class Modifier {
		Code code;
	public:
		Modifier(Code pCode) : code(pCode) {}
		friend std::ostream&
			operator<<(std::ostream& os, const Modifier& mod) {
			return os << "\033[" << mod.code << "m";
		}
	};
}
#pragma endregion

#pragma region Function Declarations
string prompt();
vector<string> split(string);
string getPath(string);
bool runCommand(string);
bool nodeExists(string);
void nodeAdd(string);
void nodeDelete(string);
bool edgeExists(string, string);
void edgeAdd(string, string);
void edgeDelete(string, string);
void printMatrix();
void printList();
bool nameCompare(Node, Node);
bool fileExists(string);
bool verifyExtension(string);
string getExtension(string);
void parseFile(string);
void printHelp();
#pragma endregion 