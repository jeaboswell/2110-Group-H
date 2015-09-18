#pragma once
#pragma region Includes/Defines
#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>
#include <vector>
#include <locale>
using namespace std;

#define NEWLINE cout << endl;

#ifdef _WIN32 // Windows compatibility
#include <Windows.h>
#include <conio.h>
#define CLEAR system("cls"); // Clear the comand prompt (screen)
#define SLEEP Sleep(1000); // Make program pause for one second before continuing
#define KEYPRESS cout << "Press any key to continue..."; _getch(); // "Press any key to continue..."
#elif __linux // Linux compatibility
#include <ncurses.h>
#define CLEAR system("clear"); // Clear the comand prompt (screen)
#define SLEEP sleep(1); // Make program pause for one second before continuing
#define KEYPRESS cout << "Press any key to continue..."; cin.ignore(); // "Press any key to continue..."
#endif
#pragma endregion

#pragma region Classes
class Node
{
public:
	string name;
	double value;
};
class Edge
{
public:
	Node* start;
	Node* end;
	double length;
};
#pragma endregion

#pragma region Function Declarations
string prompt();
vector<string> split(string);
bool runCommand(string);
#pragma endregion 