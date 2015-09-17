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
	#pragma endregion

	CLEAR

	while (quit == false)
	{
		quit = prompt();
	}
	return 0;
} 

/*
For printing all items in vector

for(auto& temp: vector)
std::cout << '"' << temp << '"' << '\n';
*/