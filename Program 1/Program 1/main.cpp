/*
CSCE 2110.001 Group H
Jesse Boswell
Craig Nafus
Daniel Lee

*/

#pragma region Includes/Globals
#include "functions.h"
using namespace std;

vector<Node> Nodes;
vector<Edge> Edges;
#pragma endregion

// Try to do as much as possible in functions
int main()
{
	#pragma region Variables
	bool quit = false;
	string command;
	#pragma endregion

	CLEAR

	while (quit == false)
	{
		command = prompt();
		quit = runCommand(command);
	}
	return 0;
} 