/*
CSCE 2110.001 Group H
Jesse Boswell
Craig Nafus
Daniel Lee
Christian Betancourt
Taylor DeLaughter
Nancy Joy
Satvir Singh
Ameera Fatima
*/

#pragma region Includes/Globals
#include "functions.h"
using namespace std;

vector<Node> Nodes;
vector<Edge> Edges;
#pragma endregion

int main()
{
	#pragma region Variables
	string command;
	bool quit = false;
	#pragma endregion

	CLEAR

	while (quit == false)
	{
		command = prompt();
		quit = runCommand(command);
	}

	CLEAR
	return 0;
} 