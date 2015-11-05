/*
CSCE 2110.001
Group H
	Jesse Boswell

*/

#pragma region Includes/Globals
#include "GroupH_10_P4_Header.h"
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