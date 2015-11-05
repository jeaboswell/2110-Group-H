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
#define null NULL
#elif __linux // Linux compatibility
#include <ncurses.h>
#define CLEAR system("clear"); // Clear the comand prompt (screen)
#define SLEEP sleep(1); // Make program pause for one second before continuing
#define KEYPRESS cout << "Press [Enter] to continue..."; cin.ignore(); // "Press any key to continue..."
#define GETCH getch
#define null NULL
#define INT_MAX 2147483647
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
	string name;
	double distance;
	Edge(string, string, string, double);
};
class Vertex
{
public:
	string name;
	double weight;
	vector<Edge> path;
	Vertex(string, double);
};
#pragma endregion

#pragma region Text Color
namespace Color
{
	enum Code
	{
		FG_RED = 31,
		FG_GREEN = 32,
		FG_BLUE = 34,
		FG_DEFAULT = 39,
		BG_RED = 41,
		BG_GREEN = 42,
		BG_BLUE = 44,
		BG_DEFAULT = 49
	};
	class Modifier
	{
		Code code;
	public:
		Modifier(Code pCode) : code(pCode) {}
		friend std::ostream&
			operator<<(std::ostream& os, const Modifier& mod)
		{
			return os << "\033[" << mod.code << "m";
		};

	};
}
#pragma endregion

#pragma region Function Declarations
string prompt();
vector<string> split(string);
string getFIlePath(string);
bool runCommand(string);
bool nodeExists(string);
void nodeAdd(string);
void nodeDelete(string);
bool edgeExists(string, string);
void edgeAdd(string, string, string, double);
void edgeUpdate(string, string, string, double);
void edgeDelete(string, string);
bool isNumber(string);
Edge getEdge(string, string);
vector<Edge> kruskalAlgorithm();
bool edgeDistanceCompare(Edge, Edge);
bool loopExist(Edge, vector<string>, vector<Edge>);
void connectedNodes(string, vector<string>*, vector<Edge>);
void updateKruskalNodes(Edge, vector<string>*);
void undirectKruskal(vector<Edge>*);
void printKruskal();
bool edgeExists(string, string, vector<Edge>);
void route(string, string);
void dijkstraAlgorithm(string, vector<Edge>*);
void iniVertices(string);
vector<string> adjacentNodes(string);
vector<string> adjacentNodes(string, vector<Edge>);
bool checkVisited(string);
void shortestPath(string, string, vector<Edge>, vector<Edge>*);
void reversePath(vector<Edge>*);
string getOutput(vector<Edge>);
void printShortestPath(string);
void printMatrix();
void printList();
void printEdges();
bool nodeCompare(Node, Node);
bool edgeNameCompare(Edge, Edge);
pair<int, int> getW();
string dtos(double);
bool fileExists(string);
bool verifyExtension(string);
string getExtension(string);
void parseFile(string);
void printHelp();
#pragma endregion