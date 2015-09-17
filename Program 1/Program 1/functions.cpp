#pragma region Includes/Globals
#include "functions.h"
#include <sstream>
#include <iterator>
using namespace std;

extern vector<Node> Nodes;
extern vector<Edge> Edges;
locale loc;
#pragma endregion

bool prompt()
{
	bool check, quit = false;
	string commandLine;
	vector<string> splitCommands;

	cout << "cmd> " << flush;
	getline(cin, commandLine);
	splitCommands = split(commandLine);

	#pragma region No Input Error Check
	if (splitCommands.empty() == true)
	{
		cout << "\a*** ERROR *** NO COMMAND ENTERED" << endl;
		NEWLINE
	}
	#pragma endregion

	#pragma region Node Handling
	else if (splitCommands.front() == "NODE")
	{
		if (splitCommands.size() != 3)
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "ADD") // Add node
		{
			cout << "Add Node" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "DELETE") // Delete node
		{
			cout << "Delete Node" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "SEARCH") // Search for node
		{
			cout << "Search Node" << endl;
			NEWLINE
		}
		else // Error handling
		{
			cout << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR NODE: " << splitCommands.at(1) << endl;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region Edge Handling
	else if (splitCommands.front() == "EDGE")
	{
		if (splitCommands.size() != 4)
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "ADD") // Add edge
		{
			cout << "Add Edge" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "DELETE") // Delete edge
		{
			cout << "Delete Edge" << endl;
			NEWLINE
		}
		else // Error handling
		{
			cout << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR EDGE: " << splitCommands.at(1) << endl;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region Print Handling
	else if (splitCommands.front() == "PRINT")
	{
		if (splitCommands.size() != 2)
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "MATRIX") // Print Matrix
		{
			cout << "Print Matrix" << endl;
			NEWLINE
		}
		else if (splitCommands.at(1) == "LIST") // Print list
		{
			cout << "Print List" << endl;
			NEWLINE
		}
		else // Error handling
		{
			cout << "\a*** ERROR *** COMMAND NOT RECOGNIZED FOR PRINT: " << splitCommands.at(1) << endl;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region CSV File Handling
	else if (splitCommands.front() == "FILE")
	{
		if (splitCommands.size() != 2)
		{
			cout << "\a*** ERROR *** MISSING PARAMETERS" << endl;
			NEWLINE
		}
		else // Execute commands from CSV file
		{
			cout << "Parse File" << endl;
			NEWLINE
		}
	}
	#pragma endregion

	#pragma region Clear Screen
	else if (splitCommands.front() == "CLEAR")
		CLEAR
	#pragma endregion

	#pragma region Quit
	else if (splitCommands.front() == "QUIT")
	{
		quit = true;
		CLEAR
	}
	#pragma endregion
	
	#pragma region Error Check First Command
	else
	{
		cout << "\a*** ERROR *** COMMAND NOT RECOGNIZED: " << splitCommands.front() << endl;
		NEWLINE
	}
	#pragma endregion

	return quit;
}

vector<string> split(string input)
{
	string temp;
	for (int i = 0; i < input.length(); i++) // Convert input into all lower case
		temp += toupper(input[i], loc);
	istringstream buf(temp);
	istream_iterator<string> beg(buf), end;

	vector<string> output(beg, end);

	return output;
}